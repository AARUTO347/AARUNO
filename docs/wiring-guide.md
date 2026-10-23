# Wiring Guide

## HC-SR04 ultrasonic

- VCC → 5 V (Uno 5V / ESP VIN 5V if USB-powered), GND → GND.
- Trig → any valid output GPIO.
- Echo → Uno: direct to GPIO. ESP: through divider:
  `Echo (5V) -- 1k --+-- ESP GPIO` and `+ -- 2k -- GND`.
- Timeout reads (no echo) usually mean wiring/power, not code.

## DHT11 / DHT22

- VCC → 3.3–5 V per module, DATA → GPIO with 4.7–10 kΩ pull-up to VCC (many modules include it), GND → GND.
- Keep wires < 20 m; wait 2 s after power before first read.

## LDR (light)

- Divider: `VCC -- LDR --+-- 10k -- GND`, `+` → analog pin.
- Bright light → low or high depending on LDR position; calibrate threshold from `readLDR()` serial output. Values are raw counts, not lux.

## Button

- `GPIO -- button -- GND` with internal pull-up (`AARUNO.button(pin)` default). Pressed = LOW.
- Debounce (50 ms) handled in `AARUNO.update()`.

## PIR (HC-SR501)

- VCC → 5 V, OUT → GPIO, GND → GND. Adjust sensitivity/time pots on module. Warm-up ~30 s.

## LED / buzzer / relay

- LED: `GPIO -- 220Ω -- LED -- GND`.
- Active buzzer: `GPIO -- (+)BUZZER(-) -- GND`, use `beep(ms)`.
- Passive buzzer: same wiring, use `beep(freqHz, ms)`.
- Relay module: VCC → 5 V, GND → GND, IN → GPIO; load on COM/NO/NC, **never** power load from GPIO. Set `relay(pin, activeHigh)` to match your module; default OFF is applied at setup.

## OLED SSD1306 / LCD I2C

- SDA → SDA (Uno A4 / ESP32 21 / ESP8266 4), SCL → SCL (Uno A5 / ESP32 22 / ESP8266 5), VCC → 3.3 V (ESP) or 5 V per module, GND → GND.
- Default address 0x3C; some modules use 0x3D — scan if `oled()` shows nothing.

## ESP WiFi

- No extra wiring. Keep antenna area clear; 2.4 GHz networks only on most ESP boards.
