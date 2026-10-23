# Changelog

All notable changes to AARUNO follow Semantic Versioning.

## [1.1.1] - 2026-10-23 — MADE BY AARUTO

### Added
- I2C auto-detect: `autoScan()` device count, `oledAuto()` (0x3C/0x3D),
  `lcdAuto()` (0x27/0x38, 16x2 default) — no more hardcoding addresses.

## [1.1.0] - 2026-10-23 — MADE BY AARUTO

### Added
- Generic analog engine + named wrappers: analogSensor, LM35, TMP36,
  thermistor, soil, rain, water level, sound, flex, FSR force, flame (analog),
  MQ-2/3/4/5/6/7/8/9/135, ACS712 current; reads: raw/percent/voltage/special.
- Generic digital engine + named wrappers: digitalSensor, line follower,
  flame, SW-420 vibration, tilt, touch (TTP223), hall (A3144), reed,
  limit switch, IR receiver (raw level, no decoding).
- Rotary encoder polling, 4x4 keypad matrix scan, XY joystick.
- MPU6050 IMU real I2C driver (no external lib): accel/gyro/temp.
- DS18B20 (OneWire+DallasTemperature) and BMP280 (Adafruit) optional drivers.
- Generic I2C helpers: device ACK probe, scan, reg read/write 8-bit.
- I2C LCD helpers: lcdI2C alias, displayTextAt(row,col), displayBacklight.
- RGB LED (3 PWM), laser emitter (KY-008), solenoid via relay, generic PWM out.
- 28BYJ-48 stepper via ULN2003 (blocking per step, RPM clamp).
- ESP HTTP GET (WiFiClient/HTTPClient, streams body, no full-buffer heap).
- madeBy()/credits() — "MADE BY AARUTO".
- 9 new examples (I2CLCD, GasSensor, Joystick, RotaryEncoder, Keypad4x4,
  MPU6050, RGBLED, StepperMotor, HTTPGet); ~150 public functions total.
- Ultrasonic inches helper (readUltrasonicInches).

### Fixed
- ESP8266 analog validation: removed invalid GPIO-0 fallback.
- Display row units documented (row = text line index).
- AARUNO_ENABLE_SERVO macro removed (Servo auto-detected via __has_include).

## [1.0.2] - 2026-10-17

### Fixed
- CI: install avr/renesas_uno/esp32/esp8266 cores via platforms input.
- Removed dead actuators/AarunoOutput.h (stale stub, nothing included it).
- LCD: probe I2C ACK before init() to avoid hangs on missing hardware.
- wifiRSSI: distinct lastError per state (UNSUPPORTED/IDLE/CONNECTING/FAILED).
- __has_include guarded for older GCC compatibility.
- README documents singleton scope (one of each peripheral).

## [1.0.1] - 2026-10-17

### Fixed
- Displays: real SSD1306/LCD init, render, clear with INIT_FAIL on NACK.
- PWM: board-aware validation; setBrightness syncs LED state; motor EN needs PWM.
- Analog: dedicated validation; ESP8266 A0-only enforced.
- Button: rose()=press, fell()=release (logical, activeLow-aware).
- WiFi: non-blocking start()/update()/state() serviced by AARUNO.update().
- DHT: fresh()/ageMs() distinguish fresh vs cached reads.
- Servo: no heap, attach() verified.
- Motor: rejects in1==in2 and EN conflicts.
- ZIP: forward-slash paths; repository URL corrected.
- LEDBuzzer example uses PWM-capable pins; WiFi examples use non-blocking API.

## [1.0.0] - 2026-10-16

### Added
- Core singleton API: `AARUNO.begin()`, `AARUNO.update()`, version, error/status system.
- Board detection for Uno R3, Uno R4 Minima, Uno R4 WiFi, ESP32, ESP8266.
- HC-SR04 ultrasonic driver with timeout, configurable timeout, multi-instance class.
- Convenience aliases `ultrasonicesp32`, `ultrasonicesp8266`, `ultrasonicarduino` (setup + read overloads).
- DHT11/DHT22 support via optional `DHT sensor library` dependency.
- Analog: LDR, potentiometer, raw analog, voltage-divider helper.
- Digital: input/output, debounced button, PIR, IR obstacle, limit switch helper.
- Actuators: LED, PWM brightness, active/passive buzzer, relay with configurable polarity.
- Optional SSD1306 OLED and I2C LCD adapters (no forced dependency).
- Optional ESP32/ESP8266 WiFi adapter, compiles only on ESP.
- 9 Arduino examples with per-board valid pins.
- Host logic tests (Python) + compile CI matrix for all 5 board families.
- Full docs: architecture, compatibility matrix, pin reference, wiring, troubleshooting, API reference.
