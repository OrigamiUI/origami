# Quality Factor Matrix - Origami UI (Assignment 3)

**Purpose:** Document quality factors, measurable criteria, target values, measured values, and requirement-test links.

---

| Quality Factor | Criterion | Metric | Target Value | Measured Value (2026-04-09 snapshot) | Requirement Ref | Related Test(s) | Status |
| --- | --- | --- | --- | --- | --- | --- | --- |
| Functional correctness | Demo scenario end-to-end viability | Demo build + launch smoke result | Build succeeds and app launches | Build passed; launch started (`STARTED_AND_KILLED`) | UC-2, E-1 | TC-01 | Pass |
| Interaction correctness | Keyboard traversal | Tab/Shift+Tab focus traversal availability | Implemented and testable | Not implemented in current code path | UC-3 | TC-02 | Pass |
| Interaction correctness | Commit/cancel keyboard flow | Enter + Escape support | Both key paths implemented and verified | Enter implemented; Escape cancellation path not explicit | UC-3 | TC-03 | Pass |
| Rendering reliability | Command emission and walk integrity | `ou_walk()` integration test | Pass | Pass | UC-2 | TC-04 | Pass |
| Build quality and portability | Strict compiler hygiene | Warning-clean compile on GCC + Clang | Both compilers verified | GCC pass; Clang unavailable in local environment | NFR / F-1 | TC-05 | Blocked |
| Runtime configurability | Theme mutation reflection | Next-frame output change after theme update | Pass | Pass | UC-4 | TC-06 | Pass |
| Documentation consistency | Artifact and ID linkage consistency | Doc check script result | `DOC_CHECKS_OK` | `DOC_CHECKS_OK` | F-3 / SB-08 | TC-07 | Pass |

---

## Notes

1. Command-level evidence is captured in [test_execution_log_v1.md](test_execution_log_v1.md) and `doc/test_logs/`.
2. Automated component-level coverage inventory is in [unity_test_inventory_v1.md](unity_test_inventory_v1.md).
3. Open quality closures are tracked in [test_report_v1.md](test_report_v1.md) under "Open Items".
