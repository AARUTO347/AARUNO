#ifndef AARUNO_ANALOG_ENGINE_H
#define AARUNO_ANALOG_ENGINE_H

#include <stdint.h>
#include "../AARUNOTypes.h"

// Analog sensor kinds. Each named wrapper (lm35, gasMQ2, ...) selects one.
typedef enum {
  AARUNO_AN_RAW = 0,    // plain counts
  AARUNO_AN_LM35,       // deg C (10 mV/C)
  AARUNO_AN_TMP36,      // deg C (500 mV offset)
  AARUNO_AN_NTC,        // deg C via beta equation
  AARUNO_AN_MQ,         // gas ratio (calibrate for ppm)
  AARUNO_AN_CURRENT,    // ACS712 amps
  AARUNO_AN_PLAIN_PCT   // percent (soil/rain/water/sound/flex/force)
} AarunoAnalogKind;

// Generic analog engine: validates the pin, reads hardware, converts by kind.
// One instance = one sensor. Use several for parallel sensors.
class AarunoAnalogEngine {
 public:
  AarunoAnalogEngine();
  AarunoError begin(uint8_t pin, AarunoAnalogKind kind);
  void setNtcParams(float seriesR, float beta, float r0, float t0C);
  void setCurrentParams(float mvPerAmp, float zeroMv);
  bool configured() const { return _configured; }
  int readRaw();        // ADC counts, -1 on fail
  float readPercent();  // 0-100, NaN on fail
  float readVolts();    // volts at pin, NaN on fail
  float readSpecial();  // kind-specific unit, NaN on fail
  AarunoError lastError() const { return _err; }

 private:
  int _pin;
  AarunoAnalogKind _kind;
  float _seriesR, _beta, _r0, _t0;
  float _mvPerAmp, _zeroMv;
  bool _configured;
  AarunoError _err;
};

#endif
