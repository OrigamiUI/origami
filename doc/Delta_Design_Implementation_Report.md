# Delta Design and Implementation Report (Assignment 3)

**Project:** Origami UI  
**Version:** A3 closeout delta v1  
**Date:** 2026-04-09

---

## 1. Selected Feedback and Rationale

| Feedback Item | Selection | Rationale |
| --- | --- | --- |
| Add verifiable automated tests per component | Selected | High evidence value and feasible within closeout window |
| Align test report with assignment formatting requirements | Selected | Required for submission compliance |
| Provide reproducible demo smoke build evidence | Selected | Required for source/demo deliverable credibility |
| Keep strict compile quality evidence | Selected | Required by quality criteria |
| Implement complete Tab/Shift+Tab traversal immediately | Deferred | Requires deeper focus-chain behavior changes |
| Implement complete Escape cancellation coverage immediately | Deferred | Requires wider interaction contract refactor |

---

## 2. Effort and Cost Snapshot

Assumption: `1 effort unit = 1 hour`.

| Work Item | Estimated Hours | Actual Hours | Cost Units |
| --- | --- | --- | --- |
| Unity test harness structure and runner | 4.0 | 4.5 | 4.5 |
| Per-component test split and maintenance | 2.0 | 2.0 | 2.0 |
| Integration tests and suite wiring | 1.5 | 1.5 | 1.5 |
| Demo build compatibility fixes | 1.0 | 1.0 | 1.0 |
| Document alignment (report, traceability, quality, matrix) | 3.0 | 3.0 | 3.0 |
| Evidence log preparation and cleanup | 1.0 | 1.0 | 1.0 |
| **Total** | **12.5** | **13.0** | **13.0** |

---

## 3. Implemented Deltas

### 3.1 Test Architecture Delta

- Added and maintained Unity suite with:
  - `29` component-focused tests
  - `3` integration tests
  - centralized test runner and support fixtures
- Added test inventory document: [unity_test_inventory_v1.md](unity_test_inventory_v1.md)

### 3.2 Build and Validation Delta

- Stabilized smoke/demo compile path for current SDL2 include/library environment.
- Added strict compile evidence logging with per-translation-unit GCC checks.
- Documented Clang availability gap as explicit blocked status.

### 3.3 Documentation Delta

- Standardized report format to assignment-required columns.
- Added synchronized Markdown + CSV test report pair.
- Tightened traceability and naming conventions.
- Added submission-oriented task matrix and evidence register.

---

## 4. Design Decision Impact

| Decision | Positive Impact | Tradeoff |
| --- | --- | --- |
| Keep incomplete items visible as `Fail`/`Blocked` | Honest audit trail and clearer next steps | Final status table is not all green |
| Separate scenario-level report from full automated inventory | Better readability for reviewers | Requires one extra supporting document |
| Keep logs as dedicated artifacts under `doc/test_logs/` | Reproducible evidence and reviewer transparency | More files to maintain |

---

## 5. Quality and Test Impact Summary

| Area | Before | After |
| --- | --- | --- |
| Automated evidence depth | Low | High (32 test suite + logs + inventory) |
| Traceability consistency | Partial | Full ID-linked matrix/report alignment |
| Submission-format compliance | Partial | Aligned report and CSV fields |
| Keyboard flow completeness | Incomplete | Still incomplete (tracked explicitly) |

---

## 6. Deferred Follow-up Items

1. Implement and validate Tab/Shift+Tab traversal (`TC-02` closure).
2. Implement and validate Escape cancellation/dismissal behavior (`TC-03` closure).
3. Add Clang execution environment or CI job for `TC-05` closure.
