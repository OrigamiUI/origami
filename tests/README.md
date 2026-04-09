# Unit Tests (Unity)

This project uses [Unity](https://github.com/ThrowTheSwitch/unity) for C unit tests.

## What is covered

- One focused test per component source file under `src/components/`
- Basic stack-balance and smoke behavior checks for widget/container APIs
- Core value-behavior checks for mutable controls (for example slider clamp, textfield input append)
- Integration checks for command pipeline and runtime theme updates

Main suite files:

- `tests/test_runner.c`
- `tests/test_support.c`
- `tests/components/test_*.c`
- `tests/test_integration.c`

## Run on Linux/macOS (bash)

```bash
chmod +x tests/run_tests.sh
tests/run_tests.sh
```

## Run on Windows (PowerShell)

```powershell
./tests/run_tests.ps1
```

## Run documentation checks

```powershell
./tests/run_doc_checks.ps1
```

This validates:

- required test report files (markdown + Excel-ready CSV)
- quality-factor matrix and delta report presence
- evidence checklist presence
- traceability/test ID consistency
- UML source artifacts presence

## Run all checks

```powershell
./tests/run_all_checks.ps1
```

## Notes

- The suite compiles against:
  - `src/origami.c`
  - all `src/components/*.c`
  - `third_party/unity/src/unity.c`
- Compiler used by default: `gcc` (override with `CC` env var).
