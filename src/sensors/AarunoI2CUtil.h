#ifndef AARUNO_I2CUTIL_H
#define AARUNO_I2CUTIL_H

#include <stdint.h>
#include "../AARUNOTypes.h"

// Generic I2C helpers over Wire (no external library).
class AarunoI2C {
 public:
  static bool isDevicePresent(uint8_t addr);       // ACK probe
  static int scanFirst();                          // first addr 8..119, else -1
  static uint8_t scanCount();                      // number of devices found
  static int readReg8(uint8_t addr, uint8_t reg);  // value or -1
  static bool writeReg8(uint8_t addr, uint8_t reg, uint8_t value);
  static void begin();                             // initialize Wire once

 private:
  static bool _wireBegun;
};

#endif
