#ifndef AARUNO_KEYPAD_H
#define AARUNO_KEYPAD_H

#include <stdint.h>
#include "../AARUNOTypes.h"

// 4x4 matrix keypad (8 pins). readKey() does one fast scan (~a few ms,
// documented blocking). Returns '1'..'9','0','A'..'D','*','#' or 0.
class AarunoKeypad {
 public:
  AarunoKeypad();
  AarunoError begin(uint8_t r0, uint8_t r1, uint8_t r2, uint8_t r3,
                    uint8_t c0, uint8_t c1, uint8_t c2, uint8_t c3);
  char readKey();
  bool configured() const { return _begun; }
  AarunoError lastError() const { return _err; }

 private:
  uint8_t _rows[4];
  uint8_t _cols[4];
  bool _begun;
  AarunoError _err;
};

#endif
