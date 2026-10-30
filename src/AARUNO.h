// ============================================================
//  AARUNO v1.1.1 — Universal Arduino & ESP Library
//  MADE BY AARUTO
//  100+ sensors, actuators, displays, comms. Simple syntax.
// ============================================================
#ifndef AARUNO_H
#define AARUNO_H

#include "AARUNOTypes.h"
#include "AARUNOConfig.h"
#include "AARUNOBoard.h"
#include "sensors/AarunoUltrasonic.h"
#include "sensors/AarunoDHT.h"
#include "sensors/AarunoDigital.h"
#include "sensors/AarunoAnalogEngine.h"
#include "sensors/AarunoDigitalEngine.h"
#include "sensors/AarunoEncoder.h"
#include "sensors/AarunoKeypad.h"
#include "sensors/AarunoMPU6050.h"
#include "sensors/AarunoI2CUtil.h"
#include "sensors/AarunoExtras.h"
#include "actuators/AarunoStepper.h"
#include "displays/AarunoDisplay.h"
#include "communication/AarunoWifi.h"
#include "communication/AarunoHTTP.h"

#if defined(ARDUINO)
#include <Arduino.h>
#else
#include <stddef.h>
#include <stdint.h>
// Minimal host fallback so IDEs/static analysis can parse the header.
class Stream {
 public:
  virtual size_t print(const char *) { return 0; }
};
#endif

// Main singleton API — MADE BY AARUTO. Recommended usage:
//   #include <AARUNO.h>
//   void setup() { AARUNO.begin(); AARUNO.ultrasonic(7, 8); AARUNO.lcdI2C(0x27,16,2); }
//   void loop()  { AARUNO.update(); float cm = AARUNO.readUltrasonic(); }
//
// Singleton holds ONE of each kind. For several sensors of the same kind use
// the independent engine classes directly (documented in docs/modules.md).
class AarunoClass {
 public:
  AarunoClass();

  // ---- Core ----
  void begin();
  void update();
  void setDebugStream(Stream *s) { _dbg = s; }
  const char *version() const { return AARUNO_VERSION_STRING; }
  const char *madeBy() const { return "MADE BY AARUTO"; }
  const char *credits() const { return "AARUNO v" AARUNO_VERSION_STRING " — MADE BY AARUTO"; }
  AarunoFamily boardFamily() const;
  const char *boardName() const;
  AarunoError lastError() const { return _err; }
  void clearError() { _err = AARUNO_OK; }
  const char *errorString() const;
  static const char *errorString(AarunoError e);

  // ---- Ultrasonic (HC-SR04) ----
  // NOTE: readUltrasonic() BLOCKS up to the echo timeout (default 30 ms).
  AarunoError ultrasonic(uint8_t trigPin, uint8_t echoPin);
  AarunoError setUltrasonicTimeout(unsigned long timeoutUs);
  float readUltrasonic();
  bool readUltrasonic(float &cm);
  float readUltrasonicInches();

  // ---- Temperature / humidity (optional DHT lib) ----
  AarunoError dht11(uint8_t pin);
  AarunoError dht22(uint8_t pin);
  float readTemperature();
  float readHumidity();
  bool readDHT(float &tempC, float &humPct);
  bool dhtFresh() const;
  unsigned long dhtAgeMs() const;

  // ---- Analog engine (one active analog sensor) ----
  // Setup picks the sensor type; readX converts with the right math.
  AarunoError analogSensor(uint8_t pin);        // raw counts
  AarunoError ldr(uint8_t pin);                 // raw counts
  AarunoError potentiometer(uint8_t pin);       // raw counts
  AarunoError lm35(uint8_t pin);                // deg C
  AarunoError tmp36(uint8_t pin);               // deg C
  AarunoError thermistor(uint8_t pin);          // deg C (beta eq.)
  AarunoError soilMoisture(uint8_t pin);        // percent
  AarunoError rainSensor(uint8_t pin);          // percent
  AarunoError waterLevel(uint8_t pin);          // percent
  AarunoError soundSensor(uint8_t pin);         // percent
  AarunoError flexSensor(uint8_t pin);          // percent
  AarunoError forceSensor(uint8_t pin);         // percent (FSR)
  AarunoError flameAnalog(uint8_t pin);         // percent
  AarunoError gasMQ2(uint8_t pin);              // RS/RL ratio
  AarunoError gasMQ3(uint8_t pin);
  AarunoError gasMQ4(uint8_t pin);
  AarunoError gasMQ5(uint8_t pin);
  AarunoError gasMQ6(uint8_t pin);
  AarunoError gasMQ7(uint8_t pin);
  AarunoError gasMQ8(uint8_t pin);
  AarunoError gasMQ9(uint8_t pin);
  AarunoError gasMQ135(uint8_t pin);
  AarunoError currentSensor(uint8_t pin);       // ACS712 amps
  void setThermistorParams(float seriesR, float beta, float r0, float t0C);
  void setCurrentParams(float mvPerAmp, float zeroMv);
  int readAnalogRaw();
  float readAnalogPercent();
  float readAnalogVoltage();
  float readAnalogValue();       // kind-specific unit
  int readLDR();                 // raw
  int readPotentiometer();       // raw
  float readTemperatureAnalog(); // deg C (LM35/TMP36/NTC active)
  float readGas();               // MQ ratio
  float readCurrent();           // amps
  float readSoil();              // percent
  float readRain();              // percent
  float readWaterLevel();        // percent
  float readSound();             // percent
  float readFlex();              // percent
  float readForce();             // percent
  float readFlameAnalog();       // percent
  int readAnalog(uint8_t pin);   // direct raw read, validated
  float readVoltage(uint8_t pin, float dividerRatio = 1.0f);

  // ---- Digital engine (one active digital sensor) ----
  AarunoError digitalSensor(uint8_t pin, bool activeLow = false, bool pullup = false);
  AarunoError pir(uint8_t pin);
  AarunoError irObstacle(uint8_t pin);
  AarunoError lineFollower(uint8_t pin);        // most modules active-low
  AarunoError flameSensor(uint8_t pin);         // most modules active-low
  AarunoError vibrationSensor(uint8_t pin);     // SW-420 active-high
  AarunoError tiltSensor(uint8_t pin);
  AarunoError touchSensor(uint8_t pin);         // TTP223 active-high
  AarunoError hallSensor(uint8_t pin);          // A3144 active-low
  AarunoError reedSwitch(uint8_t pin);
  AarunoError limitSwitch(uint8_t pin, bool pullup = true, bool activeLow = true);
  AarunoError irReceiver(uint8_t pin);          // raw pin state (no decode)
  bool readDigitalSensor();
  bool readPIR();
  bool readIR();
  bool readLine();
  bool readFlame();
  bool readVibration();
  bool readTilt();
  bool readTouch();
  bool readHall();
  bool readReed();
  bool readLimit();
  bool readIRRemote();

  // ---- Button ----
  AarunoError button(uint8_t pin, bool activeLow = true, bool pullup = true);
  int readButton();
  bool buttonPressed();
  bool buttonRose();   // press event
  bool buttonFell();   // release event

  // ---- Rotary encoder ----
  AarunoError rotaryEncoder(uint8_t clk, uint8_t dt, int swPin = -1);
  long encoderCount();
  void encoderReset();
  bool encoderButton();

  // ---- 4x4 keypad (matrix) ----
  AarunoError keypad4x4(uint8_t r0, uint8_t r1, uint8_t r2, uint8_t r3,
                        uint8_t c0, uint8_t c1, uint8_t c2, uint8_t c3);
  char readKey();  // blocking single scan (~ms); 0 = no key

  // ---- Joystick ----
  AarunoError joystick(uint8_t xPin, uint8_t yPin, int swPin = -1);
  int readJoyX();        // raw
  int readJoyY();        // raw
  bool readJoyButton();  // active-low

  // ---- MPU6050 IMU (real I2C driver, no external lib) ----
  AarunoError mpu6050(uint8_t addr = 0x68);
  bool readAccel(float &x, float &y, float &z);  // g
  bool readGyro(float &x, float &y, float &z);   // deg/s
  float readIMUTemp();                            // deg C, NaN on fail
  float readAccelX(); float readAccelY(); float readAccelZ();
  float readGyroX(); float readGyroY(); float readGyroZ();

  // ---- Optional I2C extras ----
  AarunoError ds18b20(uint8_t pin);      // needs OneWire + DallasTemperature
  float readDS18B20();                    // deg C, NaN on fail
  AarunoError bmp280(uint8_t addr = 0x76); // needs Adafruit BMP280
  float readPressure();                   // hPa, NaN on fail
  float readBMPTemp();                    // deg C, NaN on fail

  // ---- Generic I2C helpers ----
  bool i2cDevicePresent(uint8_t addr);
  int i2cScan();           // first device address, -1 if none
  uint8_t i2cScanCount();
  int i2cRead8(uint8_t addr, uint8_t reg);
  bool i2cWrite8(uint8_t addr, uint8_t reg, uint8_t value);
  // Auto-detect: probe common addresses and init the first display found.
  uint8_t autoScan();      // number of I2C devices found on the bus
  AarunoError oledAuto();  // SSD1306 at 0x3C then 0x3D
  AarunoError lcdAuto();   // LCD 16x2 at 0x27 then 0x38

  // ---- Displays (optional deps) ----
  AarunoError oled(uint8_t addr = 0x3C, uint8_t w = 128, uint8_t h = 64);
  AarunoError lcd(uint8_t addr, uint8_t cols, uint8_t rows);
  AarunoError lcdI2C(uint8_t addr, uint8_t cols, uint8_t rows);  // alias
  AarunoError displayClear();
  AarunoError displayText(const char *line);
  AarunoError displayText(int row, const char *line);
  AarunoError displayTextAt(int row, int col, const char *line);
  AarunoError displayBacklight(bool on);

  // ---- Actuators ----
  AarunoError led(uint8_t pin);
  AarunoError setLED(bool on);
  AarunoError toggleLED();
  AarunoError setBrightness(uint8_t value);  // 0-255 PWM
  AarunoError buzzer(uint8_t pin);
  AarunoError beep(unsigned long durationMs);  // blocking helper
  AarunoError beep(unsigned int freqHz, unsigned long durationMs);
  AarunoError buzzerOff();
  AarunoError relay(uint8_t pin, bool activeHigh = true);
  AarunoError setRelay(bool on);
  AarunoError solenoid(uint8_t pin, bool activeHigh = true);  // via relay driver
  AarunoError setSolenoid(bool on);
  AarunoError laserEmitter(uint8_t pin);      // KY-008 laser diode module
  AarunoError setLaser(bool on);
  AarunoError rgb(uint8_t rPin, uint8_t gPin, uint8_t bPin);  // common cathode
  AarunoError setRGBRGB(uint8_t r, uint8_t g, uint8_t b);     // 0-255 each
  AarunoError setRGB(bool on);                                // on/off
  AarunoError pwmOut(uint8_t pin);
  AarunoError writePWM(uint8_t value);        // 0-255 on pwm pin
  AarunoError motorBegin(uint8_t in1, uint8_t in2, int enPin = -1);
  AarunoError motorSpeed(int speed);  // -255..255
  AarunoError stepperAttach(uint8_t in1, uint8_t in2, uint8_t in3, uint8_t in4);
  AarunoError stepperSetRpm(double rpm);
  AarunoError stepperStep(long steps);  // BLOCKING per step
  AarunoError stepperRelease();
  AarunoError servoAttach(uint8_t pin);
  AarunoError servoWrite(int angleDeg);
  AarunoError servoDetach();

  // ---- WiFi (ESP32/ESP8266 only) ----
  AarunoError wifiBegin(const char *ssid, const char *pass,
                        unsigned long timeoutMs = 15000);  // BLOCKING
  AarunoError wifiStart(const char *ssid, const char *pass,
                        unsigned long timeoutMs = 15000);  // non-blocking
  AarunoWifiState wifiState() const;
  bool wifiConnected();
  long wifiRSSI();
  void wifiDisconnect();
  int httpGet(const char *url, Stream &out);  // HTTP code, -1 on fail (ESP)

  // ---- Pin helpers ----
  bool isValidDigitalPin(uint8_t pin, bool needOutput = false);
  bool isValidAnalogPin(int pin);
  bool isValidPwmPin(uint8_t pin);

 private:
  void dbg(const char *msg);
  AarunoError setErr(AarunoError e) { _err = e; return e; }
  AarunoError cfgAnalog(uint8_t pin, AarunoAnalogKind kind);
  AarunoError cfgDigital(uint8_t pin, bool activeLow, bool pullup);

  bool _begun;
  AarunoError _err;
  Stream *_dbg;

  AarunoUltrasonic _sonic;
  AarunoDHT _dht;
  AarunoButton _btn;
  AarunoDisplay _disp;
  AarunoWifi _wifi;
  AarunoAnalogEngine _aeng;
  AarunoDigitalEngine _deng;
  AarunoEncoder _enc;
  AarunoKeypad _keypad;
  AarunoMPU6050 _mpu;
  AarunoDS18B20 _ds;
  AarunoBMP280 _bmp;
  AarunoStepper *_step;

  int _ldrPin;
  int _potPin;
  int _ledPin;
  int _buzzerPin;
  int _relayPin;
  int _solenoidPin;
  int _laserPin;
  int _pwmPin;
  int _joyX, _joyY, _joySw;
  uint8_t _rgbPins[3];
  bool _relayActiveHigh;
  bool _solenoidActiveHigh;
  bool _ledState;
  bool _relayState;
  bool _solenoidState;
  bool _rgbSetUp;

  int _motorIn1;
  int _motorIn2;
  int _motorEn;
  int _servoPin;
  void *_servo;
};

extern AarunoClass AARUNO;

// ---- Board-specific convenience aliases (optional helpers) ----
AarunoError ultrasonicesp32(uint8_t trigPin, uint8_t echoPin);
float ultrasonicesp32();
AarunoError ultrasonicesp8266(uint8_t trigPin, uint8_t echoPin);
float ultrasonicesp8266();
AarunoError ultrasonicarduino(uint8_t trigPin, uint8_t echoPin);
float ultrasonicarduino();

#endif
