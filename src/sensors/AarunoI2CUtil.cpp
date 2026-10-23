#include "AarunoI2CUtil.h"
#if defined(ARDUINO)
#include <Wire.h>
#endif

bool AarunoI2C::isDevicePresent(uint8_t addr) {
#if defined(ARDUINO)
  if (addr < 0x08 || addr > 0x77) return false;
  Wire.beginTransmission(addr);
  return Wire.endTransmission() == 0;
#else
  (void)addr;
  return false;
#endif
}

int AarunoI2C::scanFirst() {
#if defined(ARDUINO)
  for (uint8_t a = 0x08; a <= 0x77; a++) {
    if (isDevicePresent(a)) return (int)a;
  }
  return -1;
#else
  return -1;
#endif
}

uint8_t AarunoI2C::scanCount() {
#if defined(ARDUINO)
  uint8_t n = 0;
  for (uint8_t a = 0x08; a <= 0x77; a++) {
    if (isDevicePresent(a)) n++;
  }
  return n;
#else
  return 0;
#endif
}

int AarunoI2C::readReg8(uint8_t addr, uint8_t reg) {
#if defined(ARDUINO)
  if (addr < 0x08 || addr > 0x77) return -1;
  Wire.beginTransmission(addr);
  Wire.write(reg);
  if (Wire.endTransmission(false) != 0) return -1;
  if (Wire.requestFrom((int)addr, 1) != 1) return -1;
  return Wire.read();
#else
  (void)addr; (void)reg;
  return -1;
#endif
}

bool AarunoI2C::writeReg8(uint8_t addr, uint8_t reg, uint8_t value) {
#if defined(ARDUINO)
  if (addr < 0x08 || addr > 0x77) return false;
  Wire.beginTransmission(addr);
  Wire.write(reg);
  Wire.write(value);
  return Wire.endTransmission() == 0;
#else
  (void)addr; (void)reg; (void)value;
  return false;
#endif
}
