# Traceability Matrix - Origami UI (Assignment 3)

**Purpose:** Maintain explicit links among requirements, backlog items, sprint tasks, and executed tests.

---

## Traceability Matrix

| Requirement / Objective Ref | Product Backlog ID | Sprint Task ID | Test Case ID(s) | Current Status |
| --- | --- | --- | --- | --- |
| UC-2 command pipeline walk/render validity | E-2 | SB-04 | TC-04 | Pass |
| UC-2 demo deliverable build/run closure | E-1 | SB-03 | TC-01 | Pass |
| UC-3 keyboard and focus interaction completeness | C-2 | SB-02 | TC-02, TC-03 | Fail |
| UC-4 runtime theme update behavior | A-3 | SB-01 | TC-06 | Pass |
| NFR strict compile portability (GCC + Clang) | F-1 | SB-05 | TC-05 | Blocked |
| Release readiness manual verification | F-2 | SB-06 | TC-01..TC-07 | Fail |
| Documentation completeness and consistency | F-3 | SB-07 | TC-07 | Pass |
| Cross-cutting ID consistency and mapping health | Cross-cutting closure | SB-08 | TC-07 | Pass |

---

## Naming and Numbering Convention

| Artifact Type | Convention |
| --- | --- |
| Use case / requirement | `UC-*` |
| Product backlog | `A-*`, `B-*`, `C-*`, `E-*`, `F-*` |
| Sprint backlog task | `SB-*` |
| Test case | `TC-*` |

---

## Consistency Rules

1. Every `TC-*` in this matrix must exist in [test_report_v1.md](test_report_v1.md) and [test_report_v1_excel.csv](test_report_v1_excel.csv).
2. `Current Status` values must be one of `Pass`, `Fail`, `Blocked`.
3. Test IDs, requirement IDs, and sprint task IDs must remain stable after submission freeze.
