# AARUNO — Fix Report (v1.1.1)

Repository: https://github.com/AARUTO347/AARUNO
Library version: 1.1.1
Audit date: Mon Nov 09 2026

## Summary of Changes

### Compile Blockers Fixed

| File | Issue | Fix |
|------|-------|-----|
| `src/AARUNO.h` | `AarunoStepper *_step` used but header not included | Added `#include "actuators/AarunoStepper.h"` |
| `src/utility/AarunoUtil.h` | `logf()` used before `#include <math.h>`; `AARUNO_*` constants without `AARUNOTypes.h`; `size_t` without `<stddef.h>` | Moved `#include <math.h>` to top; added `#include "../AARUNOTypes.h"`; added `#include <stddef.h>` |
| `src/AARUNO.h` | `size_t` used without declaring it in host fallback | Added `#include <stddef.h>` |
| `src/sensors/AarunoEncoder.h` | `configured()` method missing (called by `AARUNO.cpp`) | Added `bool configured() const { return _begun; }` |
| `src/actuators/AarunoStepper.h` | `setErr()` called but not defined | Added `AarunoError setErr(AarunoError e)` private helper |

### Runtime Bugs Fixed

| File | Issue | Fix |
|------|-------|-----|
| `src/sensors/AarunoAnalogEngine.cpp` | Negative TMP36/NTC/ACS712 readings rejected as `READ_FAIL` | Per-type validity: only LM35/MQ reject negative; TMP36/NTC/current allow negative |
| `src/sensors/AarunoI2CUtil.cpp` | I2C functions called `Wire.beginTransmission()` without `Wire.begin()` | Added `AarunoI2C::begin()` that calls `Wire.begin()` once; called at start of every I2C method |
| `src/sensors/AarunoMPU6050.cpp` | No WHO_AM_I verification; any device ACKing at 0x68 accepted | Added WHO_AM_I (0x75) check expecting 0x68 before wake |
| `src/sensors/AarunoExtras.cpp` | BMP280 `Wire.begin()` called directly instead of `AarunoI2C::begin()`; `<Wire.h>` inside `__has_include` guard | Moved `<Wire.h>` outside guard; changed to `AarunoI2C::begin()` |

### Memory Safety Fixes

| File | Issue | Fix |
|------|-------|-----|
| `src/AARUNO.cpp` `stepperAttach()` | `new AarunoStepper()` with no `delete` — memory leak | Replaced with static `AarunoStepper _step` member + `_stepBegun` flag |
| `src/AARUNO.h` | `_step` was `AarunoStepper*` pointer | Changed to `AarunoStepper _step` value + `_stepBegun` bool |

### HTTP/HTTPS Improvements (`src/communication/AarunoHTTP.*`)

| Issue | Fix |
|-------|-----|
| `http.getString()` buffers entire response in heap | Streaming read in 128-byte chunks via `getStreamPtr()` |
| No response-size limit | Configurable `setMaxResponseSize()`; returns -3 if exceeded |
| No connection/read timeouts | `setConnectTimeout()` / `setReadTimeout()`; passed to `HTTPClient` |
| `void *streamOut` type-unsafe | Changed to `Stream *` member set via `setStream()` |
| `http.end()` not on all paths | `http.end()` called on every return path |
| TLS certificate validation | Not disabled — uses default `HTTPClient` behavior (certificate validation enabled) |

### WiFi Improvements (`src/communication/AarunoWifi.*`)

| Issue | Fix |
|-------|-----|
| Blocking `begin()` with `delay(250)` loop | Kept as documented blocking helper; non-blocking `start()`/`update()` preferred |
| No retry policy | Added `setRetryPolicy(retries, delayMs)`; retry on connection failure |
| No reconnection on drop | `update()` detects `WL_CONNECTED` loss and attempts reconnect |
| No retry count tracking | Added `retryCount()` accessor |

### API Additions

| Method | Description |
|--------|-------------|
| `AARUNO.httpSetConnectTimeout(ms)` | Configure HTTP connect timeout |
| `AARUNO.httpSetReadTimeout(ms)` | Configure HTTP read timeout |
| `AARUNO.httpSetMaxResponseSize(bytes)` | Configure max response body size |
| `AARUNO.httpLastStatusCode()` | Last HTTP status code |
| `AARUNO.httpLastBodyBytes()` | Last response body bytes streamed |
| `AARUNO.wifiStart(ssid, pass, timeoutMs)` | Non-blocking WiFi connect |
| `AARUNO.wifiState()` | WiFi connection state machine |
| `AARUNO.oledAuto()` | Auto-detect OLED at 0x3C/0x3D |
| `AARUNO.lcdAuto()` | Auto-detect LCD at 0x27/0x38 |
| `AARUNO.autoScan()` | I2C device count |

### Driver Classification

| Driver | Status | Notes |
|--------|--------|-------|
| Ultrasonic HC-SR04 | Implemented, not HW verified | `pulseIn()` with timeout; blocking |
| DHT11/DHT22 | Implemented, not HW verified | Requires external lib; `MISSING_DEP` if absent |
| LDR/Potentiometer/Analog | Implemented, not HW verified | Raw ADC; validated pins |
| LM35/TMP36/Thermistor | Implemented, not HW verified | Math helpers; negative valid for TMP36/NTC |
| MQ gas sensors | Implemented, not HW verified | Raw RS/RL ratio; NOT calibrated ppm |
| ACS712 Current | Implemented, not HW verified | Amps conversion; negative current valid |
| Digital sensors (PIR/IR/Flame/etc.) | Implemented, not HW verified | Digital level with polarity config |
| Button | Implemented, not HW verified | Debounced; logical press/release |
| LED/PWM/Buzzer/Relay | Implemented, not HW verified | Pin validated; default OFF |
| DC Motor/Stepper/Servo | Implemented, not HW verified | External driver required |
| OLED/LCD | Implemented, not HW verified | Requires optional libs; `MISSING_DEP` if absent |
| ESP32/ESP8266 WiFi | Implemented, not HW verified | Non-blocking + blocking variants |
| HTTP GET | Implemented, not HW verified | Streaming with size limits/timeouts |
| MPU6050 IMU | Implemented, not HW verified | WHO_AM_I verification |
| DS18B20/BMP280 | Implemented, not HW verified | Requires optional libs |
| Rotary Encoder/Keypad/Joystick | Implemented, not HW verified | Polling/non-blocking |

## Testing Status

| Test Type | Status | Notes |
|-----------|--------|-------|
| Python host tests (`tests/host/test_aaruno.py`) | 19/19 PASS | Logic validation |
| C++ host tests (`tests/host/test_util.cpp`) | Compiles and passes | No Arduino headers needed |
| Board compilation (Uno R3, R4 Minima, R4 WiFi, ESP32, ESP8266) | NOT RUN LOCALLY | No local Arduino toolchain; CI workflow configured |
| Hardware validation | NOT RUN | No physical sensors/boards connected |

## CI Configuration

`.github/workflows/compile-examples.yml` compiles all 18 examples across 5 board families:
- `arduino:avr:uno` (Uno R3)
- `arduino:renesas_uno:minima` (Uno R4 Minima)
- `arduino:renesas_uno:unowifi` (Uno R4 WiFi)
- `esp32:esp32:esp32` (ESP32)
- `esp8266:esp8266:nodemcuv2` (ESP8266)

Optional dependencies installed: DHT sensor library, Adafruit Unified Sensor, Adafruit SSD1306, Adafruit GFX Library, LiquidCrystal I2C, OneWire, DallasTemperature, Adafruit BMP280 Library, SPI.

## Unresolved Issues

| ID | Severity | Description |
|----|----------|-------------|
| Board compilation | High | Cannot verify without Arduino CLI toolchain locally; CI workflow will verify on push |
| Hardware validation | High | No physical boards/sensors available; all drivers marked "not HW verified" |
| AARUNOMultiMotor | Medium | Independent motor channel objects not implemented; single facade channel only |
| R4 WiFi native networking | Medium | WiFi only on ESP32/ESP8266; R4-WiFi returns `UNSUPPORTED` |

## Version

Single source of truth: `library.properties` line 2 + `src/AARUNOTypes.h` lines 7-10 = `1.1.1`.
