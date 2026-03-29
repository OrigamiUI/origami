# Sprint Backlog - Origami UI (single sprint)

**Source:** [sprint_plan_v1.md](sprint_plan_v1.md), [product_backlog_v1.md](product_backlog_v1.md)

---

## Workflow Rule

Every backlog item must be executed in this order:

`Requirement -> Design -> Implementation -> Test`

---

## Sprint Backlog Items


| Task ID | Product Backlog ID    | Requirement Reference      | Design Reference                            | Implementation Focus                                        | Test Focus                           | Status  |
| ------- | --------------------- | -------------------------- | ------------------------------------------- | ----------------------------------------------------------- | ------------------------------------ | ------- |
| SB-01   | A-3                   | UC-4                       | architecture/theme section, SDD theme notes | Theme parameter review and docs cleanup                     | Runtime theme change sanity check    | Planned |
| SB-02   | C-2                   | UC-3                       | SDD keyboard/focus flow                     | Keyboard focus chain verification and fixes                 | Tab/Shift+Tab/Enter/Escape scenarios | Planned |
| SB-03   | E-1                   | Plan demo deliverable      | demo docs and build notes                   | Demo build command validation                               | Build/run checklist execution        | Planned |
| SB-04   | E-2                   | UC-2                       | architecture command pipeline               | Command-to-render mapping verification                      | Visual regression checks             | Planned |
| SB-05   | F-1                   | NFR compile quality        | compile flags docs                          | Strict warning cleanup (`-Wall -Wextra -pedantic -std=c11`) | GCC/Clang build checks               | Planned |
| SB-06   | F-2                   | Plan release quality       | test report template                        | Manual test execution across key controls                   | Full manual pass logging             | Planned |
| SB-07   | F-3                   | Documentation completeness | all `doc/` references                       | API and usage docs gap closure                              | Peer review/read-through             | Planned |
| SB-08   | Cross-cutting closure | Requirement set overall    | traceability matrix                         | Requirement/story/task/test mapping consistency             | Traceability consistency check       | Planned |


---

## Definition of Done

An item is `Done` only if:

1. Requirement/design/implementation/test columns are all addressed.
2. Relevant test results are written in [test_report_v1.md](test_report_v1.md).
