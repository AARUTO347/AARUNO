#ifndef AARUNO_EXTRAS_H
#define AARUNO_EXTRAS_H

#include <stdint.h>
#include "../AARUNOTypes.h"

// Optional-dependency drivers (DS18B20, BMP280). Each compiles only when the
// matching library is installed; otherwise begin()/read return MISSING_DEP.

// DS18B20 — needs "OneWire" + "DallasTemperature".
class AarunoDS18B20 {
 public:
  AarunoDS18B20();
  ~AarunoDS18B20();
  AarunoError begin(uint8_t pin);
  bool configured() const { return _begun; }
  bool readC(float &c);
  AarunoError lastError() const { return _err; }

 private:
  bool _begun;
  AarunoError _err;
  void *_ow;
  void *_bus;
};

// BMP280 — needs "Adafruit BMP280 Library" (+ Adafruit Unified Sensor).
class AarunoBMP280 {
 public:
  AarunoBMP280();
  ~AarunoBMP280();
  AarunoError begin(uint8_t addr = 0x76);
  bool configured() const { return _begun; }
  bool readPressure(float &hPa);
  bool readTempC(float &c);
  AarunoError lastError() const { return _err; }

 private:
  bool _begun;
  AarunoError _err;
  void *_handle;
};

#endif
