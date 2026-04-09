# Task Matrix - Project Assignment 3

**Project:** Origami UI  
**Assignment:** Remaining Use Cases Implementation with Design Pattern Integration and Project Completion  
**Submission target:** GitHub repository package

---

## Assignment Scope

This matrix covers contribution ownership for the remaining implemented and validated use-case set used in Assignment 3:

- `UC-2` command pipeline walk/render validation
- `UC-3` keyboard/focus interaction validation
- `UC-4` runtime theme behavior validation

---

## Team Members

| Member | Role Focus |
| --- | --- |
| Ahmet Batuhan Gunal | QA evidence, test report closure, test case ownership |
| Archyn Mikhailov | Architecture/design consistency, traceability and requirement mapping |
| Tuna Komurcu | Build/release checks, demo validation, compile quality evidence |

---

## Workload Distribution

| Work Package ID | Work Package | Requirement Scope | Primary Owner | Supporting Owner(s) | Main Artifact(s) |
| --- | --- | --- | --- | --- | --- |
| WP-01 | Update software design documentation for final implementation scope | UC-2, UC-3, UC-4 | Archyn Mikhailov | Team | `doc/SoftwareDesignDocument.md` |
| WP-02 | Build assignment-compliant test report and Excel export | UC-2, UC-3, UC-4, NFR | Ahmet Batuhan Gunal | Tuna Komurcu | `doc/test_report_v1.md`, `doc/test_report_v1_excel.csv` |
| WP-03 | Run and capture automated Unity suite evidence | UC-2, UC-3, UC-4 | Ahmet Batuhan Gunal | Archyn Mikhailov | `doc/test_logs/tc04_tc06_unity_suite.log`, `doc/unity_test_inventory_v1.md` |
| WP-04 | Execute strict compile and demo smoke checks | NFR, E-1 | Tuna Komurcu | Ahmet Batuhan Gunal | `doc/test_logs/tc01_demo_build_run.log`, `doc/test_logs/tc05_strict_compile.log` |
| WP-05 | Maintain requirement-test traceability and numbering consistency | Cross-cutting | Archyn Mikhailov | Team | `doc/traceability_matrix_v1.md`, `doc/test_execution_log_v1.md` |
| WP-06 | Produce quality-factor matrix and delta implementation closure | Cross-cutting quality | Tuna Komurcu | Archyn Mikhailov | `doc/quality_factor_matrix_v1.md`, `doc/Delta_Design_Implementation_Report.md` |
| WP-07 | Prepare media evidence register and submission checklist | Submission package | Team | Team | `doc/evidence_media_v1.md`, `doc/PA3_Submission_Checklist_v1.md` |

---

## Traceability and Naming Convention Rule

All assignment artifacts follow stable IDs:

- Requirements / use cases: `UC-*`
- Product backlog items: `A-*`, `B-*`, `C-*`, `E-*`, `F-*`
- Sprint tasks: `SB-*`
- Test cases: `TC-*`
- Work packages in this matrix: `WP-*`

These IDs are cross-referenced in:

- `doc/traceability_matrix_v1.md`
- `doc/test_report_v1.md`
- `doc/test_report_v1_excel.csv`
- `doc/test_execution_log_v1.md`

---

## Evidence of Team Contribution

Contribution tracking is provided through:

1. Git commit history with member accounts and commit messages.
2. Artifact ownership in this matrix (primary/supporting owners).
3. Tester and executor names in test report and logs.

---

## Status Snapshot

| Area | Status |
| --- | --- |
| Work package ownership clarity | Complete |
| Requirement-to-test traceability | Complete |
| Assignment-3 matrix format readiness | Complete |
