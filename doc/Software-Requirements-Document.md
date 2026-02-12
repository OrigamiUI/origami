# Software Requirements Document

## Product Perspective

Origami UI is a stand-alone, lightweight, immediate-mode GUI library to be written in portable ANSI C. It shall function as an embeddable component that application developers integrate into their own projects -- tools, editors, games, embedded systems -- alongside whatever rendering backend they already use (OpenGL, Vulkan, SDL2, DirectX, a software rasterizer, etc.).

The library shall not draw to the screen directly. It shall produce a stream of abstract drawing commands (filled rectangles, text glyphs, icons, clipping regions) that the host application consumes and renders through its own graphics pipeline. A companion SDL2 demo application shall be delivered as a reference implementation.

## Product Functions

1. **UI Control Rendering** -- The library shall provide 25+ immediate-mode control functions (buttons, sliders, text fields, checkboxes, dialogs, tabs, etc.) that produce drawing commands and return interaction results each frame.
2. **Layout Management** -- A row-and-column layout engine shall automatically position and size controls, supporting absolute, relative, and default sizing modes with nested columns.
3. **Input Processing** -- The library shall accept mouse and keyboard events fed by the host application and route them to controls based on hover and focus state.
4. **Focus and Hover Management** -- The library shall track hover and focus state using a hash-based unique ID system, managing focus transfer on click and dismissal on Escape.
5. **Drawing Command Generation** -- The library shall produce an ordered command stream (fill, glyph, symbol, scissor) into a fixed-size buffer that the host application walks to render the UI.
6. **Runtime Style Customization** -- A theme struct shall expose colors, spacing, padding, and a draw_frame callback, all modifiable at runtime.
7. **Windowed Containers** -- The library shall provide draggable, resizable, closable window containers with depth-sorted rendering and automatic popup dismissal.
8. **Custom Control API** -- The library shall expose layout, ID, input, and drawing primitives so developers can build their own controls.

## User Characteristics

- **C/C++ application developers** -- Intermediate to advanced programmers comfortable integrating C libraries and managing build systems.
- **Game developers** -- Experienced programmers needing in-game UI for debug tools, editors, or settings panels.
- **Embedded systems developers** -- Engineers in memory-constrained environments requiring predictable memory usage and small footprint.
- **Graphics programmers** -- Developers proficient with GPU APIs who want a UI library that produces draw calls rather than owning a window.
- **Students and hobbyists** -- Learners studying GUI systems, immediate-mode architectures, or C library design.

## Constraints

1. **Language** -- ANSI C (C11 standard). No C++ features or compiler-specific extensions.
2. **No External Dependencies** -- The core library (`origami.h` and `origami.c`) shall depend only on the C standard library.
3. **No Dynamic Allocation** -- All memory shall be statically allocated in fixed-size buffers within the context struct.
4. **Single-Threaded** -- One thread per context; no internal locking.
5. **No Built-In Renderer** -- The host application must supply a renderer and text-measurement callbacks.
6. **Platform** -- The core shall be platform-independent. The demo shall use SDL2. Build scripts shall target macOS, Linux, and Windows.
7. **Compilation** -- Zero errors under `-Wall -Wextra -pedantic -std=c11` on GCC and Clang.
8. **Timeline** -- 7-week development schedule across four phases, with a Week 8 post-release evaluation.

## System Features (Use Case Based)

The system has two actors: the **Application Developer** who integrates and calls the library, and the **End User** who interacts with the rendered UI through mouse and keyboard.

### Use Case 1: Initialize the Library

**Actor:** Application Developer

**Main Flow:** The developer allocates an `ou_context`, calls `ou_setup()`, assigns the required `text_width` and `text_height` callbacks, optionally customizes theme values, and enters the main loop.

**Exceptions:** If callbacks are not assigned before the first frame, `ou_frame_begin()` triggers a fatal assertion.

### Use Case 2: Build and Render a UI Frame

**Actor:** Application Developer

**Main Flow:** Each frame, the developer calls `ou_frame_begin()`, invokes control and layout functions to describe the UI, calls `ou_frame_end()`, then walks the command buffer with `ou_walk()` and translates each command (fill, glyph, symbol, scissor) into rendering API calls.

**Exceptions:** Unbalanced stacks at `ou_frame_end()` or command buffer overflow trigger fatal assertions.

### Use Case 3: Interact with Controls

**Actor:** End User

**Main Flow:** The user moves the mouse, clicks, types, drags sliders, and scrolls. The host application feeds these events to the library via `ou_feed_*()` functions. Control functions return signals (`OU_SIGNAL_COMMIT` on button click or Enter, `OU_SIGNAL_ALTERED` on value change) that the developer acts on.

**Exceptions:** Clicking outside a popup dismisses it. Clicking a non-interactive area clears focus.

### Use Case 4: Customize Theme and Create Custom Controls

**Actor:** Application Developer

**Main Flow:** The developer modifies `ctx->theme` colors and spacing values at runtime; all controls reflect changes on the next frame. To create a custom control, the developer calls `ou_hash()` for an ID, `ou_allocate()` for layout space, `ou_sense()` for input tracking, and drawing primitives to emit commands.

**Exceptions:** Omitting `ou_sense()` makes the control non-interactive. Omitting `ou_allocate()` causes layout overlap.

## Non-Functional Requirements

### Usability

- The API shall use a consistent `ou_` prefix, `_ex` suffix for extended variants, and convenience macros for common defaults.
- Each control shall be a single function call with an immediate return value -- no widget trees, listeners, or callbacks to manage.
- The library shall consist of exactly two files (`origami.h` and `origami.c`) for trivial integration.

### Performance

- All per-frame operations shall run in constant or linear time relative to active controls, targeting 60+ FPS.
- No dynamic allocation shall occur during operation.
- Fully occluded elements shall be scissor-culled and produce no drawing commands.

### Portability

- The core shall be written in ANSI C (C11) with no platform-specific code or compiler extensions.
- The library shall compile and run on macOS, Linux, and Windows with both GCC and Clang.

### Reliability

- All stack operations shall be guarded by assertions with clear error messages on overflow or underflow.
- The command buffer shall assert on overflow, preventing out-of-bounds writes.
- Focus shall be automatically cleared when a focused control is not touched during the current frame.

## External Interface Requirements

### User Interfaces

The library shall not own any screens. The demo application shall include three representative windows: a **Demo Window** (buttons, tree nodes, checkboxes, sliders, text), a **Log Window** (scrollable log panel, text input, submit button), and a **Style Editor Window** (per-channel color sliders for all 14 theme colors). All windows shall be draggable, resizable, closable, and depth-sorted.

### Software Interfaces

1. **Text Measurement Callbacks** -- The host application must assign `ctx->text_width` and `ctx->text_height` functions that measure string dimensions using the application's font system.
2. **SDL2 (Demo Only)** -- The demo shall use SDL2 for windowing, rendering, and input. SDL2 shall not be required by the core library.
3. **Custom Frame Drawing Callback** -- The host may override `ctx->draw_frame` to customize how control backgrounds and outlines are drawn.
