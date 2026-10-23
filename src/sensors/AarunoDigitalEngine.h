#ifndef AARUNO_DIGITAL_ENGINE_H
#define AARUNO_DIGITAL_ENGINE_H

#include <stdint.h>
#include "../AARUNOTypes.h"

// Generic digital sensor engine (PIR/IR/line/flame/vibration/tilt/reed/hall/
// touch/limit/IR receiver: all read a digital level with an active polarity).
class AarunoDigitalEngine {
 public:
  AarunoDigitalEngine();
  AarunoError begin(uint8_t pin, bool activeLow = false, bool pullup = false);
  bool configured() const { return _configured; }
  int readRaw();       // 0/1, -1 on fail
  bool triggered();    // true when level matches active polarity
  AarunoError lastError() const { return _err; }

 private:
  uint8_t _pin;
  bool _activeLow;
  bool _configured;
  AarunoError _err;
};

#endif
