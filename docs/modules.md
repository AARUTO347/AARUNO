# AARUNO modules — MADE BY AARUTO

150 public functions covering 100+ modules. One active sensor per engine kind
in the singleton; for parallel same-kind sensors use the engine classes
(`AarunoAnalogEngine`, `AarunoDigitalEngine`, `AarunoEncoder`, ...).

## Analog sensors (setup fn + read fn)

| Module | Setup | Read | Unit |
|---|---|---|---|
| Generic analog | `analogSensor(pin)` | `readAnalogRaw()` | counts |
| LDR light | `ldr(pin)` | `readLDR()` | counts |
| Potentiometer | `potentiometer(pin)` | `readPotentiometer()` | counts |
| LM35 | `lm35(pin)` | `readTemperatureAnalog()` | °C |
| TMP36 | `tmp36(pin)` | `readTemperatureAnalog()` | °C |
| NTC thermistor | `thermistor(pin)` | `readTemperatureAnalog()` | °C |
| Soil moisture | `soilMoisture(pin)` | `readSoil()` | % |
| Rain | `rainSensor(pin)` | `readRain()` | % |
| Water level | `waterLevel(pin)` | `readWaterLevel()` | % |
| Sound | `soundSensor(pin)` | `readSound()` | % |
| Flex | `flexSensor(pin)` | `readFlex()` | % |
| FSR force/pressure | `forceSensor(pin)` | `readForce()` | % |
| Flame (analog out) | `flameAnalog(pin)` | `readFlameAnalog()` | % |
| MQ-2 smoke/gas | `gasMQ2(pin)` | `readGas()` | RS/RL ratio |
| MQ-3 alcohol | `gasMQ3(pin)` | `readGas()` | ratio |
| MQ-4 methane | `gasMQ4(pin)` | `readGas()` | ratio |
| MQ-5 LPG | `gasMQ5(pin)` | `readGas()` | ratio |
| MQ-6 LPG | `gasMQ6(pin)` | `readGas()` | ratio |
| MQ-7 CO | `gasMQ7(pin)` | `readGas()` | ratio |
| MQ-8 hydrogen | `gasMQ8(pin)` | `readGas()` | ratio |
| MQ-9 CO+CH4 | `gasMQ9(pin)` | `readGas()` | ratio |
| MQ-135 air quality | `gasMQ135(pin)` | `readGas()` | ratio |
| ACS712 current | `currentSensor(pin)` | `readCurrent()` | A |
| Any analog | `readAnalog(pin)` / `readVoltage(pin, ratio)` | — | counts / V |

Helpers: `readAnalogPercent()`, `readAnalogVoltage()`, `setThermistorParams()`,
`setCurrentParams()` (ACS712 5A=185, 20A=100, 30A=66 mV/A).

## Digital sensors (setup fn + read fn)

| Module | Setup | Read | Active |
|---|---|---|---|
| Generic digital | `digitalSensor(pin)` | `readDigitalSensor()` | high |
| PIR motion | `pir(pin)` | `readPIR()` | high |
| IR obstacle | `irObstacle(pin)` | `readIR()` | low |
| Line follower (TCRT5000) | `lineFollower(pin)` | `readLine()` | low |
| Flame (digital) | `flameSensor(pin)` | `readFlame()` | low |
| Vibration SW-420 | `vibrationSensor(pin)` | `readVibration()` | high |
| Tilt | `tiltSensor(pin)` | `readTilt()` | high |
| Touch TTP223 | `touchSensor(pin)` | `readTouch()` | high |
| Hall A3144/KY-003 | `hallSensor(pin)` | `readHall()` | low+pullup |
| Reed | `reedSwitch(pin)` | `readReed()` | low+pullup |
| Limit switch | `limitSwitch(pin)` | `readLimit()` | low+pullup |
| IR receiver (raw) | `irReceiver(pin)` | `readIRRemote()` | high (raw; no protocol decode) |
| Push button | `button(pin)` | `buttonPressed()` / `buttonRose()` press / `buttonFell()` release | low+pullup |

## Encoders, keypads, joysticks

- `rotaryEncoder(clk, dt, sw)` + `encoderCount()` / `encoderReset()` / `encoderButton()`
- `keypad4x4(r0,r1,r2,r3,c0,c1,c2,c3)` + `readKey()` → '1'-'9','0','A'-'D','*','#' (blocking single scan)
- `joystick(x,y,sw)` + `readJoyX()` / `readJoyY()` / `readJoyButton()`

## Ultrasonic / environment

- `ultrasonic(trig,echo)` + `readUltrasonic()` cm (BLOCKING ≤ timeout) / `readUltrasonicInches()`
- `dht11(pin)` / `dht22(pin)` + `readTemperature()` / `readHumidity()` (needs DHT lib)
- `ds18b20(pin)` + `readDS18B20()` °C (needs OneWire + DallasTemperature)
- `bmp280(addr)` + `readPressure()` hPa / `readBMPTemp()` °C (needs Adafruit BMP280)

## IMU + I2C (no external lib needed)

- `mpu6050(addr)` + `readAccel(x,y,z)` g / `readGyro(x,y,z)` dps / `readIMUTemp()` °C
  and `readAccelX()/Y/Z()`,`readGyroX()/Y/Z()` single-value forms.
- `i2cDevicePresent(addr)`, `i2cScan()`, `i2cScanCount()`, `i2cRead8(addr,reg)`,
  `i2cWrite8(addr,reg,val)`.

## Displays

- `oled(addr,w,h)` — Adafruit SSD1306+GFX; `displayClear()`, `displayText(line/row)`,
  `displayTextAt(row,col,text)`.
- `lcd(addr,cols,rows)` / `lcdI2C(...)` — LiquidCrystal I2C; plus the display
  helpers above and `displayBacklight(on)`.
- Auto-detect: `oledAuto()` probes 0x3C/0x3D, `lcdAuto()` probes 0x27/0x38
  (16x2 default), `autoScan()` returns the device count on the bus.

## Actuators

- LED: `led(pin)`, `setLED(bool)`, `toggleLED()`, `setBrightness(0-255)` (PWM pin)
- Buzzer: `buzzer(pin)`, `beep(ms)` active, `beep(freqHz,ms)` passive, `buzzerOff()`
- Relay: `relay(pin,activeHigh)`, `setRelay(bool)`
- Solenoid (via relay/MOSFET module): `solenoid(pin,activeHigh)`, `setSolenoid(bool)`
- Laser KY-008: `laserEmitter(pin)`, `setLaser(bool)`
- RGB LED (common cathode): `rgb(r,g,b)` + `setRGBRGB(r,g,b)` / `setRGB(bool)`
- PWM device: `pwmOut(pin)`, `writePWM(0-255)`
- DC motor (driver): `motorBegin(in1,in2,en)`, `motorSpeed(-255..255)`
- Stepper 28BYJ-48+ULN2003: `stepperAttach(in1..in4)`, `stepperSetRpm(1-20)`,
  `stepperStep(steps)` (BLOCKING per step; 2048 = 1 rev), `stepperRelease()`
- Servo: `servoAttach(pin)`, `servoWrite(0-180)`, `servoDetach()` (Servo.h)

## Board + system

- `begin()`, `update()`, `madeBy()` → "MADE BY AARUTO", `credits()`,
  `version()`, `boardName()`, `boardFamily()`, `lastError()`, `clearError()`,
  `errorString()`, `isValidDigitalPin/AnalogPin/PwmPin()`.

## WiFi + HTTP (ESP only)

- `wifiBegin(ssid,pass,ms)` blocking / `wifiStart(...)` non-blocking +
  `update()` + `wifiState()`; `wifiConnected()`, `wifiRSSI()`, `wifiDisconnect()`.
- `httpGet(url, Serial)` — returns HTTP code, streams body.

## Units honesty

Percent modules are raw-amount normalized (0-100 of ADC range), NOT calibrated
physical quantities. Gas ratio needs datasheet calibration for ppm. IR receiver
returns the pin level only — IR protocol decoding needs IRremote (optional).
