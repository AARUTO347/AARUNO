#ifndef AARUNO_DHT_H
#define AARUNO_DHT_H

#include <stdint.h>
#include "../AARUNOTypes.h"

// DHT11/DHT22 wrapper. Requires the optional "DHT sensor library"
// (Adafruit) + "Adafruit Unified Sensor". If the headers are missing,
// begin() succeeds in storing the pin but reads return AARUNO_MISSING_DEP
// instead of inventing values.
class AarunoDHT {
 public:
  AarunoDHT();
  ~AarunoDHT();
  AarunoError begin(uint8_t pin, uint8_t type);  // type: 11 or 22
  bool configured() const { return _configured; }
  float readTemperature(bool force = false);
  float readHumidity(bool force = false);
  bool read(float &tempC, float &humPct, bool force = false);
  // Cache insight: failed reads return false + READ_FAIL and never overwrite
  // _temp/_hum, but read() still returns NaN (never stale data as success).
  // Use fresh()/ageMs() to tell a fresh sample from a cached one.
  bool fresh() const { return _fresh; }
  unsigned long ageMs(unsigned long nowMs) const;
  AarunoError lastError() const { return _err; }

 private:
  bool sample(bool force);
  uint8_t _pin;
  uint8_t _type;
  bool _configured;
  bool _fresh;  // true if last successful read sampled hardware
  float _temp;
  float _hum;
  unsigned long _lastMs;
  AarunoError _err;
  void *_handle;  // DHT* when dependency present, else nullptr
};

#endif
