# Troubleshooting

- `ultrasonic()` returns error / `INVALID_PIN`: pin not valid on this board (e.g. ESP32 pin 6–11, input-only 34–39 as trig, ESP8266 pin 6–11). See pin-reference.
- `readUltrasonic()` always TIMEOUT: echo never returns. Check echo wiring/divider, common GND, 5 V supply, object in range (2 cm–4 m). `pulseIn` timeout default 30000 us.
- Distance stuck at max: object too far/absorbent (curtains) or sensor angled.
- DHT `MISSING_DEP`: install `DHT sensor library` + `Adafruit Unified Sensor`.
- DHT `READ_FAIL`: wiring/pull-up, wait 2 s between reads, 3.3 V vs 5 V supply.
- LDR always 0/1023/4095: wrong pin (ESP8266 only A0; ESP32 use ADC1), missing divider.
- `servoAttach` → `MISSING_DEP`: install `Servo` library (bundled with most cores).
- `wifiBegin` → `UNSUPPORTED`: called on Uno. Only ESP32/ESP8266 supported in 1.0.
- `oled/lcd` → `MISSING_DEP`: install Adafruit SSD1306+GFX / LiquidCrystal_I2C.
- Relay inverted: pass correct polarity `relay(pin, false)` for active-LOW modules.
- ESP boot loops with relay/motor: GPIO 0/2/15 strapping or power sag — use separate 5 V supply with common GND.
- `NOT_INITIALIZED`: forgot `AARUNO.begin()` or module setup before read.
