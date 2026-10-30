#include "AarunoExtras.h"
#include "../AARUNOBoard.h"
#include "../AARUNOConfig.h"
#ifdef __has_include
#if defined(ARDUINO) && AARUNO_ENABLE_DHT
#if __has_include(<DallasTemperature.h>) && __has_include(<OneWire.h>)
#include <OneWire.h>
#include <DallasTemperature.h>
#define AARUNO_HAS_DS18B20 1
#endif
#endif
#if defined(ARDUINO)
#if __has_include(<Adafruit_BMP280.h>)
#include <Adafruit_BMP280.h>
#include <Wire.h>
#define AARUNO_HAS_BMP280 1
#endif
#endif
#endif
#if defined(ARDUINO)
#include <Arduino.h>
#endif

// ---------- DS18B20 ----------
AarunoDS18B20::AarunoDS18B20()
    : _begun(false), _err(AARUNO_NOT_INITIALIZED), _ow(nullptr),
      _bus(nullptr) {}

AarunoDS18B20::~AarunoDS18B20() {
#if defined(AARUNO_HAS_DS18B20)
  if (_bus) { delete (DallasTemperature *)_bus; _bus = nullptr; }
  if (_ow) { delete (OneWire *)_ow; _ow = nullptr; }
#endif
}

AarunoError AarunoDS18B20::begin(uint8_t pin) {
  AarunoError e = aarunoValidateDigitalPin(pin, false);
  if (e != AARUNO_OK) { _err = e; _begun = false; return e; }
#if defined(AARUNO_HAS_DS18B20)
  if (_bus) { delete (DallasTemperature *)_bus; _bus = nullptr; }
  if (_ow) { delete (OneWire *)_ow; _ow = nullptr; }
  OneWire *ow = new OneWire(pin);
  DallasTemperature *bus = new DallasTemperature(ow);
  bus->begin();
  _ow = ow;
  _bus = bus;
  _begun = true;
  _err = AARUNO_OK;
  return AARUNO_OK;
#else
  (void)pin;
  _begun = true;
  _err = AARUNO_MISSING_DEP;  // report at read time; stored config is valid
  return AARUNO_MISSING_DEP;
#endif
}

bool AarunoDS18B20::readC(float &c) {
  c = AARUNO_INVALID_READING;
#if defined(AARUNO_HAS_DS18B20)
  if (!_begun || !_bus) { _err = AARUNO_NOT_INITIALIZED; return false; }
  DallasTemperature *bus = (DallasTemperature *)_bus;
  bus->requestTemperatures();
  float t = bus->getTempCByIndex(0);
  if (t == DEVICE_DISCONNECTED_C) { _err = AARUNO_READ_FAIL; return false; }
  c = t;
  _err = AARUNO_OK;
  return true;
#else
  _err = _begun ? AARUNO_MISSING_DEP : AARUNO_NOT_INITIALIZED;
  return false;
#endif
}

// ---------- BMP280 ----------
AarunoBMP280::AarunoBMP280()
    : _begun(false), _err(AARUNO_NOT_INITIALIZED), _handle(nullptr) {}

AarunoBMP280::~AarunoBMP280() {
#if defined(AARUNO_HAS_BMP280)
  if (_handle) { delete (Adafruit_BMP280 *)_handle; _handle = nullptr; }
#endif
}

AarunoError AarunoBMP280::begin(uint8_t addr) {
#if defined(AARUNO_HAS_BMP280)
  if (addr < 0x08 || addr > 0x77) { _err = AARUNO_INVALID_CONFIG; return _err; }
  if (_handle) { delete (Adafruit_BMP280 *)_handle; _handle = nullptr; }
  Wire.begin();
  Adafruit_BMP280 *b = new Adafruit_BMP280();
  if (!b->begin(addr)) {
    delete b;
    _err = AARUNO_INIT_FAIL;
    _begun = false;
    return _err;
  }
  _handle = b;
  _begun = true;
  _err = AARUNO_OK;
  return AARUNO_OK;
#else
  (void)addr;
  _begun = true;
  _err = AARUNO_MISSING_DEP;
  return AARUNO_MISSING_DEP;
#endif
}

bool AarunoBMP280::readPressure(float &hPa) {
  hPa = AARUNO_INVALID_READING;
#if defined(AARUNO_HAS_BMP280)
  if (!_begun || !_handle) { _err = AARUNO_NOT_INITIALIZED; return false; }
  float p = ((Adafruit_BMP280 *)_handle)->readPressure();
  if (isnan(p) || p <= 0) { _err = AARUNO_READ_FAIL; return false; }
  hPa = p / 100.0f;
  _err = AARUNO_OK;
  return true;
#else
  _err = _begun ? AARUNO_MISSING_DEP : AARUNO_NOT_INITIALIZED;
  return false;
#endif
}

bool AarunoBMP280::readTempC(float &c) {
  c = AARUNO_INVALID_READING;
#if defined(AARUNO_HAS_BMP280)
  if (!_begun || !_handle) { _err = AARUNO_NOT_INITIALIZED; return false; }
  float t = ((Adafruit_BMP280 *)_handle)->readTemperature();
  if (isnan(t)) { _err = AARUNO_READ_FAIL; return false; }
  c = t;
  _err = AARUNO_OK;
  return true;
#else
  _err = _begun ? AARUNO_MISSING_DEP : AARUNO_NOT_INITIALIZED;
  return false;
#endif
}
