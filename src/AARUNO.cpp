// AARUNO v1.1.0 — MADE BY AARUTO
// Implementation of the AarunoClass singleton + named wrappers.
#include "AARUNO.h"
#include "utility/AarunoUtil.h"

#if defined(ARDUINO)
#include <Arduino.h>
#ifdef __has_include
#if __has_include(<Servo.h>)
#include <Servo.h>
#define AARUNO_HAS_SERVO 1
#endif
#endif
#else
// Host stubs (never used on device; keep translation unit parseable).
static unsigned long millis() { return 0; }
static void pinMode(int, int) {}
static void digitalWrite(int, int) {}
static int digitalRead(int) { return 0; }
static int analogRead(int) { return 0; }
static void analogWrite(int, int) {}
static void delay(unsigned long) {}
static void tone(int, unsigned int, unsigned long) {}
static void noTone(int) {}
#ifndef OUTPUT
#define OUTPUT 1
#endif
#ifndef INPUT
#define INPUT 0
#endif
#ifndef INPUT_PULLUP
#define INPUT_PULLUP 2
#endif
#ifndef HIGH
#define HIGH 1
#endif
#ifndef LOW
#define LOW 0
#endif
#endif

AarunoClass AARUNO;

AarunoClass::AarunoClass()
    : _begun(false), _err(AARUNO_NOT_INITIALIZED), _dbg(nullptr),
      _ldrPin(-1), _potPin(-1), _ledPin(-1), _buzzerPin(-1), _relayPin(-1),
      _solenoidPin(-1), _laserPin(-1), _pwmPin(-1), _joyX(-1), _joyY(-1),
      _joySw(-1), _relayActiveHigh(true), _solenoidActiveHigh(true),
      _ledState(false), _relayState(false), _solenoidState(false),
      _rgbSetUp(false), _motorIn1(-1), _motorIn2(-1), _motorEn(-1),
      _servoPin(-1), _servo(nullptr) {
  _rgbPins[0] = _rgbPins[1] = _rgbPins[2] = 255;
  _step = nullptr;
}

// ---------------- Core ----------------

void AarunoClass::begin() {
  _begun = true;
  _err = AARUNO_OK;
#if defined(ARDUINO)
#if defined(AARUNO_BOARD_ESP32)
  analogReadResolution(12);
#elif defined(AARUNO_BOARD_UNO_R4_MINIMA) || defined(AARUNO_BOARD_UNO_R4_WIFI)
  analogReadResolution(14);
#endif
#endif
}

void AarunoClass::update() {
  if (!_begun) return;
#if defined(ARDUINO)
  _btn.update(millis());
  _enc.update(millis());
#endif
  _wifi.update();  // services non-blocking wifiStart() attempts
}

void AarunoClass::dbg(const char *msg) {
#if AARUNO_ENABLE_DEBUG
  if (_dbg && msg) _dbg->print(msg);
#else
  (void)msg;
#endif
}

AarunoFamily AarunoClass::boardFamily() const { return (AarunoFamily)AARUNO_BOARD_FAMILY; }
const char *AarunoClass::boardName() const { return AARUNO_BOARD_NAME; }
const char *AarunoClass::errorString() const { return errorString(_err); }
const char *AarunoClass::errorString(AarunoError e) { return aarunoErrorName((int)e); }

bool AarunoClass::isValidDigitalPin(uint8_t pin, bool needOutput) {
  return aarunoValidateDigitalPin(pin, needOutput) == AARUNO_OK;
}
bool AarunoClass::isValidAnalogPin(int pin) {
  return aarunoValidateAnalogPin(pin) == AARUNO_OK;
}
bool AarunoClass::isValidPwmPin(uint8_t pin) {
  return aarunoValidatePwmPin(pin) == AARUNO_OK;
}

// ---------------- Shared config helpers ----------------

AarunoError AarunoClass::cfgAnalog(uint8_t pin, AarunoAnalogKind kind) {
  if (!_begun) return setErr(AARUNO_NOT_INITIALIZED);
  return setErr(_aeng.begin(pin, kind));
}

AarunoError AarunoClass::cfgDigital(uint8_t pin, bool activeLow, bool pullup) {
  if (!_begun) return setErr(AARUNO_NOT_INITIALIZED);
  return setErr(_deng.begin(pin, activeLow, pullup));
}

// ---------------- Ultrasonic ----------------

AarunoError AarunoClass::ultrasonic(uint8_t trigPin, uint8_t echoPin) {
  if (!_begun) return setErr(AARUNO_NOT_INITIALIZED);
  return setErr(_sonic.begin(trigPin, echoPin));
}

AarunoError AarunoClass::setUltrasonicTimeout(unsigned long us) {
  if (!_sonic.configured()) return setErr(AARUNO_NOT_INITIALIZED);
  _sonic.setTimeout(us ? us : AARUNO_ULTRASONIC_TIMEOUT_US);
  return setErr(AARUNO_OK);
}

float AarunoClass::readUltrasonic() {
  float cm = AARUNO_INVALID_READING;
  readUltrasonic(cm);
  return cm;
}

bool AarunoClass::readUltrasonic(float &cm) {
  cm = AARUNO_INVALID_READING;
  if (!_begun) { setErr(AARUNO_NOT_INITIALIZED); return false; }
  if (!_sonic.configured()) { setErr(AARUNO_NOT_INITIALIZED); return false; }
  bool ok = _sonic.readCm(cm);
  setErr(ok ? AARUNO_OK : _sonic.lastError());
  return ok;
}

float AarunoClass::readUltrasonicInches() {
  if (!_begun || !_sonic.configured()) {
    setErr(AARUNO_NOT_INITIALIZED);
    return AARUNO_INVALID_READING;
  }
  float cm = AARUNO_INVALID_READING;
  if (!readUltrasonic(cm)) return AARUNO_INVALID_READING;
  return (float)(cm * 0.393700787);
}

// ---------------- DHT ----------------

AarunoError AarunoClass::dht11(uint8_t pin) {
  if (!_begun) return setErr(AARUNO_NOT_INITIALIZED);
  return setErr(_dht.begin(pin, 11));
}

AarunoError AarunoClass::dht22(uint8_t pin) {
  if (!_begun) return setErr(AARUNO_NOT_INITIALIZED);
  return setErr(_dht.begin(pin, 22));
}

float AarunoClass::readTemperature() {
  float t = AARUNO_INVALID_READING, h = AARUNO_INVALID_READING;
  readDHT(t, h);
  return t;
}

float AarunoClass::readHumidity() {
  float t = AARUNO_INVALID_READING, h = AARUNO_INVALID_READING;
  readDHT(t, h);
  return h;
}

bool AarunoClass::readDHT(float &t, float &h) {
  t = AARUNO_INVALID_READING;
  h = AARUNO_INVALID_READING;
  if (!_begun) { setErr(AARUNO_NOT_INITIALIZED); return false; }
  bool ok = _dht.read(t, h);
  setErr(ok ? AARUNO_OK : _dht.lastError());
  return ok;
}

bool AarunoClass::dhtFresh() const { return _dht.fresh(); }

unsigned long AarunoClass::dhtAgeMs() const {
#if defined(ARDUINO)
  return _dht.ageMs(millis());
#else
  return _dht.ageMs(0);
#endif
}

// ---------------- Analog named wrappers ----------------

AarunoError AarunoClass::analogSensor(uint8_t pin) { return cfgAnalog(pin, AARUNO_AN_RAW); }
AarunoError AarunoClass::ldr(uint8_t pin) {
  AarunoError e = cfgAnalog(pin, AARUNO_AN_RAW);
  if (e == AARUNO_OK) _ldrPin = (int)pin;
  return e;
}
AarunoError AarunoClass::potentiometer(uint8_t pin) {
  AarunoError e = cfgAnalog(pin, AARUNO_AN_RAW);
  if (e == AARUNO_OK) _potPin = (int)pin;
  return e;
}
AarunoError AarunoClass::lm35(uint8_t pin) { return cfgAnalog(pin, AARUNO_AN_LM35); }
AarunoError AarunoClass::tmp36(uint8_t pin) { return cfgAnalog(pin, AARUNO_AN_TMP36); }
AarunoError AarunoClass::thermistor(uint8_t pin) { return cfgAnalog(pin, AARUNO_AN_NTC); }
AarunoError AarunoClass::soilMoisture(uint8_t pin) { return cfgAnalog(pin, AARUNO_AN_PLAIN_PCT); }
AarunoError AarunoClass::rainSensor(uint8_t pin) { return cfgAnalog(pin, AARUNO_AN_PLAIN_PCT); }
AarunoError AarunoClass::waterLevel(uint8_t pin) { return cfgAnalog(pin, AARUNO_AN_PLAIN_PCT); }
AarunoError AarunoClass::soundSensor(uint8_t pin) { return cfgAnalog(pin, AARUNO_AN_PLAIN_PCT); }
AarunoError AarunoClass::flexSensor(uint8_t pin) { return cfgAnalog(pin, AARUNO_AN_PLAIN_PCT); }
AarunoError AarunoClass::forceSensor(uint8_t pin) { return cfgAnalog(pin, AARUNO_AN_PLAIN_PCT); }
AarunoError AarunoClass::flameAnalog(uint8_t pin) { return cfgAnalog(pin, AARUNO_AN_PLAIN_PCT); }
AarunoError AarunoClass::gasMQ2(uint8_t pin) { return cfgAnalog(pin, AARUNO_AN_MQ); }
AarunoError AarunoClass::gasMQ3(uint8_t pin) { return cfgAnalog(pin, AARUNO_AN_MQ); }
AarunoError AarunoClass::gasMQ4(uint8_t pin) { return cfgAnalog(pin, AARUNO_AN_MQ); }
AarunoError AarunoClass::gasMQ5(uint8_t pin) { return cfgAnalog(pin, AARUNO_AN_MQ); }
AarunoError AarunoClass::gasMQ6(uint8_t pin) { return cfgAnalog(pin, AARUNO_AN_MQ); }
AarunoError AarunoClass::gasMQ7(uint8_t pin) { return cfgAnalog(pin, AARUNO_AN_MQ); }
AarunoError AarunoClass::gasMQ8(uint8_t pin) { return cfgAnalog(pin, AARUNO_AN_MQ); }
AarunoError AarunoClass::gasMQ9(uint8_t pin) { return cfgAnalog(pin, AARUNO_AN_MQ); }
AarunoError AarunoClass::gasMQ135(uint8_t pin) { return cfgAnalog(pin, AARUNO_AN_MQ); }
AarunoError AarunoClass::currentSensor(uint8_t pin) { return cfgAnalog(pin, AARUNO_AN_CURRENT); }

void AarunoClass::setThermistorParams(float seriesR, float beta, float r0,
                                      float t0C) {
  _aeng.setNtcParams(seriesR, beta, r0, t0C);
}

void AarunoClass::setCurrentParams(float mvPerAmp, float zeroMv) {
  _aeng.setCurrentParams(mvPerAmp, zeroMv);
}

int AarunoClass::readAnalogRaw() {
  int v = _aeng.readRaw();
  setErr(v < 0 ? _aeng.lastError() : AARUNO_OK);
  return v;
}

float AarunoClass::readAnalogPercent() {
  float v = _aeng.readPercent();
  setErr(isnan(v) ? _aeng.lastError() : AARUNO_OK);
  return v;
}

float AarunoClass::readAnalogVoltage() {
  float v = _aeng.readVolts();
  setErr(isnan(v) ? _aeng.lastError() : AARUNO_OK);
  return v;
}

float AarunoClass::readAnalogValue() {
  float v = _aeng.readSpecial();
  setErr(isnan(v) ? _aeng.lastError() : AARUNO_OK);
  return v;
}

int AarunoClass::readLDR() {
  if (_ldrPin < 0) { setErr(AARUNO_NOT_INITIALIZED); return -1; }
  return readAnalog((uint8_t)_ldrPin);
}

int AarunoClass::readPotentiometer() {
  if (_potPin < 0) { setErr(AARUNO_NOT_INITIALIZED); return -1; }
  return readAnalog((uint8_t)_potPin);
}

float AarunoClass::readTemperatureAnalog() { return readAnalogValue(); }
float AarunoClass::readGas() { return readAnalogValue(); }
float AarunoClass::readCurrent() { return readAnalogValue(); }
float AarunoClass::readSoil() { return readAnalogValue(); }
float AarunoClass::readRain() { return readAnalogValue(); }
float AarunoClass::readWaterLevel() { return readAnalogValue(); }
float AarunoClass::readSound() { return readAnalogValue(); }
float AarunoClass::readFlex() { return readAnalogValue(); }
float AarunoClass::readForce() { return readAnalogValue(); }
float AarunoClass::readFlameAnalog() { return readAnalogValue(); }

int AarunoClass::readAnalog(uint8_t pin) {
  if (!_begun) { setErr(AARUNO_NOT_INITIALIZED); return -1; }
  AarunoError e = aarunoValidateAnalogPin((int)pin);
  if (e != AARUNO_OK) { setErr(e); return -1; }
#if defined(ARDUINO)
  setErr(AARUNO_OK);
  return analogRead(pin);
#else
  (void)pin;
  setErr(AARUNO_UNSUPPORTED);
  return -1;
#endif
}

float AarunoClass::readVoltage(uint8_t pin, float dividerRatio) {
  if (dividerRatio <= 0) { setErr(AARUNO_INVALID_CONFIG); return AARUNO_INVALID_READING; }
  int raw = readAnalog(pin);
  if (raw < 0) return AARUNO_INVALID_READING;  // _err already set
  long mv = aarunoAdcToMillivolts(raw, AARUNO_ADC_MAX, AARUNO_ADC_VREF, dividerRatio);
  if (mv < 0) { setErr(AARUNO_INVALID_CONFIG); return AARUNO_INVALID_READING; }
  setErr(AARUNO_OK);
  return (float)mv / 1000.0f;
}

// ---------------- Digital named wrappers ----------------

AarunoError AarunoClass::digitalSensor(uint8_t pin, bool activeLow, bool pullup) {
  return cfgDigital(pin, activeLow, pullup);
}
AarunoError AarunoClass::pir(uint8_t pin) { return cfgDigital(pin, false, false); }
AarunoError AarunoClass::irObstacle(uint8_t pin) { return cfgDigital(pin, true, false); }
AarunoError AarunoClass::lineFollower(uint8_t pin) { return cfgDigital(pin, true, false); }
AarunoError AarunoClass::flameSensor(uint8_t pin) { return cfgDigital(pin, true, false); }
AarunoError AarunoClass::vibrationSensor(uint8_t pin) { return cfgDigital(pin, false, false); }
AarunoError AarunoClass::tiltSensor(uint8_t pin) { return cfgDigital(pin, false, false); }
AarunoError AarunoClass::touchSensor(uint8_t pin) { return cfgDigital(pin, false, false); }
AarunoError AarunoClass::hallSensor(uint8_t pin) { return cfgDigital(pin, true, true); }
AarunoError AarunoClass::reedSwitch(uint8_t pin) { return cfgDigital(pin, true, true); }
AarunoError AarunoClass::limitSwitch(uint8_t pin, bool pullup, bool activeLow) {
  return cfgDigital(pin, activeLow, pullup);
}
AarunoError AarunoClass::irReceiver(uint8_t pin) { return cfgDigital(pin, false, false); }

bool AarunoClass::readDigitalSensor() {
  bool t = _deng.triggered();
  setErr(t ? AARUNO_OK : _deng.lastError());
  return _deng.lastError() == AARUNO_OK ? t : false;
}
bool AarunoClass::readPIR() { return readDigitalSensor(); }
bool AarunoClass::readIR() { return readDigitalSensor(); }
bool AarunoClass::readLine() { return readDigitalSensor(); }
bool AarunoClass::readFlame() { return readDigitalSensor(); }
bool AarunoClass::readVibration() { return readDigitalSensor(); }
bool AarunoClass::readTilt() { return readDigitalSensor(); }
bool AarunoClass::readTouch() { return readDigitalSensor(); }
bool AarunoClass::readHall() { return readDigitalSensor(); }
bool AarunoClass::readReed() { return readDigitalSensor(); }
bool AarunoClass::readLimit() { return readDigitalSensor(); }
bool AarunoClass::readIRRemote() { return readDigitalSensor(); }

// ---------------- Button ----------------

AarunoError AarunoClass::button(uint8_t pin, bool activeLow, bool pullup) {
  if (!_begun) return setErr(AARUNO_NOT_INITIALIZED);
  return setErr(_btn.begin(pin, activeLow, pullup));
}

int AarunoClass::readButton() {
  if (!_begun || !_btn.begun()) { setErr(AARUNO_NOT_INITIALIZED); return -1; }
  setErr(AARUNO_OK);
  return _btn.read();
}

bool AarunoClass::buttonPressed() {
  if (!_begun) { setErr(AARUNO_NOT_INITIALIZED); return false; }
  setErr(AARUNO_OK);
  return _btn.pressed();
}

bool AarunoClass::buttonRose() {
  if (!_begun) { setErr(AARUNO_NOT_INITIALIZED); return false; }
  setErr(AARUNO_OK);
  return _btn.rose();
}

bool AarunoClass::buttonFell() {
  if (!_begun) { setErr(AARUNO_NOT_INITIALIZED); return false; }
  setErr(AARUNO_OK);
  return _btn.fell();
}

// ---------------- Encoder ----------------

AarunoError AarunoClass::rotaryEncoder(uint8_t clk, uint8_t dt, int swPin) {
  if (!_begun) return setErr(AARUNO_NOT_INITIALIZED);
  return setErr(_enc.begin(clk, dt, swPin));
}

long AarunoClass::encoderCount() {
  if (!_enc.configured()) { setErr(AARUNO_NOT_INITIALIZED); return 0; }
  setErr(AARUNO_OK);
  return _enc.count();
}

void AarunoClass::encoderReset() { _enc.reset(); }

bool AarunoClass::encoderButton() {
  setErr(AARUNO_OK);
  return _enc.buttonPressed();
}

// ---------------- Keypad ----------------

AarunoError AarunoClass::keypad4x4(uint8_t r0, uint8_t r1, uint8_t r2,
                                   uint8_t r3, uint8_t c0, uint8_t c1,
                                   uint8_t c2, uint8_t c3) {
  if (!_begun) return setErr(AARUNO_NOT_INITIALIZED);
  return setErr(_keypad.begin(r0, r1, r2, r3, c0, c1, c2, c3));
}

char AarunoClass::readKey() {
  char k = _keypad.readKey();
  setErr(k == 0 ? _keypad.lastError() : AARUNO_OK);
  return k;
}

// ---------------- Joystick ----------------

AarunoError AarunoClass::joystick(uint8_t xPin, uint8_t yPin, int swPin) {
  if (!_begun) return setErr(AARUNO_NOT_INITIALIZED);
  AarunoError e = aarunoValidateAnalogPin((int)xPin);
  if (e != AARUNO_OK) return setErr(e);
  e = aarunoValidateAnalogPin((int)yPin);
  if (e != AARUNO_OK) return setErr(e);
  if (xPin == yPin) return setErr(AARUNO_INVALID_CONFIG);
  if (swPin >= 0) {
    e = aarunoValidateDigitalPin((uint8_t)swPin, false);
    if (e != AARUNO_OK) return setErr(e);
#if defined(ARDUINO)
    pinMode((uint8_t)swPin, INPUT_PULLUP);
#endif
  }
  _joyX = (int)xPin;
  _joyY = (int)yPin;
  _joySw = swPin;
  return setErr(AARUNO_OK);
}

int AarunoClass::readJoyX() { return readAnalog((uint8_t)_joyX); }
int AarunoClass::readJoyY() { return readAnalog((uint8_t)_joyY); }
bool AarunoClass::readJoyButton() {
  if (_joySw < 0) { setErr(AARUNO_NOT_INITIALIZED); return false; }
#if defined(ARDUINO)
  setErr(AARUNO_OK);
  return digitalRead((uint8_t)_joySw) == LOW;
#else
  setErr(AARUNO_UNSUPPORTED);
  return false;
#endif
}

// ---------------- MPU6050 ----------------

AarunoError AarunoClass::mpu6050(uint8_t addr) {
  if (!_begun) return setErr(AARUNO_NOT_INITIALIZED);
  return setErr(_mpu.begin(addr));
}

bool AarunoClass::readAccel(float &x, float &y, float &z) {
  bool ok = _mpu.readAccel(x, y, z);
  setErr(ok ? AARUNO_OK : _mpu.lastError());
  return ok;
}

bool AarunoClass::readGyro(float &x, float &y, float &z) {
  bool ok = _mpu.readGyro(x, y, z);
  setErr(ok ? AARUNO_OK : _mpu.lastError());
  return ok;
}

float AarunoClass::readIMUTemp() {
  float c = AARUNO_INVALID_READING;
  if (!_mpu.readTempC(c)) setErr(_mpu.lastError());
  else setErr(AARUNO_OK);
  return c;
}

float AarunoClass::readAccelX() { float x,y,z; if (!readAccel(x,y,z)) return AARUNO_INVALID_READING; return x; }
float AarunoClass::readAccelY() { float x,y,z; if (!readAccel(x,y,z)) return AARUNO_INVALID_READING; return y; }
float AarunoClass::readAccelZ() { float x,y,z; if (!readAccel(x,y,z)) return AARUNO_INVALID_READING; return z; }
float AarunoClass::readGyroX() { float x,y,z; if (!readGyro(x,y,z)) return AARUNO_INVALID_READING; return x; }
float AarunoClass::readGyroY() { float x,y,z; if (!readGyro(x,y,z)) return AARUNO_INVALID_READING; return y; }
float AarunoClass::readGyroZ() { float x,y,z; if (!readGyro(x,y,z)) return AARUNO_INVALID_READING; return z; }

// ---------------- Optional I2C extras ----------------

AarunoError AarunoClass::ds18b20(uint8_t pin) {
  if (!_begun) return setErr(AARUNO_NOT_INITIALIZED);
  return setErr(_ds.begin(pin));
}

float AarunoClass::readDS18B20() {
  float c = AARUNO_INVALID_READING;
  if (!_ds.readC(c)) setErr(_ds.lastError());
  else setErr(AARUNO_OK);
  return c;
}

AarunoError AarunoClass::bmp280(uint8_t addr) {
  if (!_begun) return setErr(AARUNO_NOT_INITIALIZED);
  return setErr(_bmp.begin(addr));
}

float AarunoClass::readPressure() {
  float p = AARUNO_INVALID_READING;
  if (!_bmp.readPressure(p)) setErr(_bmp.lastError());
  else setErr(AARUNO_OK);
  return p;
}

float AarunoClass::readBMPTemp() {
  float c = AARUNO_INVALID_READING;
  if (!_bmp.readTempC(c)) setErr(_bmp.lastError());
  else setErr(AARUNO_OK);
  return c;
}

// ---------------- Generic I2C ----------------

bool AarunoClass::i2cDevicePresent(uint8_t addr) {
  return AarunoI2C::isDevicePresent(addr);
}

int AarunoClass::i2cScan() { return AarunoI2C::scanFirst(); }

uint8_t AarunoClass::i2cScanCount() { return AarunoI2C::scanCount(); }

int AarunoClass::i2cRead8(uint8_t addr, uint8_t reg) {
  return AarunoI2C::readReg8(addr, reg);
}

bool AarunoClass::i2cWrite8(uint8_t addr, uint8_t reg, uint8_t value) {
  return AarunoI2C::writeReg8(addr, reg, value);
}

uint8_t AarunoClass::autoScan() {
  return AarunoI2C::scanCount();
}

AarunoError AarunoClass::oledAuto() {
  // Common SSD1306 addresses: 0x3C (most), 0x3D (some 128x64).
  static const uint8_t addrs[2] = {0x3C, 0x3D};
  if (!_begun) return setErr(AARUNO_NOT_INITIALIZED);
  for (int i = 0; i < 2; i++) {
    if (AarunoI2C::isDevicePresent(addrs[i])) {
      return setErr(_disp.oled(addrs[i]));
    }
  }
  return setErr(AARUNO_INIT_FAIL);  // no OLED on the bus
}

AarunoError AarunoClass::lcdAuto() {
  // Common HD44780 backpack addresses: 0x27, 0x38 (PCF8574 vs PCF8574A).
  static const uint8_t addrs[2] = {0x27, 0x38};
  if (!_begun) return setErr(AARUNO_NOT_INITIALIZED);
  for (int i = 0; i < 2; i++) {
    if (AarunoI2C::isDevicePresent(addrs[i])) {
      return setErr(_disp.lcd(addrs[i], 16, 2));  // safest default 16x2
    }
  }
  return setErr(AARUNO_INIT_FAIL);  // no LCD on the bus
}

// ---------------- Displays ----------------

AarunoError AarunoClass::oled(uint8_t addr, uint8_t w, uint8_t h) {
  return setErr(_disp.oled(addr, w, h));
}

AarunoError AarunoClass::lcd(uint8_t addr, uint8_t cols, uint8_t rows) {
  return setErr(_disp.lcd(addr, cols, rows));
}

AarunoError AarunoClass::lcdI2C(uint8_t addr, uint8_t cols, uint8_t rows) {
  return setErr(_disp.lcd(addr, cols, rows));
}

AarunoError AarunoClass::displayClear() { return setErr(_disp.clear()); }
AarunoError AarunoClass::displayText(const char *line) { return setErr(_disp.text(line)); }
AarunoError AarunoClass::displayText(int row, const char *line) {
  return setErr(_disp.text(row, line));
}
AarunoError AarunoClass::displayTextAt(int row, int col, const char *line) {
  return setErr(_disp.textAt(row, col, line));
}
AarunoError AarunoClass::displayBacklight(bool on) {
  return setErr(_disp.backlight(on));
}

// ---------------- Actuators ----------------

AarunoError AarunoClass::led(uint8_t pin) {
  if (!_begun) return setErr(AARUNO_NOT_INITIALIZED);
  AarunoError e = aarunoValidateDigitalPin(pin, true);
  if (e != AARUNO_OK) return setErr(e);
  _ledPin = (int)pin;
  _ledState = false;
#if defined(ARDUINO)
  pinMode(pin, OUTPUT);
  digitalWrite(pin, LOW);
#endif
  return setErr(AARUNO_OK);
}

AarunoError AarunoClass::setLED(bool on) {
  if (_ledPin < 0) return setErr(AARUNO_NOT_INITIALIZED);
  _ledState = on;
#if defined(ARDUINO)
  digitalWrite(_ledPin, on ? HIGH : LOW);
#endif
  return setErr(AARUNO_OK);
}

AarunoError AarunoClass::toggleLED() { return setLED(!_ledState); }

AarunoError AarunoClass::setBrightness(uint8_t value) {
  if (_ledPin < 0) return setErr(AARUNO_NOT_INITIALIZED);
  AarunoError e = aarunoValidatePwmPin((uint8_t)_ledPin);
  if (e != AARUNO_OK) return setErr(e);
  _ledState = (value > 0);
#if defined(ARDUINO)
  analogWrite(_ledPin, (int)value);
#endif
  return setErr(AARUNO_OK);
}

AarunoError AarunoClass::buzzer(uint8_t pin) {
  if (!_begun) return setErr(AARUNO_NOT_INITIALIZED);
  AarunoError e = aarunoValidateDigitalPin(pin, true);
  if (e != AARUNO_OK) return setErr(e);
  _buzzerPin = (int)pin;
#if defined(ARDUINO)
  pinMode(pin, OUTPUT);
  digitalWrite(pin, LOW);
#endif
  return setErr(AARUNO_OK);
}

AarunoError AarunoClass::beep(unsigned long durationMs) {
  if (_buzzerPin < 0) return setErr(AARUNO_NOT_INITIALIZED);
#if defined(ARDUINO)
  digitalWrite(_buzzerPin, HIGH);
  delay(durationMs);
  digitalWrite(_buzzerPin, LOW);
#endif
  dbg("beep");
  return setErr(AARUNO_OK);
}

AarunoError AarunoClass::beep(unsigned int freqHz, unsigned long durationMs) {
  if (_buzzerPin < 0) return setErr(AARUNO_NOT_INITIALIZED);
  if (freqHz == 0) return setErr(AARUNO_INVALID_CONFIG);
#if defined(ARDUINO)
  tone(_buzzerPin, freqHz, durationMs);
  delay(durationMs);
  noTone(_buzzerPin);
#endif
  return setErr(AARUNO_OK);
}

AarunoError AarunoClass::buzzerOff() {
  if (_buzzerPin < 0) return setErr(AARUNO_NOT_INITIALIZED);
#if defined(ARDUINO)
  noTone(_buzzerPin);
  digitalWrite(_buzzerPin, LOW);
#endif
  return setErr(AARUNO_OK);
}

AarunoError AarunoClass::relay(uint8_t pin, bool activeHigh) {
  if (!_begun) return setErr(AARUNO_NOT_INITIALIZED);
  AarunoError e = aarunoValidateDigitalPin(pin, true);
  if (e != AARUNO_OK) return setErr(e);
  _relayPin = (int)pin;
  _relayActiveHigh = activeHigh;
  _relayState = false;
#if defined(ARDUINO)
  pinMode(pin, OUTPUT);
  digitalWrite(pin, activeHigh ? LOW : HIGH);  // default OFF
#endif
  return setErr(AARUNO_OK);
}

AarunoError AarunoClass::setRelay(bool on) {
  if (_relayPin < 0) return setErr(AARUNO_NOT_INITIALIZED);
  _relayState = on;
#if defined(ARDUINO)
  bool level = _relayActiveHigh ? on : !on;
  digitalWrite(_relayPin, level ? HIGH : LOW);
#endif
  return setErr(AARUNO_OK);
}

AarunoError AarunoClass::solenoid(uint8_t pin, bool activeHigh) {
  // Solenoids are driven through a relay/MOSFET module, never from GPIO.
  AarunoError e = relay(pin, activeHigh);
  if (e == AARUNO_OK) { _solenoidPin = _relayPin; _solenoidActiveHigh = activeHigh; _solenoidState = false; }
  return setErr(e);
}

AarunoError AarunoClass::setSolenoid(bool on) {
  if (_solenoidPin < 0) return setErr(AARUNO_NOT_INITIALIZED);
  _solenoidState = on;
  return setRelay(on);
}

AarunoError AarunoClass::laserEmitter(uint8_t pin) {
  if (!_begun) return setErr(AARUNO_NOT_INITIALIZED);
  AarunoError e = aarunoValidateDigitalPin(pin, true);
  if (e != AARUNO_OK) return setErr(e);
  _laserPin = (int)pin;
#if defined(ARDUINO)
  pinMode(pin, OUTPUT);
  digitalWrite(pin, LOW);
#endif
  return setErr(AARUNO_OK);
}

AarunoError AarunoClass::setLaser(bool on) {
  if (_laserPin < 0) return setErr(AARUNO_NOT_INITIALIZED);
#if defined(ARDUINO)
  digitalWrite(_laserPin, on ? HIGH : LOW);
#endif
  return setErr(AARUNO_OK);
}

AarunoError AarunoClass::rgb(uint8_t rPin, uint8_t gPin, uint8_t bPin) {
  if (!_begun) return setErr(AARUNO_NOT_INITIALIZED);
  if (rPin == gPin || gPin == bPin || rPin == bPin) return setErr(AARUNO_INVALID_CONFIG);
  AarunoError e = aarunoValidatePwmPin(rPin);
  if (e != AARUNO_OK) return setErr(e);
  e = aarunoValidatePwmPin(gPin);
  if (e != AARUNO_OK) return setErr(e);
  e = aarunoValidatePwmPin(bPin);
  if (e != AARUNO_OK) return setErr(e);
  _rgbPins[0] = rPin; _rgbPins[1] = gPin; _rgbPins[2] = bPin;
  _rgbSetUp = true;
#if defined(ARDUINO)
  pinMode(rPin, OUTPUT); pinMode(gPin, OUTPUT); pinMode(bPin, OUTPUT);
  analogWrite(rPin, 0); analogWrite(gPin, 0); analogWrite(bPin, 0);
#endif
  return setErr(AARUNO_OK);
}

AarunoError AarunoClass::setRGBRGB(uint8_t r, uint8_t g, uint8_t b) {
  if (!_rgbSetUp) return setErr(AARUNO_NOT_INITIALIZED);
#if defined(ARDUINO)
  analogWrite(_rgbPins[0], r);
  analogWrite(_rgbPins[1], g);
  analogWrite(_rgbPins[2], b);
#endif
  return setErr(AARUNO_OK);
}

AarunoError AarunoClass::setRGB(bool on) {
  if (!_rgbSetUp) return setErr(AARUNO_NOT_INITIALIZED);
  return setRGBRGB(on ? 255 : 0, on ? 255 : 0, on ? 255 : 0);
}

AarunoError AarunoClass::pwmOut(uint8_t pin) {
  if (!_begun) return setErr(AARUNO_NOT_INITIALIZED);
  AarunoError e = aarunoValidatePwmPin(pin);
  if (e != AARUNO_OK) return setErr(e);
  _pwmPin = (int)pin;
#if defined(ARDUINO)
  pinMode(pin, OUTPUT);
  analogWrite(pin, 0);
#endif
  return setErr(AARUNO_OK);
}

AarunoError AarunoClass::writePWM(uint8_t value) {
  if (_pwmPin < 0) return setErr(AARUNO_NOT_INITIALIZED);
#if defined(ARDUINO)
  analogWrite(_pwmPin, (int)value);
#endif
  return setErr(AARUNO_OK);
}

AarunoError AarunoClass::motorBegin(uint8_t in1, uint8_t in2, int enPin) {
  if (!_begun) return setErr(AARUNO_NOT_INITIALIZED);
  if (in1 == in2) return setErr(AARUNO_INVALID_CONFIG);
  AarunoError e1 = aarunoValidateDigitalPin(in1, true);
  AarunoError e2 = aarunoValidateDigitalPin(in2, true);
  if (e1 != AARUNO_OK) return setErr(e1);
  if (e2 != AARUNO_OK) return setErr(e2);
  if (enPin >= 0) {
    if (enPin == (int)in1 || enPin == (int)in2) return setErr(AARUNO_INVALID_CONFIG);
    AarunoError e3 = aarunoValidatePwmPin((uint8_t)enPin);
    if (e3 != AARUNO_OK) return setErr(e3);
  }
  _motorIn1 = (int)in1;
  _motorIn2 = (int)in2;
  _motorEn = enPin;
#if defined(ARDUINO)
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  if (enPin >= 0) pinMode((uint8_t)enPin, OUTPUT);
#endif
  return setErr(AARUNO_OK);
}

AarunoError AarunoClass::motorSpeed(int speed) {
  if (_motorIn1 < 0) return setErr(AARUNO_NOT_INITIALIZED);
  if (speed > 255) speed = 255;
  if (speed < -255) speed = -255;
#if defined(ARDUINO)
  if (speed > 0) {
    digitalWrite(_motorIn1, HIGH);
    digitalWrite(_motorIn2, LOW);
  } else if (speed < 0) {
    digitalWrite(_motorIn1, LOW);
    digitalWrite(_motorIn2, HIGH);
  } else {
    digitalWrite(_motorIn1, LOW);
    digitalWrite(_motorIn2, LOW);
  }
  if (_motorEn >= 0) analogWrite((uint8_t)_motorEn, abs(speed));
#endif
  return setErr(AARUNO_OK);
}

AarunoError AarunoClass::stepperAttach(uint8_t in1, uint8_t in2, uint8_t in3,
                                       uint8_t in4) {
  if (!_begun) return setErr(AARUNO_NOT_INITIALIZED);
  if (!_step) _step = new AarunoStepper();
  AarunoError e = _step->begin(in1, in2, in3, in4);
  if (e != AARUNO_OK) { setErr(e); return e; }
  _step->setRpm(15.0);
  return setErr(AARUNO_OK);
}

AarunoError AarunoClass::stepperSetRpm(double rpm) {
  if (!_step) return setErr(AARUNO_NOT_INITIALIZED);
  _step->setRpm(rpm);
  return setErr(AARUNO_OK);
}

AarunoError AarunoClass::stepperStep(long steps) {
  if (!_step) return setErr(AARUNO_NOT_INITIALIZED);
  // BLOCKING by design: moves step-by-step at the set RPM.
  return setErr(_step->step(steps));
}

AarunoError AarunoClass::stepperRelease() {
  if (!_step) return setErr(AARUNO_NOT_INITIALIZED);
  _step->release();
  return setErr(AARUNO_OK);
}

AarunoError AarunoClass::servoAttach(uint8_t pin) {
#if defined(AARUNO_HAS_SERVO) && defined(ARDUINO)
  if (!_begun) return setErr(AARUNO_NOT_INITIALIZED);
  AarunoError e = aarunoValidateDigitalPin(pin, true);
  if (e != AARUNO_OK) return setErr(e);
  static Servo s;
  s.attach(pin);
  if (!s.attached()) return setErr(AARUNO_INIT_FAIL);
  _servo = &s;
  _servoPin = (int)pin;
  return setErr(AARUNO_OK);
#else
  (void)pin;
  return setErr(AARUNO_MISSING_DEP);
#endif
}

AarunoError AarunoClass::servoWrite(int angleDeg) {
#if defined(AARUNO_HAS_SERVO) && defined(ARDUINO)
  if (!_servo || _servoPin < 0) return setErr(AARUNO_NOT_INITIALIZED);
  if (angleDeg < 0) angleDeg = 0;
  if (angleDeg > 180) angleDeg = 180;
  ((Servo *)_servo)->write(angleDeg);
  return setErr(AARUNO_OK);
#else
  (void)angleDeg;
  return setErr(AARUNO_MISSING_DEP);
#endif
}

AarunoError AarunoClass::servoDetach() {
#if defined(AARUNO_HAS_SERVO) && defined(ARDUINO)
  if (_servo) ((Servo *)_servo)->detach();
  _servoPin = -1;
  return setErr(AARUNO_OK);
#else
  return setErr(AARUNO_MISSING_DEP);
#endif
}

// ---------------- WiFi / HTTP ----------------

AarunoError AarunoClass::wifiBegin(const char *ssid, const char *pass,
                                   unsigned long timeoutMs) {
  return setErr(_wifi.begin(ssid, pass, timeoutMs));
}

AarunoError AarunoClass::wifiStart(const char *ssid, const char *pass,
                                   unsigned long timeoutMs) {
  return setErr(_wifi.start(ssid, pass, timeoutMs));
}

AarunoWifiState AarunoClass::wifiState() const { return _wifi.state(); }
bool AarunoClass::wifiConnected() { return _wifi.connected(); }

long AarunoClass::wifiRSSI() {
  long r = _wifi.rssi();
  if (r != 0) { setErr(AARUNO_OK); return r; }
  switch (_wifi.state()) {
    case AARUNO_WIFI_UNSUPPORTED: setErr(AARUNO_UNSUPPORTED); break;
    case AARUNO_WIFI_IDLE: setErr(AARUNO_NOT_INITIALIZED); break;
    case AARUNO_WIFI_CONNECTING: setErr(AARUNO_ERROR); break;
    case AARUNO_WIFI_FAILED: setErr(AARUNO_TIMEOUT); break;
    case AARUNO_WIFI_CONNECTED:
    default: setErr(AARUNO_READ_FAIL); break;
  }
  return 0;
}

void AarunoClass::wifiDisconnect() { _wifi.disconnect(); }

int AarunoClass::httpGet(const char *url, Stream &out) {
#if defined(ESP32) || defined(ESP8266) || defined(ARDUINO_ARCH_ESP32) || defined(ARDUINO_ARCH_ESP8266)
  int code = AarunoHTTP::get(url, (void *)&out);
  setErr(code > 0 ? AARUNO_OK : AARUNO_ERROR);
  return code;
#else
  (void)url; (void)out;
  setErr(AARUNO_UNSUPPORTED);
  return -1;
#endif
}

// ---------------- Convenience aliases ----------------

AarunoError ultrasonicesp32(uint8_t trigPin, uint8_t echoPin) {
  return AARUNO.ultrasonic(trigPin, echoPin);
}
float ultrasonicesp32() { return AARUNO.readUltrasonic(); }

AarunoError ultrasonicesp8266(uint8_t trigPin, uint8_t echoPin) {
  return AARUNO.ultrasonic(trigPin, echoPin);
}
float ultrasonicesp8266() { return AARUNO.readUltrasonic(); }

AarunoError ultrasonicarduino(uint8_t trigPin, uint8_t echoPin) {
  return AARUNO.ultrasonic(trigPin, echoPin);
}
float ultrasonicarduino() { return AARUNO.readUltrasonic(); }
