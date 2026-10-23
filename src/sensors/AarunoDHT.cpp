#include "AarunoDHT.h"
#include "../AARUNOBoard.h"
#include "../AARUNOConfig.h"

#if defined(ARDUINO) && AARUNO_ENABLE_DHT
#ifdef __has_include
#if __has_include(<DHT.h>)
#include <DHT.h>
#define AARUNO_HAS_DHT_LIB 1
#endif
#endif
#endif

#if defined(ARDUINO)
#include <Arduino.h>
#else
static unsigned long millis() { return 0; }
#endif

AarunoDHT::AarunoDHT()
    : _pin(255), _type(11), _configured(false), _fresh(false),
      _temp(AARUNO_INVALID_READING), _hum(AARUNO_INVALID_READING), _lastMs(0),
      _err(AARUNO_NOT_INITIALIZED), _handle(nullptr) {}

AarunoDHT::~AarunoDHT() {
#if defined(AARUNO_HAS_DHT_LIB)
  if (_handle) { delete (DHT *)_handle; _handle = nullptr; }
#endif
}

AarunoError AarunoDHT::begin(uint8_t pin, uint8_t type) {
  if (type != 11 && type != 22) { _err = AARUNO_INVALID_CONFIG; return _err; }
  AarunoError e = aarunoValidateDigitalPin(pin, false);
  if (e != AARUNO_OK) { _err = e; _configured = false; return e; }
#if defined(AARUNO_HAS_DHT_LIB)
  if (_handle) { delete (DHT *)_handle; _handle = nullptr; }
  uint8_t dhtType = (type == 11) ? DHT11 : DHT22;
  DHT *d = new DHT(pin, dhtType);
  d->begin();
  _handle = d;
  _pin = pin;
  _type = type;
  _configured = true;
  _fresh = false;
  _lastMs = 0;
  _temp = AARUNO_INVALID_READING;
  _hum = AARUNO_INVALID_READING;
  _err = AARUNO_OK;
  return AARUNO_OK;
#else
  // Store config so error is reported at read time with a clear code.
  _pin = pin;
  _type = type;
  _configured = true;
  _err = AARUNO_MISSING_DEP;
  return AARUNO_MISSING_DEP;
#endif
}

bool AarunoDHT::sample(bool force) {
#if defined(AARUNO_HAS_DHT_LIB)
  if (!_configured || !_handle) { _err = AARUNO_NOT_INITIALIZED; return false; }
  unsigned long now =
#if defined(ARDUINO)
      millis();
#else
      0;
#endif
  if (!force && _lastMs != 0 &&
      (now - _lastMs) < AARUNO_DHT_MIN_INTERVAL_MS) {
    // Cache hit: report success but mark as not-fresh so callers can tell.
    _fresh = false;
    return !isnan(_temp) && !isnan(_hum);
  }
  DHT *d = (DHT *)_handle;
  float h = d->readHumidity();
  float t = d->readTemperature();
  if (isnan(h) || isnan(t)) {
    _err = AARUNO_READ_FAIL;
    _fresh = false;
    return false;
  }
  _hum = h;
  _temp = t;
  _lastMs = now;
  _fresh = true;
  _err = AARUNO_OK;
  return true;
#else
  (void)force;
  _err = _configured ? AARUNO_MISSING_DEP : AARUNO_NOT_INITIALIZED;
  return false;
#endif
}

unsigned long AarunoDHT::ageMs(unsigned long nowMs) const {
  if (_lastMs == 0) return 0xFFFFFFFFUL;
  return nowMs - _lastMs;
}

float AarunoDHT::readTemperature(bool force) {
  float t, h;
  if (!read(t, h, force)) return AARUNO_INVALID_READING;
  return t;
}

float AarunoDHT::readHumidity(bool force) {
  float t, h;
  if (!read(t, h, force)) return AARUNO_INVALID_READING;
  return h;
}

bool AarunoDHT::read(float &tempC, float &humPct, bool force) {
  tempC = AARUNO_INVALID_READING;
  humPct = AARUNO_INVALID_READING;
  if (!_configured) { _err = AARUNO_NOT_INITIALIZED; return false; }
  if (!sample(force)) return false;
  tempC = _temp;
  humPct = _hum;
  return true;
}
