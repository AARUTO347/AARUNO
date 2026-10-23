#ifndef AARUNO_STEPPER_H
#define AARUNO_STEPPER_H

#include <stdint.h>
#include "../AARUNOTypes.h"

// 28BYJ-48 stepper via ULN2003 driver board (4 GPIO pins).
// step() is BLOCKING per step (documented); default 2048 half-steps/rev.
class AarunoStepper {
 public:
  AarunoStepper();
  AarunoError begin(uint8_t in1, uint8_t in2, uint8_t in3, uint8_t in4);
  void setRpm(double rpm);            // clamps 1..20 (28BYJ-48 practical max)
  // BLOCKING: rotates steps (negative = reverse), ~1-2 ms per step.
  AarunoError step(long steps);
  void release();                     // de-energize coils
  long position() const { return _pos; }
  AarunoError lastError() const { return _err; }

 private:
  void writePhase(int phase);
  uint8_t _pins[4];
  double _rpm;
  long _pos;
  int _phase;
  bool _begun;
  AarunoError _err;
};

#endif
