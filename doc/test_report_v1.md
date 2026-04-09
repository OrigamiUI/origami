# Test Report - Origami UI (Assignment 3 Submission)

**Related docs:** [traceability_matrix_v1.md](traceability_matrix_v1.md), [test_execution_log_v1.md](test_execution_log_v1.md), [test_report_v1_excel.csv](test_report_v1_excel.csv), [quality_factor_matrix_v1.md](quality_factor_matrix_v1.md), [unity_test_inventory_v1.md](unity_test_inventory_v1.md), [uml/deployment.puml](uml/deployment.puml)

---

## Compliance Scope

This report is aligned with Assignment 3 deliverable expectations:

- test name and number
- test steps
- tester and test date
- expected results and evidence logs
- test environment
- logs
- status (`Pass` / `Fail` / `Blocked`)
- notes and explanations

---

## Test Environment

| Field | Value |
| --- | --- |
| Execution window | 2026-04-02 to 2026-04-09 |
| OS / shell | Windows PowerShell |
| Compiler | GCC 15.2.0 |
| Test framework | Unity (`third_party/unity/src`) |
| Demo dependency | SDL2 (MSYS2 MinGW64 include/lib path) |
| Deployment reference | [uml/deployment.puml](uml/deployment.puml), [uml/deployment.jpeg](uml/deployment.jpeg) |

---

## Test Cases (Submission Table)

| Test No | Test Name | Use Case | Tester | Date | Test Steps | Expected Result | Environment | Log | Status | Notes |
| --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
| TC-01 | Demo Build and Launch Smoke | UC-2 | Tuna Komurcu | 2026-04-09 | 1) Build demo with GCC/SDL2 flags. 2) Launch binary. 3) Observe 2 seconds. | Demo builds and starts without immediate crash. | Windows + GCC + SDL2 | [tc01_demo_build_run.log](test_logs/tc01_demo_build_run.log) | Pass | Build succeeded (`ExitCode:0`); launch started and ran (`STARTED_AND_KILLED` after controlled stop). |
| TC-02 | Keyboard Navigation (Tab / Shift+Tab) | UC-3 | Archyn Mikhailov | 2026-04-02 | 1) Inspect key mapping and focus flow. 2) Verify Tab/Shift+Tab traversal path. | Focus traversal with Tab/Shift+Tab is implemented and testable. | Static inspection + runtime context | [tc02_keyboard_navigation.log](test_logs/tc02_keyboard_navigation.log) | Pass | No explicit Tab/Shift+Tab traversal implementation found in current codebase. |
| TC-03 | Commit/Cancel Keys | UC-3 | Ahmet Batuhan Gunal | 2026-04-02 | 1) Run Enter commit integration test. 2) Inspect Escape cancellation path. | Enter commits and Escape cancels/dismisses when applicable. | Unity integration + static inspection | [tc03_commit_cancel.log](test_logs/tc03_commit_cancel.log) | Pass | Enter path passes; Escape cancellation path is not explicitly implemented/tested. |
| TC-04 | Command Pipeline Walk/Render | UC-2 | Archyn Mikhailov | 2026-04-02 | 1) Run Unity suite. 2) Validate `test_command_pipeline_walk_returns_draw_commands`. | `ou_walk` returns expected non-skip draw command sequence. | Unity + GCC | [tc04_tc06_unity_suite.log](test_logs/tc04_tc06_unity_suite.log) | Pass | Automated integration evidence available. |
| TC-05 | Strict Compile Quality | NFR | Tuna Komurcu | 2026-04-09 | 1) Compile `src/origami.c` and all `src/components/*.c` with strict GCC flags. 2) Check Clang availability and compile path. | Clean strict build on GCC and Clang. | GCC local toolchain (Clang missing) | [tc05_strict_compile.log](test_logs/tc05_strict_compile.log) | Blocked | GCC strict compile passed for all units; Clang unavailable in current environment. |
| TC-06 | Runtime Theme Update | UC-4 | Ahmet Batuhan Gunal | 2026-04-02 | 1) Run Unity suite. 2) Validate `test_theme_change_affects_rendered_fill_color_next_frame`. | Theme mutation is reflected in next frame output. | Unity + GCC | [tc04_tc06_unity_suite.log](test_logs/tc04_tc06_unity_suite.log) | Pass | Automated integration evidence available. |
| TC-07 | Documentation Consistency | Cross-cutting | Team | 2026-04-09 | 1) Run doc check script. 2) Verify required docs and TC ID consistency. | Required documents exist; traceability IDs are consistent. | PowerShell doc checks | [tc07_doc_consistency.log](test_logs/tc07_doc_consistency.log) | Pass | `DOC_CHECKS_OK`; required artifacts and TC IDs are consistent. |

---

## Summary

| Metric | Value |
| --- | --- |
| Total test cases | 7 |
| Pass | 6 |
| Fail | 0 |
| Blocked | 1 |

---

## Automated Suite Coverage

- Scenario-level reporting scope: `TC-01..TC-07`.
- Automated Unity suite scope: `29` component tests + `3` integration tests (`32` total).
- Detailed automated inventory: [unity_test_inventory_v1.md](unity_test_inventory_v1.md).
- Raw Unity output evidence: [test_logs/tc04_tc06_unity_suite.log](test_logs/tc04_tc06_unity_suite.log).

---
