# Software Design Document

**Project:** Origami UI
**Selected Use Case:** UC-3 — Interact with Controls
**Version:** 1.0
**Date:** February 2026

---

## Document-Specific Task Matrix

| Task | Responsible Member |
|---|---|
| System Overview, System Context | Ahmet Batuhan Günal |
| Architectural Design, Component Design | Archyn Mikhailov |
| Data Design, Design Patterns | Archyn Mikhailov |
| Implementation Notes, UI Design, Interfaces | Tuna Kömürcü |
| Performance, Error Handling, Testability, Deployment | Tuna Kömürcü |


---

## 1. System Overview

Origami UI is a lightweight, immediate-mode GUI library written in portable ANSI C (C11). It provides a set of interactive UI controls — buttons, text inputs, checkboxes, sliders, toggles, and more — that application developers call as functions each frame. The library does not draw to the screen directly; it produces a stream of abstract drawing commands (filled rectangles, text glyphs, icons, clipping regions) that the host application renders through its own graphics pipeline.

This document describes the design of the **Use Case 3: Interact with Controls** implementation. In this use case, the End User interacts with rendered UI controls through mouse and keyboard input. The host application feeds raw input events into the library, which routes them to the appropriate control based on spatial hit-testing, hover state, and focus state. Each control function returns signal flags (`OU_SIGNAL_COMMIT`, `OU_SIGNAL_ALTERED`) that the application developer acts upon.

The implementation covers the Phase 1 styled controls: **Button** (with four variants: Default, Secondary, Outline, Ghost), **Input** (with placeholder text, Ctrl+A selection, cursor feedback), **Checkbox**, **Slider** (with filled progress track), and **Toggle**.

## 2. System Context

Origami UI operates as an embedded component within a host application. It does not own a window, a rendering context, or an event loop. The host application is responsible for:

1. **Providing a rendering backend** — translating Origami's drawing commands into actual pixels (via OpenGL, Vulkan, SDL2, DirectX, or a software rasterizer).
2. **Feeding input events** — converting platform-specific mouse/keyboard events into Origami's `ou_feed_*()` calls.
3. **Supplying text measurement callbacks** — the library calls `text_width` and `text_height` function pointers to measure strings without depending on any font system.

```
+--------------------------------------------------------+
|                    Host Application                    |
|                                                        |
|  +--------------+ +--------------+ +----------------+  |
|  |  Renderer    | |  Event Loop  | |  Font System   |  |
|  |  (SDL2/GL)   | |  (SDL2/OS)   | |  (stb/FT2)     |  |
|  +------+-------+ +------+-------+ +-------+--------+  |
|         |                |                  |          |
|         | ou_walk()      | ou_feed_*()      | callbacks|
|         v                v                  v          |
|  +---------------------------------------------------+ |
|  |                 Origami UI Core                   | |
|  |                                                   | |
|  |  +-----------+ +-----------+ +------------------+ | |
|  |  |  Layout   | |  Input    | | Control Widgets  | | |
|  |  |  Engine   | |  Router   | | (Phase 1 Styled) | | |
|  |  +-----------+ +-----------+ +------------------+ | |
|  |  +-----------+ +-----------+ +------------------+ | |
|  |  |  Command  | |  Theme    | | ID / Focus Mgmt  | | |
|  |  |  Buffer   | |  System   | | (FNV-1a Hashing) | | |
|  |  +-----------+ +-----------+ +------------------+ | |
|  +---------------------------------------------------+ |
+--------------------------------------------------------+
```

### Actors

- **End User** — Interacts with the rendered UI through mouse movement, clicks, drags, scrolls, and keyboard input.
- **Application Developer** — Integrates the library, calls control functions, and acts on returned signals.

## 3. Key Features and Functionality

The implemented use case delivers the following interactive controls:

| Control | Function | Interaction Model |
|---|---|---|
| **Button** (4 variants) | `ou_btn_ex()` | Click to commit. Variants: Default, Secondary, Outline, Ghost. Visual feedback on hover and press (1px content shift). |
| **Input** | `ou_input_ex()` | Click to focus, type to enter text. Backspace to delete. Enter to commit. Ctrl+A to select all. Placeholder text when empty. I-beam cursor on hover. |
| **Checkbox** | `ou_check_ex()` | Click to toggle boolean state. Visual: filled box with tick when checked, empty box when unchecked. Hover/press feedback on border. |
| **Slider** | `ou_slide_ex()` | Click/drag to change floating-point value within a range. Filled track shows progress. Shift+click for direct number entry. |
| **Toggle** | `ou_toggle()` | Click to toggle boolean state. Visual: sliding handle on a track. Bright handle when on, standard handle when off. |

Each control follows a consistent interaction contract:
- Returns `OU_SIGNAL_COMMIT` on definitive user action (button click, Enter in text field).
- Returns `OU_SIGNAL_ALTERED` when the control's value changes (checkbox toggled, slider dragged).
- Returns `0` when no interaction occurred this frame.

## 4. Assumptions and Dependencies

### Assumptions

1. The host application calls `ou_frame_begin()` and `ou_frame_end()` exactly once per frame, with all control calls between them.
2. Input events are fed before `ou_frame_begin()` or between `ou_frame_begin()` and control calls.
3. The host application provides accurate `text_width` and `text_height` callbacks before the first frame.
4. A single thread accesses a given `ou_context` at any time.

### Dependencies

| Dependency | Type | Purpose |
|---|---|---|
| C Standard Library (`stdio.h`, `stdlib.h`, `string.h`) | Build-time | String operations, `sprintf`, `strtod`, `abort` |
| SDL2 | Demo only | Windowing, input, rendering in the reference demo |
| Host renderer | Runtime | Translates drawing commands to pixels |
| Host font system | Runtime | Provides `text_width`/`text_height` callbacks |

The core library (`origami.h` + `origami.c`) has **zero external dependencies** beyond the C standard library.

## 5. Architectural Design

### 5.1 System Architecture Diagram (High-Level)

The library follows a **pipeline architecture** where each frame flows through distinct stages:

```
 Input Events      UI Description       Command Stream      Rendered Output
+-------------+   +---------------+   +----------------+   +---------------+
| ou_feed_*   |-->| Control Calls |-->| Command Buffer |-->| ou_walk()     |
| (mouse,     |   | (ou_btn_ex,   |   | (FILL, GLYPH,  |   | -> Renderer   |
|  keyboard)  |   |  ou_input_ex, |   |  SCISSOR, SKIP)|   |               |
+-------------+   |  ou_slide_ex) |   +----------------+   +---------------+
                  +---------------+
       ^                 |
       |                 v
  +---------------------------------+
  |   ou_context (Single Struct)    |
  |  +--------+ +-------+ +------+  |
  |  | Input  | |Layout | |  ID  |  |
  |  | State  | |Stacks | | Mgmt |  |
  |  +--------+ +-------+ +------+  |
  +---------------------------------+
```

### 5.2 Architectural Patterns and Styles

**Layered Architecture** — The library is organized into layers with clear responsibilities:

| Layer | Components | Responsibility |
|---|---|---|
| **Infrastructure** | Hash system, cache pools, stack macros, rect helpers | Identity, memory management, utility |
| **Core Services** | Layout engine, input router, clipping, command buffer | Spatial arrangement, event dispatch, draw command generation |
| **Control Helpers** | `ou_sense`, `ou_decorate`, `ou_inscribe`, `ou_hovering` | Shared interaction and rendering logic for all controls |
| **Widgets** | `ou_btn_ex`, `ou_input_ex`, `ou_check_ex`, `ou_slide_ex`, `ou_toggle` | User-facing interactive controls |
| **Containers** | Windows, panels, popups | Scrollable, draggable framed regions |

**Immediate-Mode Architecture** — No retained widget tree. The UI is described as a sequence of function calls each frame. State is derived from the `ou_context` struct, not from persistent widget objects.

### 5.3 Rationale for Architectural Decisions

1. **Single context struct** — All state lives in one `ou_context` allocated by the host. This eliminates global state, supports multiple independent UI instances, and makes the library's memory footprint fully deterministic.

2. **Fixed-size buffers with no dynamic allocation** — All internal stacks and pools use compile-time sizes. This guarantees constant memory usage, avoids fragmentation, and makes the library suitable for embedded systems.

3. **Command-based rendering** — Controls produce abstract drawing commands instead of calling a renderer directly. This decouples the UI logic from any specific graphics API, making the library portable across OpenGL, Vulkan, SDL2, DirectX, and software rasterizers.

4. **Hash-based identity** — Controls are identified by FNV-1a hashes of their label/pointer, not by persistent objects. This enables the immediate-mode pattern where controls are created and destroyed implicitly each frame.

## 6. Component Design

### 6.1 Subsystems and Modules

```
origami.c
|-- Constructors         (ou_vec2_make, ou_rect_make, ou_color_make)
|-- Rect Helpers         (grow_rect, overlap_rects, point_in_rect)   [static]
|-- Lifecycle            (ou_setup, ou_frame_begin, ou_frame_end)
|-- Identity System      (ou_hash, ou_push_scope, ou_pop_scope)       [FNV-1a]
|-- Focus Management     (ou_grant_focus)
|-- Clipping             (ou_push_scissor, ou_pop_scissor, ou_test_scissor)
|-- Layout Engine        (ou_row, ou_allocate, ou_column_begin/end)
|-- Input Router         (ou_feed_motion/press/release/scroll/key/text)
|-- Command Buffer       (ou_emit, ou_walk, emit_skip)
|-- Drawing Primitives   (ou_paint_rect, ou_paint_outline, ou_paint_text, ou_paint_symbol)
|-- Control Helpers      (ou_sense, ou_decorate, ou_inscribe, ou_hovering)
|-- Base Widgets         (ou_label, ou_paragraph, ou_button_ex, ou_checkbox, ...)
|-- Styled Controls      (ou_btn_ex, ou_input_ex, ou_check_ex, ou_slide_ex, ou_toggle)
|-- Collapsible/Tree     (ou_section_ex, ou_tree_begin_ex, ou_tree_end)
|-- Scrollbars           (render_scrollbar macro, handle_scrollbars)
|-- Containers           (ou_window_begin_ex, ou_panel_begin_ex, ou_popup_begin)
+-- Color Utility        (shift_color)                                [static]
```

### 6.2 Responsibilities of Each Component

**Identity System** — Generates unique 32-bit IDs for each control using FNV-1a hashing. Supports scoped hashing (`ou_push_scope`/`ou_pop_scope`) to disambiguate controls with identical labels in different containers.

**Input Router** — Stores raw input state (mouse position, button state, key state, text input buffer) in the context. Does not directly dispatch to controls; instead, each control queries the input state during its `ou_sense()` call.

**Layout Engine** — Manages a stack of `ou_grid` structs. `ou_row()` configures column widths and row height. `ou_allocate()` returns the next available rectangle. Supports absolute pixel widths, relative-to-container widths (negative values), and default sizes.

**Control Helpers** — Four functions that encapsulate the common control pattern:
- `ou_sense(id, rect, opt)` — Hit-tests the mouse against the control's rect, updates hover/focus state.
- `ou_decorate(id, rect, colorid, opt)` — Draws the control background with automatic hover/focus color shifting (+1/+2 to the color index).
- `ou_inscribe(str, rect, colorid, opt)` — Draws text aligned within a rectangle (left, center, or right).
- `ou_hovering(rect)` — Returns whether the mouse is over a rectangle within the current hover root.

**Styled Controls** — The Phase 1 controls built for this use case. Each follows a consistent internal pattern:
1. `ou_hash()` -- generate unique ID
2. `ou_allocate()` -- get layout rectangle
3. `ou_sense()` -- process input
4. Custom rendering -- emit drawing commands
5. Return signal flags

### 6.3 Interfaces Between Components

Controls interact with the core through a set of well-defined internal APIs:

```
Control (e.g., ou_btn_ex)
    |
    +--> ou_hash()          Identity system -> returns ou_id
    +--> ou_allocate()      Layout engine   -> returns ou_rect
    +--> ou_sense()         Input router    -> updates ctx->hover, ctx->focus
    +--> ou_paint_rect()    Command buffer  -> emits OU_CMD_FILL
    +--> ou_paint_outline() Command buffer  -> emits OU_CMD_FILL (4x for edges)
    +--> ou_inscribe()      Drawing + text  -> emits OU_CMD_GLYPH (clipped)
    +--> return signals     To host app     -> OU_SIGNAL_COMMIT / OU_SIGNAL_ALTERED
```

### 6.4 Component Diagram

```
+-----------------------------------------------------------+
|                        ou_context                         |
|                                                           |
|  +--------------+  +--------------+  +----------------+   |
|  |  Identity    |  |   Layout     |  |  Input State   |   |
|  |  System      |<-|   Engine     |  |  (mouse, keys) |   |
|  |  (FNV-1a)    |  |  (grid stk)  |  |                |   |
|  +------+-------+  +------+-------+  +-------+--------+   |
|         |                 |                   |           |
|         v                 v                   v           |
|  +-----------------------------------------------------+  |
|  |             Control Helpers Layer                   |  |
|  |  ou_sense()  ou_decorate()  ou_inscribe()           |  |
|  +--------+-----------+-----------+--------------------+  |
|           |            |          |                       |
|           v            v          v                       |
|  +------------+  +------------+  +------------------+     |
|  | ou_btn_ex  |  |ou_input_ex |  | ou_check/slide/  |     |
|  | (Button)   |  | (Input)    |  | toggle           |     |
|  +-----+------+  +-----+------+  +--------+---------+     |
|        |               |                   |              |
|        v               v                   v              |
|  +-----------------------------------------------------+  |
|  |             Command Buffer (256 KB)                 |  |
|  |  OU_CMD_FILL | OU_CMD_GLYPH | OU_CMD_SYMBOL |       |  |
|  |  OU_CMD_SCISSOR | OU_CMD_SKIP                       |  |
|  +-----------------------------------------------------+  |
+-----------------------------------------------------------+
```

## 7. Data Design

### 7.1 Data Model

The library's data is entirely contained in the `ou_context` struct. There is no external database or file storage.

**Core Data Entities:**

| Entity | Type | Purpose |
|---|---|---|
| `ou_context` | struct (≈270 KB) | All library state for one UI instance |
| `ou_theme` | struct (embedded) | 14 colors, spacing, padding, font, callbacks |
| `ou_frame` | struct (pool of 48) | Retained state for windows/panels (rect, scroll, z-index) |
| `ou_grid` | struct (stack of 16) | Current layout state (position, widths, row config) |
| `ou_cmd` | union (in 256 KB buffer) | A single drawing command |
| `ou_cache_slot` | struct (pools of 48) | LRU cache entry mapping hash ID to pool index |

### 7.2 Data Storage

All data is stored in **statically-sized fields** within `ou_context`:

```
ou_context memory layout (approximate):
+------------------------------------------------+
| Callbacks (3 function pointers)           24B   |
| Theme struct                            ~100B   |
| Core state (hover, focus, tick...)       ~60B   |
| Command buffer                        262144B   | (256 KB)
| Root list (32 pointers)                 256B    |
| Frame stack (32 pointers)               256B    |
| Scissor stack (32 rects)                512B    |
| Scope stack (32 IDs)                    128B    |
| Grid stack (16 grids)                ~2048B     |
| Frame pool (48 slots + 48 frames)    ~3456B     |
| Tree pool (48 slots)                   384B     |
| Input state (mouse, keys, text)        ~100B    |
| Cursor & selection state                 ~8B    |
+------------------------------------------------+
| TOTAL                                 ~270 KB   |
+------------------------------------------------+
```

### 7.3 Data Flow Diagram

```
                          Per-Frame Data Flow

+------------+  +--------------+  +--------------+  +------------+
| SDL2 /     |  | ou_feed_*    |  | ou_frame_    |  | Control    |
| Host       |->| functions    |->| begin()      |->| Calls      |
| Events     |  |              |  |              |  |            |
+------------+  +--------------+  +--------------+  +------+-----+
                                                           |
               +-------------------------------------------+
               |
               v
+-----------------+  +--------------+  +------------------+
| ou_sense()      |  | ou_paint_*   |  | ou_frame_end()   |
| reads mouse_pos |->| emits cmds   |->| sorts roots,     |
| and mouse_down  |  | to buffer    |  | resets state     |
+-----------------+  +--------------+  +---------+--------+
                                                 |
                                                 v
                                       +-------------------+
                                       | ou_walk()         |
                                       | yields commands   |
                                       | to host renderer  |
                                       +-------------------+
```

### 7.4 Data Validation Rules

| Data | Validation | Mechanism |
|---|---|---|
| Stack push/pop | Index within bounds | `verify()` assertion macro |
| Command buffer writes | Sufficient space remaining | `verify()` assertion in `ou_emit()` |
| Callback assignment | `text_width` and `text_height` non-NULL | `verify()` in `ou_frame_begin()` |
| Text input buffer | Length < buffer capacity | `ou_min()` clamp before `memcpy` |
| Slider value | Clamped to `[low, high]` | `ou_clamp()` after computation |
| Color channel arithmetic | Clamped to `[0, 255]` | `ou_clamp()` in `shift_color()` |

## 8. Design Patterns

### 8.1 Applied Design Patterns

#### 8.1.1 Command Pattern

**Intent:** Encapsulate drawing operations as objects that can be stored, ordered, and executed later.

**Application:** Every `ou_paint_*()` call creates a command object (`ou_cmd`) in the command buffer. The host application later iterates the buffer via `ou_walk()` and executes each command against its renderer.

**Motivation:** This decouples UI logic from rendering. The library knows nothing about OpenGL, Vulkan, or SDL — it only produces a portable command stream. Different renderers can consume the same commands.

**Improvement:** Enables depth-sorted rendering (windows are sorted by z-index and their command chains are linked via `OU_CMD_SKIP` pointers), scissor-based clipping without renderer coupling, and potential command batching for performance.

**Code location:** `ou_emit()`, `ou_walk()`, `ou_paint_rect()`, `ou_paint_text()`, `ou_paint_symbol()`, `ou_clip()` in `origami.c`.

#### 8.1.2 Strategy Pattern

**Intent:** Define a family of algorithms, encapsulate each one, and make them interchangeable.

**Application:** Three callback function pointers on `ou_context` allow the host application to inject its own implementations:
- `text_width(font, str, len)` — measures text width in pixels.
- `text_height(font)` — returns font line height.
- `draw_frame(ctx, rect, colorid)` — renders a control frame (background + optional outline).

**Motivation:** The library must work with any font system and any rendering style. By using callbacks, different applications can plug in FreeType, stb_truetype, bitmap fonts, or any custom system without modifying the library.

**Improvement:** Eliminates hard dependencies on font libraries and rendering APIs. The `draw_frame` callback enables application-specific visual styles (e.g., rounded corners, gradients) without changing library code.

**Code location:** `ou_context` struct (callback declarations), `ou_setup()` (default `draw_frame` assignment), all controls that call `ctx->text_width()` and `ctx->draw_frame()`.

#### 8.1.3 State Pattern

**Intent:** Allow an object to alter its behavior and appearance when its internal state changes.

**Application:** Each styled control has three visual states — **rest**, **hover**, and **pressed** — determined by comparing `ctx->hover` and `ctx->focus` against the control's ID. The rendering logic branches based on these states:

```c
int hover = (ctx->hover == id && ctx->focus != id);
int press = (ctx->focus == id);
```

For `ou_btn_ex`, this drives four distinct color calculations per variant. For `ou_input_ex`, it controls border brightness and background color. For `ou_check_ex`, it determines fill color and border intensity.

**Motivation:** Users expect visual feedback confirming that a control is interactive and responding to their actions. The state pattern keeps this logic clean and consistent across all controls.

**Improvement:** Adding a new state (e.g., "disabled") requires adding one branch per control rather than restructuring the entire input system.

**Code location:** Every styled control function (`ou_btn_ex`, `ou_input_ex`, `ou_check_ex`, `ou_slide_ex`, `ou_toggle`).

#### 8.1.4 Template Method Pattern

**Intent:** Define the skeleton of an algorithm, deferring some steps to subclasses (or, in C, to parameterized variations).

**Application:** All controls follow the same algorithmic skeleton:

1. **Identify** — `ou_hash()` to generate a unique ID.
2. **Layout** — `ou_allocate()` to obtain a positioned rectangle.
3. **Sense** — `ou_sense()` to process mouse input and update hover/focus.
4. **Render** — Control-specific drawing using `ou_paint_*()` functions.
5. **Signal** — Return `OU_SIGNAL_COMMIT` or `OU_SIGNAL_ALTERED`.

The `_ex` suffix convention (e.g., `ou_btn_ex`, `ou_slide_ex`) extends this template with additional parameters (`style`, `step`, `fmt`, `opt`) that customize behavior without changing the structure.

**Motivation:** Consistency. Every control behaves predictably in terms of layout participation, input handling, and signal semantics. Developers can create custom controls by following the same 5-step template.

**Improvement:** New controls can be added by implementing only the "Render" step differently while reusing the identity, layout, and input infrastructure unchanged.

**Code location:** Structural pattern visible in every control function in `origami.c`.

#### 8.1.5 Flyweight Pattern

**Intent:** Use sharing to support large numbers of fine-grained objects efficiently.

**Application:** Controls do not have persistent per-instance objects. Instead, the library maintains two LRU cache pools:
- `frame_pool[48]` — maps hash IDs to `ou_frame` structs (for windows/panels needing retained state like scroll position and open/close state).
- `tree_pool[48]` — maps hash IDs to boolean expanded/collapsed state for tree nodes and sections.

Controls that need no retained state (buttons, checkboxes, sliders) share the transient state in `ou_context` (hover, focus, input) without any per-instance allocation.

**Motivation:** An immediate-mode GUI can display hundreds of controls per frame. Allocating persistent objects for each would defeat the library's zero-allocation guarantee. The flyweight pattern allows retained state only where necessary (windows, panels) while keeping stateless controls allocation-free.

**Improvement:** Fixed pool sizes (48 entries each) bound memory usage at compile time. LRU eviction ensures the most recently used containers retain their state. The 48-entry pool supports up to 48 simultaneous windows/panels — sufficient for typical applications.

**Code location:** `ou_cache_alloc()`, `ou_cache_find()`, `ou_cache_touch()`, `resolve_frame()`.

### 8.2 Pattern Interaction

```
Host App --feed_*()--> [Input State] --sense()--> [State Pattern: hover/press]
                                                          |
                                                          v
[Template Method] --> hash -> allocate -> sense -> render -> signal
                                          |
                                          v
                              [Command Pattern: ou_cmd buffer]
                                          |
                              ou_walk() --+--> [Strategy Pattern: renderer callback]

[Flyweight Pattern] <-- frame_pool / tree_pool (retained state only where needed)
```

## 9. Implementation Notes

### 9.1 Control Implementation Pattern

Every styled control in Phase 1 follows this implementation structure (using `ou_check_ex` as an example):

```c
int ou_check_ex(ou_context *ctx, const char *label, int *state, int opt) {
    int res = 0;
    ou_id id = ou_hash(ctx, &state, sizeof(state));   // 1. Identity
    ou_rect r = ou_allocate(ctx);                       // 2. Layout
    ou_sense(ctx, id, r, opt);                          // 3. Input

    // 4. State change
    if (ctx->mouse_pressed == OU_MOUSE_PRIMARY && ctx->focus == id) {
        res |= OU_SIGNAL_ALTERED;
        *state = !*state;
    }

    // 5. Render (state-dependent colors)
    // ... ou_paint_rect, ou_paint_outline, ou_paint_symbol ...

    return res;                                         // 6. Signal
}
```

### 9.2 Button Variant System

`ou_btn_ex()` uses a `style` parameter to select among four visual variants. A `switch` statement on the style determines fill color, border color, and whether fill/border are drawn at all. The `shift_color()` helper adjusts color brightness for hover/press states:

```c
static ou_color shift_color(ou_color c, int delta) {
    c.r = (unsigned char) ou_clamp((int)c.r + delta, 0, 255);
    c.g = (unsigned char) ou_clamp((int)c.g + delta, 0, 255);
    c.b = (unsigned char) ou_clamp((int)c.b + delta, 0, 255);
    return c;
}
```

### 9.3 Text Input Select-All

`ou_input_ex()` supports Ctrl+A through a `text_select` field on `ou_context` that stores the ID of the field with selected text. When selected:
- New text input replaces the entire buffer.
- Backspace clears the entire buffer.
- The selected text is rendered with a highlight rectangle behind it.
- Selection is cleared on mouse click, focus loss, or any edit.

### 9.4 Cursor Feedback

A `cursor_icon` field on `ou_context` is reset to `OU_CURSOR_ARROW` each frame in `ou_frame_begin()`. When `ou_input_ex()` detects the mouse is over its rectangle (via `point_in_rect` and `in_hover_root`), it sets `cursor_icon = OU_CURSOR_TEXT`. The host application reads this value after `ou_frame_end()` and sets the appropriate system cursor.

## 10. User Interface Design

### 10.1 Visual Design Language

The styled controls follow a dark theme aesthetic with the following design principles:

- **Subtle borders at rest** — Borders use `shift_color(FIELD, +18)` at rest (barely visible), brightening to `CONTROL` on hover and `CONTROL_HOT` on focus. This creates a clean, modern appearance where borders don't compete for attention.
- **State-driven brightness** — Controls get brighter on hover, indicating interactivity. On press, they darken or shift, providing physical feedback.
- **1px press offset** — Button content shifts down 1px when pressed, simulating physical press depth.
- **Filled progress track** — The styled slider shows a filled portion from minimum to current value, making the value visually apparent at a glance.

### 10.2 Theme Color Palette

| Index | Name | Default RGBA | Usage |
|---|---|---|---|
| 0 | LABEL | 230, 230, 230, 255 | Text color |
| 1 | EDGE | 25, 25, 25, 255 | Window/panel outlines |
| 2 | CANVAS | 50, 50, 50, 255 | Window background |
| 3 | HEADER | 25, 25, 25, 255 | Title bar background |
| 4 | HEADING | 240, 240, 240, 255 | Title bar text, bright accents |
| 5 | SURFACE | 0, 0, 0, 0 | Panel background (transparent) |
| 6 | CONTROL | 75, 75, 75, 255 | Button/control base |
| 7 | CONTROL_WARM | 95, 95, 95, 255 | Control hover state |
| 8 | CONTROL_HOT | 115, 115, 115, 255 | Control active/focus state |
| 9 | FIELD | 30, 30, 30, 255 | Input field background |
| 10 | FIELD_WARM | 35, 35, 35, 255 | Field hover |
| 11 | FIELD_HOT | 40, 40, 40, 255 | Field active |
| 12 | TRACK | 43, 43, 43, 255 | Slider/scrollbar track |
| 13 | HANDLE | 30, 30, 30, 255 | Scrollbar handle |

All colors are runtime-configurable via `ctx->theme->colors[]`.

## 11. External Interfaces

### 11.1 APIs

**Input API** (host -> library):
| Function | Purpose |
|---|---|
| `ou_feed_motion(ctx, x, y)` | Mouse position update |
| `ou_feed_press(ctx, x, y, btn)` | Mouse button pressed |
| `ou_feed_release(ctx, x, y, btn)` | Mouse button released |
| `ou_feed_scroll(ctx, x, y)` | Scroll wheel delta |
| `ou_feed_keydown(ctx, key)` | Key pressed (modifier/special) |
| `ou_feed_keyup(ctx, key)` | Key released |
| `ou_feed_text(ctx, text)` | Text input (UTF-8 string) |

**Output API** (library -> host):
| Function | Purpose |
|---|---|
| `ou_walk(ctx, &cmd)` | Iterate drawing commands |
| `ctx->cursor_icon` | Recommended system cursor |
| Control return values | `OU_SIGNAL_COMMIT`, `OU_SIGNAL_ALTERED` |

**Drawing Command Types:**
| Type | Data | Renderer Action |
|---|---|---|
| `OU_CMD_FILL` | rect, color | Draw filled rectangle |
| `OU_CMD_GLYPH` | position, color, string | Draw text |
| `OU_CMD_SYMBOL` | rect, color, symbol_id | Draw icon/glyph |
| `OU_CMD_SCISSOR` | rect | Set clipping rectangle |

### 11.2 Third-Party Systems

| System | Role | Integration Point |
|---|---|---|
| SDL2 | Demo renderer, input source | `demo/main.c`, `demo/renderer.c` |
| Host font system | Text measurement | `ctx->text_width`, `ctx->text_height` callbacks |

## 12. Performance Considerations

### 12.1 Performance Requirements

- All per-frame operations shall complete in constant or linear time relative to the number of active controls.
- Target: 60+ FPS with 100+ visible controls on a modern desktop.
- Zero dynamic memory allocation during operation.

### 12.2 Scalability and Optimization Strategies

| Strategy | Implementation |
|---|---|
| **Fixed-size command buffer** | 256 KB pre-allocated buffer avoids allocation overhead. Commands are written sequentially with no pointer chasing. |
| **Scissor culling** | `ou_test_scissor()` returns `OU_CLIP_TOTAL` for fully occluded controls, skipping all drawing commands. `OU_CLIP_PARTIAL` triggers clip-and-draw. |
| **LRU cache eviction** | Frame and tree pools use LRU eviction based on `last_touch` tick, ensuring unused entries are reclaimed without scanning. |
| **O(1) ID lookup** | Hash-based identity avoids string comparisons during interaction. `ou_hash()` uses FNV-1a which is fast and has good distribution. |
| **Depth-sorted skip list** | Root containers are sorted by z-index. The command walk uses `OU_CMD_SKIP` pointers to jump between containers in depth order, avoiding a full buffer scan. |

## 13. Error Handling and Logging

### 13.1 Exception Management

The library uses a `verify()` macro (equivalent to a fatal assertion) for programming errors:

```c
#define verify(x) do {                                                  \
        if (!(x)) {                                                     \
            fprintf(stderr, "Fatal error: %s:%d: assertion '%s' failed\n", \
                __FILE__, __LINE__, #x);                                \
            abort();                                                    \
        }                                                               \
    } while (0)
```

Conditions guarded by `verify()`:
- Stack overflow/underflow on push/pop operations.
- Command buffer overflow in `ou_emit()`.
- Missing text measurement callbacks in `ou_frame_begin()`.
- Unbalanced frame/scissor/scope/grid stacks at `ou_frame_end()`.

These are all **programmer errors** (violated preconditions), not runtime exceptions. The library follows a fail-fast philosophy — an assertion failure means a bug in the calling code.

### 13.2 Logging Mechanisms

The library does not include a logging framework. Errors are reported via `fprintf(stderr, ...)` followed by `abort()`. The host application is responsible for capturing `stderr` if log persistence is needed.

## 14. Design for Testability

| Aspect | How Testability Is Achieved |
|---|---|
| **Deterministic state** | All state lives in `ou_context`. A test can set up a known state, call a control function, and inspect the resulting commands and return value. |
| **Synthetic input** | `ou_feed_*()` functions accept raw coordinates and key codes. Tests can simulate mouse clicks, drags, and keyboard input without a real input device. |
| **Command inspection** | `ou_walk()` yields the exact drawing commands produced by a frame. Tests can verify that a button click produces the expected fill/glyph commands. |
| **No global state** | Multiple independent `ou_context` instances can run in parallel within a single test process. |
| **Signal return values** | Control functions return bitmask signals. Tests can verify `OU_SIGNAL_COMMIT` on a simulated button click and `OU_SIGNAL_ALTERED` on a simulated checkbox toggle. |

**Example test scenario for UC-3 (Checkbox Toggle):**

```c
ou_context ctx;
ou_setup(&ctx);
ctx.text_width = mock_text_width;
ctx.text_height = mock_text_height;

int state = 0;

// Simulate mouse at checkbox position and click
ou_feed_motion(&ctx, 50, 50);
ou_feed_press(&ctx, 50, 50, OU_MOUSE_PRIMARY);

ou_frame_begin(&ctx);
ou_window_begin(&ctx, "Test", ou_rect_make(0, 0, 200, 200));
int sig = ou_check_ex(&ctx, "Test Checkbox", &state, 0);
ou_window_end(&ctx);
ou_frame_end(&ctx);

assert(sig & OU_SIGNAL_ALTERED);  // checkbox was toggled
assert(state == 1);               // state changed from 0 to 1
```

## 15. Deployment and Installation Design

### 15.1 Environment Configuration

| Requirement | Specification |
|---|---|
| **Compiler** | GCC or Clang with C11 support |
| **Flags** | `-Wall -Wextra -pedantic -std=c11` |
| **Platforms** | Windows (MSYS2/MinGW), macOS (Homebrew), Linux (apt/pacman) |
| **SDL2** | Required for demo only; not required by core library |

### 15.2 Packaging and Dependencies

The core library consists of exactly **two files**: `origami.h` and `origami.c`. Integration requires:

1. Copy `origami.h` and `origami.c` into the project.
2. `#include "origami.h"` in the application source.
3. Compile `origami.c` alongside application sources.

No build system, package manager, or configuration step is required for the core library.

**Demo build (Windows):**

```batch
gcc main.c renderer.c ..\src\origami.c ^
    -I..\src -IC:\msys64\mingw64\include\SDL2 ^
    -LC:\msys64\mingw64\lib ^
    -lmingw32 -lSDL2main -lSDL2 -lm ^
    -Wall -Wextra -pedantic -std=c11 -O3 -g ^
    -o origami_demo.exe
```

## 16. Change Log

| Version | Date | Changes |
|---|---|---|
| 1.0 | February 2026 | Initial design document for UC-3 implementation. Phase 1 styled controls: Button (4 variants), Input (with Ctrl+A, cursor feedback, placeholder), Checkbox, Slider (with filled track), Toggle. |

## 17. Future Work / Open Issues

| Item | Priority | Description |
|---|---|---|
| Remaining Phase 1 controls | High | Label, Text, Separator, Switch, Progress, Meter, Avatar not yet styled. |
| Phase 2 controls | High | Radio, Checkbox Group, Toggle Group, Field, Fieldset, Form, Accordion, Collapsible, Tabs, Scroll Area, Toolbar. |
| Phase 3 controls | Medium | Dialog, Alert Dialog, Popover, Tooltip, Context Menu, Select, Preview Card. |
| Text cursor positioning | Medium | Input fields currently append-only; clicking within text to position the cursor is not yet supported. |
| Text selection rendering | Low | Ctrl+A highlights all text, but arbitrary click-drag selection is not implemented. |
| Keyboard navigation | Low | Tab to move focus between controls is not yet implemented. |
| Rounded corners | Low | The renderer only supports rectangles. Rounded corners would require renderer-level support or a new command type. |
| Animation/transitions | Low | Controls snap between states. Smooth transitions would require a time-based interpolation system. |
