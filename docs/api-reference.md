# API Reference - AARUNO 1.1.1

All functions are methods on the global `AARUNO` singleton unless noted.
Check `AARUNO.lastError()` after reads. `AARUNO_OK == 0`.

## Core

- `void begin()` — must be called first. Sets ADC resolution per board.
- `void update()` — call every loop. Services button debounce.
- `const char* version()` - e.g. `"1.1.1"`.
- `AarunoError lastError() / clearError() / errorString()`.

Error codes: `OK, ERROR, INVALID_PIN, INVALID_CONFIG, TIMEOUT, READ_FAIL, UNSUPPORTED, MISSING_DEP, INIT_FAIL, NOT_INITIALIZED`.

## Ultrasonic

- `ultrasonic(trig, echo)` — configures shared sensor. Validates pins; `trig != echo`.
- `setUltrasonicTimeout(us)` — default 30000.
- `float readUltrasonic()` — cm, or `NaN` on fail. BLOCKING up to timeout (pulseIn).
- `bool readUltrasonic(float &cm)` — status-aware form.
- `AarunoUltrasonic` class — independent instances for multi-sensor projects.
- Aliases: `ultrasonicesp32(trig,echo)+ultrasonicesp32()`, `ultrasonicesp8266`, `ultrasonicarduino`. Same units.

Units: cm. Formula `cm = echo_us / 58`. Timeout → `lastError()==TIMEOUT`.

## DHT

- `dht11(pin)` / `dht22(pin)` — needs DHT sensor library; else `MISSING_DEP`.
- `readTemperature()` °C, `readHumidity()` %RH, `readDHT(t,h)`.
- Min interval 2000 ms enforced; checksum failures → `READ_FAIL`, never invented data.
- Failed reads return NaN (never stale-as-success); `dhtFresh()` / `dhtAgeMs()` tell fresh vs cached.

## Analog

- `ldr(pin)` / `readLDR()` — raw ADC counts (NOT lux). Pin validated as analog input.
- `potentiometer(pin)` / `readPotentiometer()`.
- `readAnalog(pin)` — raw; `-1` on fail. Rejects non-analog pins (ESP8266: A0 only).
- `readVoltage(pin, dividerRatio)` — volts; `dividerRatio=(R1+R2)/R2`.
- ADC: Uno R3 10-bit/5 V, R4 14-bit/5 V, ESP32 12-bit/3.3 V, ESP8266 10-bit/3.3 V (A0 only).

## Digital

- `digitalInput(pin, pullup)` / `digitalOutput(pin)` / `setDigital(pin,v)` / `readDigital(pin)`.
- `button(pin, activeLow, pullup)` / `readButton()` / `buttonPressed()` — 50 ms non-blocking debounce.
- `buttonRose()` = press event, `buttonFell()` = release event (logical, edge-cleared).
- `pir(pin)` / `readPIR()` — HIGH = motion.
- `irObstacle(pin)` / `readIR()` — LOW = obstacle (common modules).

## Actuators

- `led(pin)` / `setLED(bool)` / `toggleLED()` / `setBrightness(0-255)` — brightness validates PWM pins and syncs LED state.
- `buzzer(pin)` / `beep(ms)` (active buzzers, blocking) / `beep(freqHz, ms)` (passive, blocking) / `buzzerOff()`.
- `relay(pin, activeHigh=true)` / `setRelay(bool)` — default OFF at setup.
- `motorBegin(in1,in2,enPin=-1)` / `motorSpeed(-255..255)` — requires external driver.
- `servoAttach(pin)` / `servoWrite(0-180)` / `servoDetach()` — needs `<Servo.h>`, else `MISSING_DEP`.

## Displays (optional)

- `oled(addr=0x3C,w=128,h=64)` — needs Adafruit SSD1306+GFX. Real init; `INIT_FAIL` on NACK.
- `lcd(addr,cols,rows)` — needs LiquidCrystal_I2C. Real init; `INIT_FAIL` on NACK.
- `displayClear()` / `displayText(line)` / `displayText(row,line)` — actually render.

## WiFi (ESP only)

- `wifiBegin(ssid,pass,timeoutMs)` — BLOCKING helper, `TIMEOUT` on failure, `UNSUPPORTED` on non-ESP.
- `wifiStart(ssid,pass,timeoutMs)` + `update()` + `wifiState()` — non-blocking; states IDLE/CONNECTING/CONNECTED/FAILED/UNSUPPORTED.
- `wifiConnected()` / `wifiRSSI()` / `wifiDisconnect()`.
