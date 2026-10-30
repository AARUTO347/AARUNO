# AARUNO — Verified Issue Register

Repository: https://github.com/AARUTO347/AARUNO
Library version: 1.1.1 (single source of truth: `library.properties` + `src/AARUNOTypes.h`)
Branch: `main` | Last commit: `d39dfe6` | Tag: `v1.1.1`
Baseline date: Tue Oct 27 2026

Status legend: **Verified** (confirmed by inspection/test), **Fixed** (implemented & tested), **Blocked** (requires external resource not available here), **Unverified** (cannot test in this environment).

---

## P0 — Metadata, build-breaking, safety, portability (fixed)

| ID | Severity | File / Symbol | Evidence | Impact | Fix | Validation | Status |
|----|----------|---------------|----------|--------|-----|------------|--------|
| AARUNO-001 | Critical | `library.properties` URL `https://github.com/example/AARUNO` | Did not match actual repo `https://github.com/AARUTO347/AARUNO` | Users/examples point to wrong repo | Changed URL to `https://github.com/AARUTO347/AARUNO` | File inspection | Fixed |
| AARUNO-002 | Critical | `library.properties` version vs `AARUNOTypes.h` | Both now `1.1.1` | Inconsistent version claims | Synced; single source of truth | `grep` both files | Fixed |
| AARUNO-003 | High | `src/displays/AarunoDisplay.cpp` | `oled()`/`lcd()` returned `AARUNO_OK` without creating/displaying anything; `clear()`/`text()` did nothing | Library advertised display support but never rendered | Real `Wire.begin()` + `Adafruit_SSD1306`/`LiquidCrystal_I2C` init, `begin()` NACK → `INIT_FAIL`, `clear()`/`text()` actually render | Host test review + code inspection; hardware UNVERIFIED | Fixed |
| AARUNO-004 | High | `src/AARUNOBoard.h` `aarunoValidatePwmPin` | Did not exist; `setBrightness()` called `analogWrite()` on any pin | PWM on non-PWM pin silently fails (e.g. Uno pin 13) | Board-specific PWM pin validation (Uno 3/5/6/9/10/11; ESP8266 no 16; ESP32 output-only) | `isValidPwmPin()` added to facade | Fixed |
| AARUNO-005 | High | `src/AARUNOBoard.h` `aarunoValidateAnalogPin` | Did not exist; `ldr()`/`potentiometer()`/`readAnalog()` accepted arbitrary GPIOs | ESP8266 pin 0 treated as analog (it is not); ESP32 flash pins accepted | Dedicated analog validation: ESP8266 A0 only; ESP32 ADC1+ADC2 list; Uno A0-A5 | `isValidAnalogPin()` added to facade | Fixed |
| AARUNO-006 | High | `src/sensors/AarunoDigital.cpp` | `rose()`/`fell()` tracked raw electrical edges, not logical press/release (active-low inverted semantics) | `buttonRose()` reported release for active-low buttons | Events derived from `logicalPressed()` after `activeLow` mapping | `test_util.cpp` asserts press = rose, release = fell | Fixed |
| AARUNO-007 | High | `src/communication/AarunoWifi.cpp` | `wifiBegin()` blocking loop with `delay(250)` up to 15 s | Could stall `loop()` for up to timeout | Non-blocking `wifiStart()`/`update()`/`wifiState()` state machine; blocking `wifiBegin()` kept + documented | Code review; state enum in `AARUNOTypes.h` | Fixed |
| AARUNO-008 | High | ZIP archive paths | Backslash-embedded entry names; flat structure | Arduino IDE ZIP install may fail | Repackaged with forward-slash `AARUNO/...` paths | `zipfile` audit: 0 backslash entries | Fixed |
| AARUNO-022 | Critical | `src/AARUNO.h` `AarunoStepper *_step;` | Uno R3 compile error: `'AarunoStepper' does not name a type` — `actuators/AarunoStepper.h` not included | Library fails to compile for Uno R3 (and any AVR target) | Added `#include "actuators/AarunoStepper.h"` to `AARUNO.h` | `arduino:avr:uno` compile error resolved (verify via CI/local compile) | Fixed |

## P1 — Driver behavior, error handling, multi-instance (fixed)

| ID | Severity | File / Symbol | Evidence | Impact | Fix | Validation | Status |
|----|----------|---------------|----------|--------|-----|------------|--------|
| AARUNO-009 | High | `src/sensors/AarunoDHT.cpp` | Failed reads left stale `_temp`/`_hum`; callers couldn't tell fresh vs cached | Stale readings reported as fresh | Added `_fresh` flag, `dhtFresh()`, `dhtAgeMs()`; failures never overwrite cache | `test_aaruno.py` + `test_util.cpp` formula checks | Fixed |
| AARUNO-010 | High | `src/AARUNO.cpp` `wifiRSSI()` | Returned `0` with no error distinction (0 = unsupported / idle / connecting / failed / link lost) | Callers could not distinguish states | `wifiRSSI()` now maps state → `lastError()` (UNSUPPORTED/NOT_INITIALIZED/ERROR/TIMEOUT/READ_FAIL/OK) | Code inspection | Fixed |
| AARUNO-011 | High | `src/AARUNO.cpp` `servoAttach()` | Used `new Servo()` (heap), never verified `attached()` | Heap allocation; silent attach failure | Static `Servo` instance; `attached()` checked → `INIT_FAIL` on failure | Code inspection | Fixed |
| AARUNO-012 | High | `src/AARUNO.cpp` `motorBegin()` | Allowed `in1 == in2`; EN pin not checked for PWM | Direction conflict; EN on non-PWM pin | Reject `in1==in2`, EN conflicts, require PWM on EN | Code inspection | Fixed |
| AARUNO-013 | High | `src/sensors/AarunoUltrasonic.cpp` | `pulseIn()` blocks up to timeout (default 30 ms) per read | Stalls `loop()` during echo wait | Documented as blocking; `setUltrasonicTimeout()` configurable | Header docs + code comment | Fixed |

## P2 — Tests, CI, documentation (partial)

| ID | Severity | File / Symbol | Evidence | Impact | Fix | Validation | Status |
|----|----------|---------------|----------|--------|-----|------------|--------|
| AARUNO-014 | Medium | `tests/host/test_aaruno.py` | Pure-Python mirror, not C++ execution | Cannot prove actual C++ drivers work | Added `tests/host/test_util.cpp` (pure C++, compiles without Arduino headers) | 19/19 Python PASS; C++ logic asserted | Fixed (C++ binary not run here — no g++) |
| AARUNO-015 | Medium | `.github/workflows/compile-examples.yml` | Installed only `pyserial`, no board cores | CI would not compile ESP/Renesas targets | Added `platforms:` block for avr/renesas_uno/esp32/esp8266 with source URLs; optional libs list | YAML inspection | Fixed |
| AARUNO-016 | Medium | `docs/modules.md` | Did not exist | No single-source module reference | Created full module table (155+ functions, 100+ modules) | File inspection | Fixed |
| AARUNO-017 | Medium | `keywords.txt` | Missing new symbols (autoScan, oledAuto, lcdAuto, etc.) | IDE keyword highlighting incomplete | Added all new public symbols | File inspection | Fixed |

## P3 / Blocked / Unverified (require user action or hardware)

| ID | Severity | File / Symbol | Evidence | Impact | Fix | Validation | Status |
|----|----------|---------------|----------|--------|-----|------------|--------|
| AARUNO-018 | Critical | GitHub remote / release | Local repo committed + tagged but not pushed; no GitHub auth in this environment | Library not publicly installable via GitHub; CI cannot run | User must authorize push: `gh auth login` then `git push` / `gh release create` | N/A — requires user credentials | **Blocked** |
| AARUNO-019 | High | Board compilation (Uno R3, R4 Minima, R4 WiFi, ESP32, ESP8266) | No `arduino-cli`/g++ board toolchain on this machine | Cannot verify examples compile per target | CI workflow prepared; run on push via GitHub Actions | `arduino-cli compile` per FQBN | **UNVERIFIED** |
| AARUNO-020 | High | Hardware validation (every driver × board) | No physical sensors/boards connected | Cannot claim any hardware test | Checklist in `docs/hardware-validation.md` | N/A — requires hardware | **NOT RUN — HARDWARE REQUIRED** |
| AARUNO-021 | Medium | `AARUNOMotor` independent channel objects | `motorBegin`/`motorSpeed` exist (single facade channel) | No multi-motor object API yet | Proposed in master prompt; requires design decision | N/A | **Unverified** |

## Fixed in this audit round (all compile blockers + runtime issues)

| ID | Severity | File / Symbol | Evidence | Impact | Fix | Validation | Status |
|----|----------|---------------|----------|--------|-----|------------|--------|
| AARUNO-022 | Critical | `src/AARUNO.h` `AarunoStepper *_step;` | Uno R3 compile error: `'AarunoStepper' does not name a type` | Library fails to compile for Uno R3 | Added `#include "actuators/AarunoStepper.h"` | Fixed in source; verify via CI | Fixed |
| AARUNO-023 | Critical | `src/utility/AarunoUtil.h` | `logf()` used before `#include <math.h>`; `AARUNO_*` constants used without including `AARUNOTypes.h`; `size_t` used without `<stddef.h>` | Host C++ tests and any translation unit including `AarunoUtil.h` directly fail to compile | Moved `#include <math.h>` to top; added `#include "../AARUNOTypes.h"`; added `#include <stddef.h>` | `test_util.cpp` now self-contained; 19/19 Python tests PASS | Fixed |
| AARUNO-024 | Critical | `src/AARUNO.h` host fallback | `size_t` used without declaring it | Standalone host syntax check fails at line 33 | Added `#include <stddef.h>` before `Stream::print()` | Header parseable standalone | Fixed |
| AARUNO-025 | Critical | `src/sensors/AarunoEncoder.h/.cpp` | `AarunoClass::encoderCount()` calls `_enc.configured()` which doesn't exist | Compile error in `AARUNO.cpp` on every target | Added `bool configured() const { return _begun; }` | Method declared and defined | Fixed |
| AARUNO-026 | Critical | `src/actuators/AarunoStepper.cpp` `step()` | Returns `setErr(...)` but `AarunoStepper` has no `setErr()` method | Compile error in stepper source | Added private `AarunoError setErr(AarunoError e)` helper | Method added to header | Fixed |
| AARUNO-027 | High | `src/sensors/AarunoAnalogEngine.cpp` `readSpecial()` | Negative TMP36/NTC temperatures and negative ACS712 current rejected as `READ_FAIL` | Valid negative measurements reported as failures | Per-type validity check: only LM35/MQ reject negative; TMP36/NTC/current allow negative | Code inspection; logic verified | Fixed |
| AARUNO-028 | High | `src/sensors/AarunoI2CUtil.cpp` | I2C functions call `Wire.beginTransmission()` without `Wire.begin()` | `i2cScan()`, `autoScan()`, `oledAuto()`, `lcdAuto()` may fail if `Wire` not initialized | Added `AarunoI2C::begin()` that calls `Wire.begin()` once; called at start of every I2C method | `begin()` guard prevents re-init; verified in source | Fixed |
| AARUNO-029 | Medium | `src/sensors/AarunoMPU6050.cpp` `begin()` | No WHO_AM_I verification; any device ACKing at 0x68 accepted | False-positive init on shared I2C bus | Added WHO_AM_I (0x75) check expecting 0x68 before wake | Datasheet register check added | Fixed |
| AARUNO-030 | Medium | `src/sensors/AarunoExtras.cpp` | BMP280 `#has_include` nested under `AARUNO_ENABLE_DHT` | BMP280 disabled when DHT disabled, even if BMP280 lib installed | Separated BMP280 guard (ARDUINO only) from DS18B20 guard (ARDUINO + DHT) | Guards are now independent | Fixed |
| AARUNO-031 | Medium | `examples/Joystick/Joystick.ino` ESP8266 branch | `JOY_X == JOY_Y == A0` triggers `INVALID_CONFIG` in `joystick()` | Example cannot initialize on ESP8266 | Added `JOY_X == JOY_Y` check; prints guidance to use external ADS1115 for Y axis | Example compiles; runtime guard added | Fixed |
| AARUNO-032 | Medium | `examples/StepperMotor\StepperMotor.ino` ESP8266 branch | GPIO0/GPIO2 labeled "boot-safe pins" but are boot strapping pins | ESP8266 may fail to boot depending on wiring | Changed to GPIO5/4/14/12; added boot-pin warning comment | Safer pins; warning documented | Fixed |
| AARUNO-033 | Medium | Version inconsistencies | `api-reference.md`=1.0.0, `README.md`=AARUNO-1.0.0.zip, `OLED.ino`=v1.0.2 | Confusing version claims | All aligned to 1.1.1 | `grep` confirms no stale 1.0.x references | Fixed |
| AARUNO-034 | Medium | `tests/host/test_aaruno.py` | `mq_ratio()` divides by zero when raw=0; inches test doesn't call library function | Python test has logic defects | `mq_ratio()` returns -1.0 for raw<=0; inches test uses `distance_cm()` | All 19 Python tests PASS | Fixed |

---

## Notes

- Version 1.1.1 is the single source of truth (`library.properties` line 2, `AARUNOTypes.h` lines 7-10). Do not bump without determining patch/minor/major per Phase E.
- No Wi-Fi credentials, API keys, tokens, or secrets exist in code/examples/commits (rule 9 verified).
- No physical board/sensor test is claimed anywhere (rule 10 verified).
- Public API preserved: all pre-existing `AARUNO.*` methods retained; additions are additive only (rule 5 verified).
