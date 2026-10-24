# AARUNO — Universal Arduino & ESP Library
## MADE BY AARUTO

One consistent API for ultrasonic, DHT11/DHT22, LDR, buttons, PIR, LED, buzzer, relay, motor, servo, OLED/LCD and ESP WiFi across:

- Arduino Uno R3 (ATmega328P, 5 V)
- Arduino Uno R4 Minima
- Arduino Uno R4 WiFi
- ESP32 family (3.3 V)
- ESP8266 family incl. NodeMCU / Wemos D1 mini (3.3 V)

Core has **no mandatory third-party dependencies**. Displays, DHT and Servo are optional.

## Install (Arduino IDE → .zip)

1. Download this repo as `.zip` (Code → Download ZIP) or use the release `AARUNO-1.0.0.zip`.
2. Arduino IDE → Sketch → Include Library → Add .ZIP Library → select the file.
3. Check: File → Examples → AARUNO → Basic.

## Install (Arduino CLI)

```sh
arduino-cli lib install --zip-path AARUNO-1.0.0.zip
# Optional deps (only if you use those modules):
arduino-cli lib install "DHT sensor library" "Adafruit Unified Sensor"
arduino-cli lib install "Adafruit SSD1306" "Adafruit GFX Library"
arduino-cli lib install "LiquidCrystal I2C"
```

## Quick start

```cpp
#include <AARUNO.h>

void setup() {
  AARUNO.begin();
  AARUNO.ultrasonic(7, 8);  // Uno pins; ESP32 e.g. (5, 18)
  AARUNO.lcdI2C(0x27, 16, 2);  // I2C LCD (needs LiquidCrystal I2C)
  AARUNO.led(13);
  AARUNO.buzzer(9);
}

void loop() {
  AARUNO.update();
  float cm = AARUNO.readUltrasonic();
  if (AARUNO.lastError() != AARUNO_OK) return;  // timeout / not configured
  if (cm > 0 && cm < 20) {
    AARUNO.setLED(true);
    AARUNO.beep(2000, 100);
    AARUNO.clearError();
    AARUNO.displayText("Close!");
  } else {
    AARUNO.setLED(false);
  }
}
```

Board-specific aliases (same units, delegate to singleton):

```cpp
AARUNO.begin();
ultrasonicarduino(7, 8);
float cm = ultrasonicarduino();
```

## Safety

- ESP32/ESP8266 GPIO is **3.3 V, not 5 V tolerant** (typical). HC-SR04 echo is 5 V → use a voltage divider (e.g. 1k/2k) or level shifter. Software cannot fix wiring.
- Never drive motors, relays, servos or high-current loads directly from GPIO. Use driver modules + external power.
- Relays can be active-HIGH or active-LOW: `AARUNO.relay(pin, activeHigh)`.

## Docs

- `docs/api-reference.md` — full function list, units, errors.
- `docs/board-compatibility.md` — compatibility matrix.
- `docs/pin-reference.md` — safe pins per board.
- `docs/wiring-guide.md` — wiring + divider diagrams (text).
- `docs/troubleshooting.md` — common failures.
- `docs/architecture.md`, `docs/adding-a-driver.md`.

## API sketch

| Component | API |
|---|---|
| Ultrasonic | `AARUNO.ultrasonic(trig, echo)` / `readUltrasonic()` |
| DHT11/22 | `AARUNO.dht11(pin)` / `readTemperature()` / `readHumidity()` |
| LDR | `AARUNO.ldr(pin)` / `readLDR()` |
| Button | `AARUNO.button(pin)` / `buttonPressed()` |
| PIR | `AARUNO.pir(pin)` / `readPIR()` |
| LED | `AARUNO.led(pin)` / `setLED()` / `setBrightness()` |
| Buzzer | `AARUNO.buzzer(pin)` / `beep()` |
| Relay | `AARUNO.relay(pin, activeHigh)` / `setRelay()` |
| Error | `AARUNO.lastError()` / `AARUNO.errorString()` |

Failures return `NaN` (float), `-1` (int) or `false` (bool) **plus** a code in `lastError()`. Zero is never used as a generic failure for distance.

Singleton scope: the global `AARUNO` object holds ONE of each peripheral (one ultrasonic, one DHT, one button, one servo). For multiple sensors of the same type, use the independent classes directly (e.g. `AarunoUltrasonic`); for multiple servos/displays, use the driver object per device.

## License

MIT — see `LICENSE`.
"# AARUNO" 
"# AARUNO" 
