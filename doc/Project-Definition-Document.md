# Project Definition Document

## Project Name

**Origami UI**

## Project Summary

Origami UI is a lightweight, portable, immediate-mode GUI library written in ANSI C. It provides a comprehensive set of UI controls for building graphical user interfaces in applications such as tools, editors, games, and embedded systems. The library operates on a command-based rendering model -- it does not draw directly to the screen but instead produces a stream of drawing commands (rectangles, text, icons, clipping regions) that the host application renders using any graphics backend of its choosing (OpenGL, Vulkan, SDL, DirectX, a software rasterizer, etc.).

The project solves the problem of building cross-platform GUIs in C without heavy dependencies. Existing GUI toolkits are often large, complex, and tightly coupled to specific rendering systems or operating systems. Origami UI takes a different approach: a single-header, single-source-file library with fixed memory usage, no dynamic allocation, and no platform-specific code in the core. Developers get a rich set of controls -- from basic buttons and sliders to accordions, dialogs, select dropdowns, and tabbed panels -- while retaining complete control over how and where pixels are drawn.

## Objectives

1. **Implement 25+ immediate-mode UI controls** covering form inputs, overlays, data display, and layout primitives -- all in portable ANSI C.
2. **Keep the library lightweight and self-contained** with no external dependencies, no dynamic memory allocation, and fixed-size internal buffers.
3. **Provide a flexible layout system** supporting row-based layouts, columns, absolute positioning, and nested containers.
4. **Support keyboard and mouse interaction** for interactive controls, including focus management, hover states, and text input.
5. **Deliver a customizable styling system** where colors, spacing, padding, and frame-drawing behavior can be modified at runtime.
6. **Supply a working demo application** with an SDL2 renderer to demonstrate all controls and serve as a reference implementation.

## Scope

### In Scope

- Implementation of 25+ UI controls in C
- A core library consisting of a single C header and a single C source file.
- A row-and-column layout system with support for absolute and relative sizing.
- A runtime-configurable styling system (colors, spacing, fonts, custom frame drawing).
- Mouse input handling (move, click, scroll) and keyboard input handling (keys, text).
- Focus and hover state management with unique ID generation for controls.
- A demo application using SDL2  to showcase all controls.
- API documentation and a usage guide.

### Out of Scope

- Built-in rendering backend -- the library produces drawing commands only; the consumer provides the renderer.
- Platform-specific windowing or OS integration (handled by the host application).
- Rich text rendering, font loading, or text shaping (the library calls user-supplied text_width/text_height callbacks).
- Animations or transition systems.
- Multi-threading support (the library is designed for single-threaded use per context).
- Language bindings for other programming languages (C++, Python, Rust, etc.) -- may be considered in future versions.
- Networking, file I/O, or any functionality beyond GUI control logic.
- High-complexity controls e.g Autocomplete, Combobox, Menu (with sub-menus), Menubar, Navigation Menu, Toast.

## Target Audience

- **C/C++ application developers** building tools, editors, or utilities that need a lightweight embedded GUI.
- **Game developers** who need an in-game UI for debug tools, level editors, settings panels, or HUDs.
- **Embedded systems developers** working in memory-constrained environments where a small, predictable-footprint GUI library is required.
- **Graphics programmers** who already have a rendering pipeline and want a UI library that integrates with it rather than replacing it.
- **Students and hobbyists** learning about GUI systems, immediate-mode architectures, or C library design.

## Key Features

1. **Immediate-Mode Architecture** -- The UI is rebuilt every frame. There is no retained widget tree or persistent state objects to manage. Controls are called as functions, and their results (clicked, changed, submitted) are returned immediately, leading to simple, linear control flow.

2. **25+ Built-In Controls** -- A control set covering common UI needs:
   - **Form controls:** Button, Input, Checkbox, Checkbox Group, Radio, Select, Number Field, Slider, Switch, Toggle, Toggle Group, Field, Fieldset, Form.
   - **Overlay/popup controls:** Dialog, Alert Dialog, Popover, Tooltip, Context Menu, Preview Card.
   - **Navigation/layout controls:** Tabs, Toolbar.
   - **Display controls:** Accordion, Collapsible, Scroll Area, Separator, Meter, Progress, Avatar.

4. **Flexible Layout System** -- A row-and-column layout engine where items can be sized absolutely (pixels), relative to the container edge (negative values), or using defaults. Columns can be nested within rows, and controls can be positioned arbitrarily using explicit rectangles.

5. **Runtime Style Customization** -- Colors, spacing, padding, indentation, title bar height, scrollbar dimensions, and frame-drawing behavior can all be changed at runtime by modifying the style struct or overriding the draw_frame callback.

6. **Custom Control API** -- The library exposes its internal layout, ID generation, input-state, and drawing primitives so developers can implement their own custom controls that integrate seamlessly with built-in ones.

## Deliverables

1. **Origami UI core library** -- A single C header file (`origami_ui.h`) and a single C source file (`origami_ui.c`) containing all 25+ controls, the layout system, input handling, and the command pipeline.
2. **Demo application** -- A complete SDL2  demo that showcases every control and serves as a reference for integrating the library.
3. **API documentation** -- A usage guide covering getting started, the layout system, style customization, and creating custom controls.
4. **Component reference** -- Per-control documentation listing function signatures, parameters, option flags, return values, and usage examples.
5. **Build scripts** -- Cross-platform build scripts for compiling the demo on macOS, Linux, and Windows.

## Project Success Criteria

1. **All 25+ controls are implemented and functional** -- Each control listed in the component specification compiles, renders correctly via the command pipeline, and responds to mouse and keyboard interaction as expected.
2. **The library compiles with zero errors** under `-Wall -Wextra -pedantic` on GCC and Clang with C11 standard, confirming code quality and portability.
3. **The demo application runs on macOS, Linux, and Windows** -- The SDL2  demo builds and runs on all three major platforms, demonstrating all 25+ controls.
4. **Documentation completeness** -- Every public function, struct, enum, and macro is documented with a description, parameter list, and at least one usage example.
