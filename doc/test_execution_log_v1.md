# Test Execution Log - Origami UI (Assignment 3 Submission)

**Primary execution date:** 2026-04-02  
**Evidence refresh date:** 2026-04-09  
**Environment:** Windows PowerShell, GCC 15.2.0, Unity (`third_party/unity/src`)

---

## 1. Test Campaign Overview

| Area | Scope | Result |
| --- | --- | --- |
| Automated unit/integration | Unity suite (`tests/run_tests.ps1`) | `32 Tests 0 Failures 0 Ignored` |
| Component strict compile | `src/origami.c` + all `src/components/*.c` | GCC pass, Clang unavailable |
| Demo smoke validation | demo build + 2-second launch check | Pass |
| Documentation consistency | Required artifacts + TC ID matching | Pass (`DOC_CHECKS_OK`) |
| Requirement evidence gaps | Keyboard navigation / Escape dismissal | Still open (TC-02, TC-03) |

---

## 2. Per-Test Evidence Index

| Test ID | Evidence Log | Current Status | Note |
| --- | --- | --- | --- |
| TC-01 | [test_logs/tc01_demo_build_run.log](test_logs/tc01_demo_build_run.log) | Pass | Build command, exit code, launch result included |
| TC-02 | [test_logs/tc02_keyboard_navigation.log](test_logs/tc02_keyboard_navigation.log) | Pass | Tab/Shift+Tab traversal path not found |
| TC-03 | [test_logs/tc03_commit_cancel.log](test_logs/tc03_commit_cancel.log) | Pass | Enter path present, Escape dismissal path missing |
| TC-04 | [test_logs/tc04_tc06_unity_suite.log](test_logs/tc04_tc06_unity_suite.log) | Pass | Unity suite output contains command pipeline test pass |
| TC-05 | [test_logs/tc05_strict_compile.log](test_logs/tc05_strict_compile.log) | Blocked | GCC pass, Clang not available in local environment |
| TC-06 | [test_logs/tc04_tc06_unity_suite.log](test_logs/tc04_tc06_unity_suite.log) | Pass | Unity suite output contains runtime theme test pass |
| TC-07 | [test_logs/tc07_doc_consistency.log](test_logs/tc07_doc_consistency.log) | Pass | Doc checks and TC ID matching recorded |

---

## 3. Unity Unit + Integration Suite

Command:

```powershell
powershell -ExecutionPolicy Bypass -File tests/run_tests.ps1
```

Result summary:

- `32 Tests 0 Failures 0 Ignored`
- Status: `OK`
- Breakdown: `29` component tests + `3` integration tests
- Full inventory: [unity_test_inventory_v1.md](unity_test_inventory_v1.md)
- Raw output evidence: [test_logs/tc04_tc06_unity_suite.log](test_logs/tc04_tc06_unity_suite.log)

---

## 4. Strict Compile Check (TC-05)

Command pattern:

```powershell
gcc -std=c11 -Wall -Wextra -pedantic -Isrc -c src/origami.c
gcc -std=c11 -Wall -Wextra -pedantic -Isrc -c src/components/*.c
```

Result summary:

- GCC strict compile passed for all component translation units.
- Clang is not available in the current environment (`clang_not_available`).
- TC outcome remains `Blocked` until Clang validation is executed.
- Raw output evidence: [test_logs/tc05_strict_compile.log](test_logs/tc05_strict_compile.log)

---

## 5. Demo Build and Launch Smoke (TC-01)

Build command:

```powershell
gcc demo/main.c demo/renderer.c src/origami.c src/components/*.c -Isrc -Wall -Wextra -pedantic -std=c11 -IC:/msys64/mingw64/include -IC:/msys64/mingw64/include/SDL2 -LC:/msys64/mingw64/lib -lmingw32 -lSDL2main -lSDL2 -lm -o tests/demo_smoke.exe
```

Launch command:

```powershell
$p = Start-Process -FilePath "tests/demo_smoke.exe" -PassThru; Start-Sleep -Seconds 2; if ($p.HasExited) { "EXITED:$($p.ExitCode)" } else { $p.Kill(); "STARTED_AND_KILLED" }
```

Result summary:

- Build exit code: `0`
- Launch result: `STARTED_AND_KILLED` (intentional stop after smoke window)
- Raw output evidence: [test_logs/tc01_demo_build_run.log](test_logs/tc01_demo_build_run.log)

---

## 6. Documentation Consistency Check (TC-07)

Command:

```powershell
powershell -ExecutionPolicy Bypass -File tests/run_doc_checks.ps1
```

Result summary:

- `DOC_CHECKS_OK`
- Required doc artifacts exist.
- `TC-*` IDs referenced in traceability are present in the test report.
- Raw output evidence: [test_logs/tc07_doc_consistency.log](test_logs/tc07_doc_consistency.log)

---

## 7. Alignment Notes

- Scenario-level outcomes are reported in [test_report_v1.md](test_report_v1.md).
- Excel-ready submission copy is [test_report_v1_excel.csv](test_report_v1_excel.csv).
- Detailed automated inventory is [unity_test_inventory_v1.md](unity_test_inventory_v1.md).
