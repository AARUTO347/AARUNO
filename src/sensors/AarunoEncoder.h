#ifndef AARUNO_ENCODER_H
#define AARUNO_ENCODER_H

#include <stdint.h>
#include "../AARUNOTypes.h"

// Rotary encoder (KY-040 style: CLK, DT, optional SW). Polled, non-blocking.
// Call update() every loop (AARUNO.update() does it for the singleton).
class AarunoEncoder {
 public:
  AarunoEncoder();
  AarunoError begin(uint8_t clkPin, uint8_t dtPin, int swPin = -1);
  void update();                       // poll now (uses millis internally)
  void update(unsigned long nowMs);    // poll with injected time
  long count() const { return _count; }
  void reset() { _count = 0; }
  bool buttonPressed();                // requires swPin in begin(); active-low
  void inject(uint8_t clk, uint8_t dt, unsigned long nowMs);  // host tests
  AarunoError lastError() const { return _err; }

 private:
  uint8_t _clk, _dt;
  int _sw;
  int _lastClk;
  long _count;
  unsigned long _lastMs;
  bool _begun;
  AarunoError _err;
};

#endif
