# Architecture

- `src/AARUNO.h/.cpp` — public singleton `AARUNO`, delegates to modules.
- `src/AARUNOTypes.h` — errors, families, version.
- `src/AARUNOConfig.h` — timeouts, debounce, feature guards.
- `src/AARUNOBoard.h` — compile-time detection + pin validation (never GPIO guessing).
- `src/sensors/` — `AarunoUltrasonic` (multi-instance), `AarunoDHT` (optional dep), `AarunoDigital` (button debounce), analog via singleton.
- `src/actuators/` — LED/PWM/buzzer/relay/motor/servo (servo guarded).
- `src/displays/` — OLED/LCD guarded, `MISSING_DEP` without libs.
- `src/communication/` — ESP-only WiFi, `UNSUPPORTED` elsewhere. No WiFi headers in portable builds.
- `src/utility/AarunoUtil.h` — pure logic (distance, ADC→mV, debounce, error names), host-testable.
- Rules: no heap in core paths (except guarded DHT/Servo objects), no blocking in `update()`, `beep(ms)` documented as blocking helper, headers idempotent.
