# Product Backlog — Origami UI (v1)

**Version:** 1.0  
**Date:** March 2026  
**Source:** Derived from `[Software-Requirements-Document.md](Software-Requirements-Document.md)` and `[Project-Plan-Document.md](Project-Plan-Document.md)`

> Agile note: The backlog is **living**. Only the items planned for the **next sprint** need full acceptance criteria and estimates; others stay at epic/story level until groomed.
> Scope guard: Stories listed under **Post-v1 Parking Lot** are explicitly out of v1.0 scope and are not candidates for sprint selection in this plan.

---

## Legend


| Priority | Meaning                |
| -------- | ---------------------- |
| P0       | Must-have for release  |
| P1       | Should-have            |
| P2       | Nice-to-have / stretch |



| Size | Rough effort      |
| ---- | ----------------- |
| S    | ≤ few days        |
| M    | ~ week slice      |
| L    | multi-week / epic |


---

## Epic A — Core platform & API


| ID  | Story                                                                                                                          | Priority | Size | Notes |
| --- | ------------------------------------------------------------------------------------------------------------------------------ | -------- | ---- | ----- |
| A-1 | As a developer, I can initialize `ou_context`, set text metrics callbacks, and run a frame without crashes.                    | P0       | S    | UC-1  |
| A-2 | As a developer, I receive fatal assertions on misconfiguration (missing callbacks, unbalanced stacks) so failures are obvious. | P0       | S    | NFR   |
| A-3 | As a developer, I can customize `ou_theme` at runtime and see changes next frame.                                              | P0       | M    | UC-4  |


---

## Epic B — Layout & command pipeline


| ID  | Story                                                                                                    | Priority | Size | Notes      |
| --- | -------------------------------------------------------------------------------------------------------- | -------- | ---- | ---------- |
| B-1 | As a developer, I can use row/column layout with nested grids and sane default sizing.                   | P0       | L    | FR-2       |
| B-2 | As a developer, I can consume an ordered `ou_cmd` stream via `ou_walk()` and map it to my renderer.      | P0       | M    | FR-5, UC-2 |
| B-3 | As a developer, fixed-size buffers and no `malloc` in the library are guaranteed for predictable memory. | P0       | M    | NFR        |


---

## Epic C — Input & interaction


| ID  | Story                                                                                          | Priority | Size | Notes |
| --- | ---------------------------------------------------------------------------------------------- | -------- | ---- | ----- |
| C-1 | As an end user, mouse hover/click/drag are routed to the correct control via hit-testing.      | P0       | L    | UC-3  |
| C-2 | As an end user, keyboard input updates focused controls (text fields, shortcuts as defined).   | P0       | M    | UC-3  |
| C-3 | As a developer, controls return `OU_SIGNAL_`* flags so I can react to commit vs. value change. | P0       | M    | UC-3  |


---

## Epic D — Widget library (25+ controls)


| ID  | Story                                                                                                          | Priority | Size | Notes   |
| --- | -------------------------------------------------------------------------------------------------------------- | -------- | ---- | ------- |
| D-1 | Phase 1 set: button, input, checkbox, slider, toggle, label, text, separator, switch, progress, meter, avatar. | P0       | L    | Phase 1 |
| D-2 | Phase 2 set: radio, groups, field, accordion, collapsible, tabs, scroll area, toolbar, …                       | P0       | L    | Phase 2 |
| D-3 | Phase 3 set: dialog, popover, tooltip, context menu, select, preview card, …                                   | P0       | L    | Phase 3 |


---

## Epic E — Demo & integration


| ID  | Story                                                                            | Priority | Size | Notes |
| --- | -------------------------------------------------------------------------------- | -------- | ---- | ----- |
| E-1 | Reference SDL2 demo builds on macOS, Linux, Windows with documented steps.       | P0       | M    | Plan  |
| E-2 | Demo renderer translates fills/glyphs/scissors to pixels (minimal but credible). | P0       | M    | UC-2  |


---

## Epic F — Quality & release


| ID  | Story                                                                           | Priority | Size | Notes   |
| --- | ------------------------------------------------------------------------------- | -------- | ---- | ------- |
| F-1 | CI or script builds with `-Wall -Wextra -pedantic -std=c11` clean on GCC/Clang. | P0       | S    | NFR     |
| F-2 | Manual test pass across controls before v1.0 tag.                               | P0       | M    | Plan    |
| F-3 | API / usage documentation complete for release.                                 | P0       | M    | Phase 4 |


---

## Post-v1 Parking Lot (Explicitly out of scope)

These stories are tracked for future releases and are **excluded from v1.0** per project scope documents.


| ID  | Story                                                    | Priority | Size | Notes                                                |
| --- | -------------------------------------------------------- | -------- | ---- | ---------------------------------------------------- |
| D-4 | Deferred controls: complex menus, toast, autocomplete, … | P2       | L    | Out of v1.0 scope (see Project-Plan "Not Included"). |


---

## Grooming checklist (per sprint)

Before each sprint, the team should:

1. Re-prioritize top of backlog vs. remaining weeks.
2. Break **next** stories into tasks with **acceptance criteria** and owner.
3. Move selected items into **Sprint Backlog** (see `[sprint_plan_v1.md](sprint_plan_v1.md)`).

---

## Version history


| Version | Date       | Notes                                            |
| ------- | ---------- | ------------------------------------------------ |
| 1.0     | March 2026 | Initial backlog from requirements + project plan |


