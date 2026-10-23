# Contributing to AARUNO

- Use feature branches + pull requests; keep `AARUNO.h` API stable.
- Follow Arduino style: `camelCase` methods, `AARUNO_` prefixed macros.
- Validate pins with `aarunoValidateDigitalPin()`; return `AarunoError`.
- Never fake sensor data; use `NaN`/`-1`/`false` + `lastError()`.
- Guard optional deps with `__has_include`; document install steps.
- Add an example + docs entry for every new driver.
- Run `python tests/host/test_aaruno.py` before pushing.
