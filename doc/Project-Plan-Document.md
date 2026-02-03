# Project Plan Document

## Project Scope

### Included

- Design and implementation of 25+ immediate-mode UI controls in portable ANSI C.
- A command-based rendering pipeline producing abstract drawing commands (Rect, Text, Icon, Clip).
- A row-and-column layout system with absolute, relative, and default sizing modes.
- Mouse and keyboard input handling with focus/hover state management.
- A runtime-configurable styling system (colors, spacing, padding, fonts, custom frame drawing).
- A custom control API exposing internal primitives for user-defined controls.
- A demo application using SDL2  rendering as a reference implementation.
- Cross-platform build scripts for macOS, Linux, and Windows.
- API documentation, usage guide, and per-component reference.

### Not Included

- A built-in rendering backend (the library produces commands; the consumer renders them).
- Platform-specific windowing, OS integration, or system dialog wrappers.
- Font loading, rich text rendering, or text shaping.
- Animation or transition frameworks.
- Multi-threading support or thread-safe context access.
- Language bindings for C++, Python, Rust, or other languages.
- Package manager distribution
- High-complexity controls deferred e.g. Autocomplete, Combobox, Menu (with sub-menus), Menubar, Navigation Menu, and Toast. These require infrastructure (text filtering engines, sub-menu trees, notification queues, timer systems) beyond v1.0 scope.

---

## Project Organization -- People (Roles and Responsibilities)

| Role                             | Responsibility                                                                        |
| -------------------------------- | ------------------------------------------------------------------------------------- |
| **Project Lead / Architect**     | Overall design decisions, API design, code review.                                    |
| **Core Developers**              | Implementation of controls, layout system, input handling, command pipeline.          |
| **Demo / Integration Developer** | SDL2  demo application, renderer implementation, cross-platform build testing.        |
| **Documentation Author**         | Usage guide, API reference, component docs, README, and project documents.            |
| **QA / Tester**                  | Manual testing of all controls, cross-platform build verification, edge-case testing. |

> Note: In a solo or small-team setting, one person may hold multiple roles.

---

## Objectives

1. Deliver a fully functional C library containing all 25+ UI controls by the end of the project timeline.
2. Maintain code quality by compiling with strict warnings (`-Wall -Wextra -pedantic`) and zero errors throughout development.
3. Ensure cross-platform compatibility by testing builds and the demo application on macOS, Linux, and Windows at every milestone.
4. Produce  documentation alongside the code 
5. Keep the core library self-contained with zero external dependencies

---

## Key Phases and Timeline

| Phase                                         | Description                                                                                                                                                                                                                 | Deliverables                                                                                               | Start  | End    |
| --------------------------------------------- | --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- | ---------------------------------------------------------------------------------------------------------- | ------ | ------ |
| **Phase 1: Foundation & Basic Controls**      | Core architecture (context, command pipeline, layout system, input handling, styling, rendering loop) and basic controls: Button, Input, Checkbox, Slider, Label, Text, Separator, Toggle, Switch, Progress, Meter, Avatar. | Core library skeleton, layout engine, input system, demo scaffold with SDL2 renderer, 12 working controls. | Week 1 | Week 2 |
| **Phase 2: Form, Group & Container Controls** | Form and group controls (Radio, Checkbox Group, Toggle Group, Number Field, Field, Fieldset, Form) and container/layout controls (Accordion, Collapsible, Tabs, Scroll Area, Toolbar).                                      | 12 additional controls (24 total).                                                                         | Week 3 | Week 4 |
| **Phase 3: Overlay & Popup Controls**         | Overlay controls requiring anchor positioning and modal infrastructure: Dialog, Alert Dialog, Popover, Tooltip, Context Menu, Select, Preview Card.                                                                         | 5 additional controls (29 total, feature-complete).                                                        | Week 5 | Week 6 |
| **Phase 4: Documentation, Testing & Release** | API reference, usage guide, per-component docs. Cross-platform build testing. End-to-end control testing, edge-case fixes, final code review, release packaging.                                                            | documentation, demo, build scripts, v1.0 release, tagged source, release notes.                            | Week 7 | Week 7 |

> Note: Documentation is written incrementally during Phases 1--3 (each control is documented as it is implemented). Phase 4 is for final review, gap-filling, and release packaging.

**Project Closure Date:** End of Week 7

**Project Evaluation Date:** Week 8 (post-release retrospective and feedback collection)

### Deliverable-to-Phase Mapping

| Deliverable | Phase |
|---|---|
| Core library skeleton (header + source) | Phase 1 |
| Demo application scaffold | Phase 1 |
| 12 basic controls | Phase 1 |
| 12 form/group/container controls | Phase 2 |
| 5 overlay/popup controls | Phase 3 |
| API documentation and usage guide | Phases 1--3 (incremental), Phase 4 (finalized) |
| v1.0 release package | Phase 4 |

---

## Resource Planning

### Software Tools

| Resource                        | Purpose                                                                                                       |
| ------------------------------- | ------------------------------------------------------------------------------------------------------------- |
| **GCC / Clang**                 | C compiler (C11 standard) for building the library and demo.                                                  |
| **SDL2**                        | Cross-platform windowing and input for the demo application (demo dependency only, not a library dependency). |
| **Make / Shell scripts**        | Build system for compiling the library and demo across platforms.                                             |
| **Git**                         | Version control.                                                                                              |
| **GitHub **                     | Repository hosting, issue tracking, and release management.                                                   |

### Access
- All tools are free and open-source or freely available.
- SDL2 is available via its official website.
- No paid licenses, cloud services, or databases are required.

---

## Risk Management

| Risk                                                                                                                         | Likelihood | Impact | Mitigation                                                                                                                                                                                            |
| ---------------------------------------------------------------------------------------------------------------------------- | ---------- | ------ | ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| **Compressed timeline causes quality shortcuts.**                                                                            | High       | High   | Prioritize controls by dependency order. Write tests and docs incrementally, not in a final rush. Cut scope (defer medium controls to v1.1) before cutting quality.                                   |
| **Overlay/popup controls** (Dialog, Select, Popover) require anchor-positioning infrastructure that is harder than expected. | Medium     | High   | Prototype anchor positioning at the end of Phase 2 so Phase 3 starts with a working foundation. Keep initial implementations simple (no edge-flipping).                                               |
| **Cross-platform build failures** on Windows due to compiler or SDL2 differences.                                            | Medium     | Medium | Test on Windows at least once during Phase 2 (not just at the end). Stick to ANSI C to avoid platform-specific code.                                                                                  |
| **Fixed-size buffer overflows** when controls produce more commands or state entries than allocated.                         | Low        | High   | Define generous default buffer sizes. Add runtime overflow checks with clear error reporting. Document limits.                                                                                        |
| **Team member unavailability** (illness, schedule conflict).                                                                 | Low        | High   | With only 7 weeks, even a few lost days are significant. Maintain clear documentation so any developer can pick up where another left off. Keep modular code with minimal inter-control dependencies. |

---

## Communication Plan

- **Weekly sync meetings** (or async written updates if remote/solo): Review progress against the phase timeline, discuss blockers, plan the coming week.
- **Chat channel** (WhatsApp): Day-to-day communication for quick questions and coordination.

---

## Change Management Plan
1. **Proposal**: If a team member realizes a requirement needs to change (e.g., a feature is too complex or unnecessary), they will raise the issue immediately in the WhatsApp group.
2. **Decision Making**: The team will discuss the proposed change via chat or arrange a quick ad-hoc meeting to reach a consensus.
3. **Implementation**: Once a change is agreed upon, the team will simply update the current task list and the member responsible for that specific module will adjust their code accordingly.

---

## Budget Plan (Effort and Resource Allocation)

| Phase | Effort Allocation | Notes |
|---|---|---|
| Phase 1: Foundation & Basic Controls | 30% | Critical path -- architecture plus 12 basic controls. High throughput; establishes patterns for all later controls. |
| Phase 2: Form, Group & Container Controls | 30% | 12 controls of moderate complexity. Includes anchor-positioning groundwork for Phase 3. |
| Phase 3: Overlay & Popup Controls | 25% | Highest per-control complexity in v1.0 -- modal overlays, anchor positioning, click-outside dismissal. |
| Phase 4: Documentation, Testing & Release | 15% | Final doc review, cross-platform testing, edge-case fixes, release packaging. Docs are written incrementally in Phases 1--3. |

---

## Acceptance Tests and Acceptance Criteria

### Per-Control Acceptance Criteria

Each of the 25+ controls must satisfy:

1. **Renders correctly** -- The control produces valid drawing commands and appears correctly in the demo application.
2. **Responds to mouse input** -- Hover, click, drag, and scroll interactions work as expected for the control type.
3. **Responds to keyboard input** -- Where applicable, the control handles focus, Tab navigation, Enter/Space activation, arrow key movement, and Escape dismissal.
4. **Respects the layout system** -- The control sizes and positions itself correctly within rows, columns, and nested containers, including absolute and relative width modes.
5. **Respects styling** -- The control uses colors and spacing from the active style struct and responds to runtime style changes.
6. **Compiles without errors** -- The control code compiles with `-Wall -Wextra -pedantic -std=c11` on both GCC and Clang with zero errors.

### System-Level Acceptance Tests

| Test                            | Criteria                                                                                                                                                   |
| ------------------------------- | ---------------------------------------------------------------------------------------------------------------------------------------------------------- |
| **Full demo test**              | The demo application launches, displays all 25+ controls, and all interactions work correctly on macOS, Linux, and Windows.                                |
| **Strict compilation test**     | `gcc -Wall -Wextra -pedantic -std=c11` and `clang -Wall -Wextra -pedantic -std=c11` both compile the library with zero errors.                             |
| **Documentation coverage test** | Every public function, struct, enum, and macro in the header file has a corresponding entry in the API reference document.                                 |
| **Style override test**         | Modifying all style colors and spacing values at runtime visibly changes the appearance of all controls in the demo without crashes or rendering glitches. |
| **Custom control test**         | A sample custom control (not one of the 25) is implemented using the public API and works correctly alongside built-in controls.                           |
