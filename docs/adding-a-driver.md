# Adding a Driver

1. Create `src/sensors|Acutators/AarunoX.h/.cpp` with a self-contained class (no globals).
2. Validate pins via `aarunoValidateDigitalPin()`; return `AarunoError`, never fake data.
3. Guard third-party includes with `#if __has_include(<Lib.h>)`; return `AARUNO_MISSING_DEP` when absent.
4. Guard board-specific code with `AARUNO_BOARD_*` macros; return `AARUNO_UNSUPPORTED` otherwise.
5. Put pure math in `src/utility/AarunoUtil.h` + extend `tests/host/test_aaruno.py`.
6. Add a singleton wrapper in `AARUNO.h/.cpp` only if single-instance makes sense; otherwise document the class.
7. Add an example under `examples/` using real API + valid per-board pins, and document in `docs/api-reference.md` + compatibility matrix.
