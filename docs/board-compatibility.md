# Board Compatibility — AARUNO 1.0.0

Legend: ✅ supported · ⚠️ limited · ❌ unavailable (returns error, never fakes data).

| Feature | Uno R3 | Uno R4 Minima | Uno R4 WiFi | ESP32 | ESP8266 |
|---|---|---|---|---|---|
| GPIO / digital IO | ✅ | ✅ | ✅ | ✅ (no 6–11, 34–39 input-only) | ✅ (0,1,2,3,4,5,12–16) |
| Ultrasonic HC-SR04 | ✅ | ✅ | ✅ | ✅ + divider on echo | ✅ + divider on echo |
| Button / PIR / IR | ✅ | ✅ | ✅ | ✅ | ✅ |
| Analog / LDR / pot | ✅ A0–A5 10-bit | ✅ 14-bit | ✅ 14-bit | ✅ 12-bit, ADC1 preferred | ⚠️ A0 only |
| LED / PWM | ✅ (3,5,6,9,10,11) | ✅ | ✅ | ✅ | ✅ (0–16 limits) |
| Buzzer tone() | ✅ | ✅ | ✅ | ✅ | ✅ |
| Relay (via module) | ✅ | ✅ | ✅ | ✅ | ✅ |
| Servo | ✅ (Servo.h) | ✅ | ✅ | ✅ | ✅ |
| DC motor (driver) | ✅ | ✅ | ✅ | ✅ | ✅ |
| DHT11/22 | ✅ + lib | ✅ + lib | ✅ + lib | ✅ + lib | ✅ + lib |
| OLED SSD1306 (I2C) | ✅ + lib | ✅ + lib | ✅ + lib | ✅ + lib | ✅ + lib |
| LCD I2C | ✅ + lib | ✅ + lib | ✅ + lib | ✅ + lib | ✅ + lib |
| WiFi / HTTP | ❌ UNSUPPORTED | ❌ (Minima: no radio) | ✅ native R4 WiFi* | ✅ | ✅ |
| ADC Vref | 5 V | 5 V | 5 V | 3.3 V | 3.3 V (A0 divider on board) |

\* Uno R4 WiFi networking uses the Renesas + ESP32-S3 bridge; AARUNO 1.0 exposes WiFi only on ESP32/ESP8266 and returns `UNSUPPORTED` elsewhere. R4-WiFi native support is roadmap.

## Unknown boards

Pin validation is permissive on unknown boards by design (compiles on new variants; Arduino core enforces limits). PWM/analog helpers accept and document this fallback.

## Voltage hazards

- Uno = 5 V logic. ESP = 3.3 V, generally **not 5 V tolerant**.
- HC-SR04 VCC 5 V, echo 5 V → divide to ≤3.3 V for ESP (e.g. echo → 1 kΩ → ESP pin → 2 kΩ → GND).
- Powering 5 V sensors from ESP 5 V/VIN pin is for supply only; signal lines still need shifting.

## PWM / ADC notes

- Uno R3: `analogWrite` on 3,5,6,9,10,11 only.
- ESP32: `analogWrite` emulated (LEDC); pins 34–39 cannot output.
- ESP8266: single ADC (A0, 0–3.3 V after on-board divider on NodeMCU).
