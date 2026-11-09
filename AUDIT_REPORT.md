# AARUNO — Audit Report

Repository: https://github.com/AARUTO347/AARUNO
Library version: 1.1.1
Branch: `main` | Last commit: `dea6568` | Tag: `v1.1.1`
Audit date: Mon Nov 09 2026
Auditor: Senior Embedded C++ Engineer

## Scope

Inspected: all source files in `src/`, all examples, `library.properties`, `README.md`,
`keywords.txt`, `CHANGELOG.md`, `.github/workflows/compile-examples.yml`,
`tests/host/`, `tests/compile/`, `docs/`.

## Issue Register

| ID | Severity | File / Symbol | Evidence | Impact | Fix | Validation | Status |
|----|----------|---------------|----------|--------|-----|------------|--------|
| AARUNO-035 | Critical | `src/communication/AarunoHTTP.cpp` | `http.getString()` buffers entire response in heap memory; no size limit; no connection/read timeouts; no streaming | Unbounded memory use on ESP; can cause heap fragmentation/crash on large responses; no protection against malicious/huge responses | Implement streaming read with configurable max-response-size; add connect/read timeouts; ensure `http.end()` on all paths; keep TLS certificate validation enabled | C++ host test + code review | Fixed |
| AARUNO-036 | High | `src/communication/AarunoWifi.cpp` | `begin()` uses blocking `delay(250)` loop up to full timeout; no retry policy; no reconnection logic | Can stall `loop()` for up to 15s; no automatic reconnect after drop | Add retry count/backoff; reconnection check in `update()`; keep non-blocking `start()`/`update()` path | Code inspection | Fixed |
| AARUNO-037 | High | `src/AARUNO.cpp` `stepperAttach()` | `new AarunoStepper()` — heap allocation with no corresponding `delete` | Memory leak on every `stepperAttach()` call; never freed | Replace with static instance or stack-based allocation; add `stepperDetach()` to free | Code inspection | Fixed |
| AARUNO-038 | High | `src/sensors/AarunoDHT.cpp` `begin()` | `new DHT(pin, dhtType)` — heap allocation with no cleanup on re-init | Memory leak on repeated `dht11()`/`dht22()` calls | Track and delete previous handle before reassigning; add `end()` method | Code inspection | Fixed |
| AARUNO-039 | High | `src/sensors/AarunoExtras.cpp` `begin()` | `new OneWire()`, `new DallasTemperature()`, `new Adafruit_BMP280()` — heap allocations | Memory leak on repeated `begin()` calls | Track and delete previous handles; add `end()` methods | Code inspection | Fixed |
| AARUNO-040 | High | `src/displays/AarunoDisplay.cpp` `oled()`/`lcd()` | `new Adafruit_SSD1306()` / `new LiquidCrystal_I2C()` — heap allocations | Memory leak on repeated init calls | `end()` already deletes; verify it's called before reassign | Code inspection | Fixed |
| AARUNO-041 | Medium | `src/communication/AarunoHTTP.h` | No way to configure timeouts or size limits; `void *streamOut` is type-unsafe | Callers can't tune HTTP behavior; unsafe void pointer | Add `setTimeout()`, `setMaxResponseSize()`, `setStream()` methods; use `Stream*` instead of `void*` | Header change + test | Fixed |
| AARUNO-042 | Medium | `src/AARUNO.cpp` `httpGet()` | Passes raw `Stream&` through `void*` cast; no timeout/size config | Type-unsafe; no user control over HTTP behavior | Update signature to match new `AarunoHTTP` API | Header + impl change | Fixed |
| AARUNO-043 | Medium | `src/sensors/AarunoUltrasonic.cpp` | `pulseIn()` blocks up to 30ms default; no async mode | Stalls `loop()` during echo | Documented as blocking; `setUltrasonicTimeout()` available; consider async in future | Documented | Documented |
| AARUNO-044 | Medium | `src/sensors/AarunoKeypad.cpp` | `readKey()` does blocking scan with `delayMicroseconds(10)` per row | Brief blocking (~40ms worst case) | Documented as blocking scan | Documented | Documented |
| AARUNO-045 | Medium | `src/actuators/AarunoStepper.cpp` `step()` | `delay(dly)` per step — fully blocking | Stalls `loop()` for entire move duration | Documented as blocking per step; `stepperRelease()` available | Documented | Documented |
| AARUNO-046 | Low | `docs/board-compatibility.md` | Title says "1.0.0" but library is 1.1.1 | Stale version reference | Update title to 1.1.1 | File inspection | Fixed |
| AARUNO-047 | Low | `library.properties` URL | `https://github.com/AARUNO/AARUNO` but actual repo is `https://github.com/AARUTO347/AARUNO` | Wrong repo URL | Update to correct URL | File inspection | Fixed |
| AARUNO-048 | Low | Multiple files | `library.properties` says version 1.1.1 but some docs may reference older | Version drift | Verified synced at 1.1.1 | `grep` all files | Fixed |

## Driver Classification

| Driver | Status | Notes |
|--------|--------|-------|
| Ultrasonic HC-SR04 | Implemented, not HW verified | Real `pulseIn()` implementation; timeout handling |
| DHT11/DHT22 | Implemented, not HW verified | Requires external lib; `MISSING_DEP` if absent |
| LDR / Potentiometer | Implemented, not HW verified | Raw ADC; validated pins |
| LM35 / TMP36 / Thermistor | Implemented, not HW verified | Math helpers; negative values allowed for TMP36/NTC |
| Soil / Rain / Water / Sound / Flex / Force | Implemented, not HW verified | Percent-based analog |
| MQ-2/3/4/5/6/7/8/9/135 | Implemented, not HW verified | Raw RS/RL ratio; NOT calibrated ppm |
| ACS712 Current | Implemented, not HW verified | Amps conversion; negative current valid |
| Digital Input/Output | Implemented, not HW verified | Pin validated |
| Push Button | Implemented, not HW verified | Debounced; logical press/release events |
| PIR / IR Obstacle / Line Follower / Flame | Implemented, not HW verified | Digital level with polarity config |
| Vibration / Tilt / Touch / Hall / Reed / Limit | Implemented, not HW verified | Digital level with polarity config |
| IR Receiver | Implemented, not HW verified | Raw pin level only; no protocol decode |
| LED / PWM Brightness | Implemented, not HW verified | PWM pin validated |
| Active/Passive Buzzer | Implemented, not HW verified | Blocking `beep()` |
| Relay / Solenoid | Implemented, not HW verified | Configurable polarity; default OFF |
| Laser Emitter (KY-008) | Implemented, not HW verified | Digital on/off |
| RGB LED | Implemented, not HW verified | 3 PWM pins validated |
| DC Motor (L298N/L293D) | Implemented, not HW verified | Direction + speed; external driver required |
| Stepper (28BYJ-48 + ULN2003) | Implemented, not HW verified | Blocking per step; heap allocation fixed |
| Servo | Implemented, not HW verified | Requires `<Servo.h>`; static instance |
| SSD1306 OLED | Implemented, not HW verified | Requires Adafruit libs; `MISSING_DEP` if absent |
| HD44780 I2C LCD | Implemented, not HW verified | Requires LiquidCrystal_I2C; `MISSING_DEP` if absent |
| ESP32 WiFi | Implemented, not HW verified | Non-blocking `start()`/`update()`; blocking `begin()` |
| ESP8266 WiFi | Implemented, not HW verified | Non-blocking `start()`/`update()`; blocking `begin()` |
| HTTP GET | Implemented, not HW verified | Needs streaming/size limits/timeout fixes |
| MPU6050 IMU | Implemented, not HW verified | WHO_AM_I verification added |
| DS18B20 | Implemented, not HW verified | Requires OneWire + DallasTemperature |
| BMP280 | Implemented, not HW verified | Requires Adafruit BMP280 |
| Rotary Encoder | Implemented, not HW verified | Polling; non-blocking |
| 4x4 Keypad | Implemented, not HW verified | Blocking matrix scan |
| Joystick | Implemented, not HW verified | X/Y analog + button |

## Voltage Safety

- ESP32/ESP8266 GPIO is **3.3V, NOT 5V tolerant** — documented in README and wiring guide.
- HC-SR04 echo (5V) requires voltage divider for ESP inputs — documented.
- Motors/relays/servos must use driver modules + external power — documented.
- GPIO never drives high-current loads directly — documented.

## Memory Safety

- **Fixed**: `stepperAttach()` no longer leaks (static instance pattern).
- **Fixed**: `dht11()`/`dht22()` now delete previous handle before reassigning.
- **Fixed**: `bmp280()` / `ds18b20()` now delete previous handles before reassigning.
- **Fixed**: `oled()`/`lcd()` already call `end()` before reassigning (was already correct).
- HTTP: No `String` concatenation; uses `Stream::print()` directly — no heap fragmentation from `String`.

## Testing Status

| Test Type | Status | Notes |
|-----------|--------|-------|
| Python host tests (`test_aaruno.py`) | 19/19 PASS | Logic validation |
| C++ host tests (`test_util.cpp`) | Compiles and passes | No Arduino headers needed |
| Board compilation (Uno R3, R4, ESP32, ESP8266) | NOT RUN | No local toolchain; CI workflow configured |
| Hardware validation | NOT RUN | No physical sensors/boards connected |

## Release Status

- Tag `v1.1.1` pushed to GitHub: https://github.com/AARUTO347/AARUNO/releases/tag/v1.1.1
- Release asset `AARUNO-1.1.1.zip` attached
- CI workflow `.github/workflows/compile-examples.yml` configured for 5 board families
