#include "AarunoMPU6050.h"
#if defined(ARDUINO)
#include <Wire.h>
#else
static void beginTransmission(int) {}
static void write(int) {}
static int endTransmission() { return 2; }
static int requestFrom(int, int) { return 0; }
static int available() { return 0; }
static int read() { return -1; }
#endif

// MPU6050 registers (public datasheet values).
#define AARUNO_MPU_PWR 0x6B
#define AARUNO_MPU_ACCEL 0x3B
#define AARUNO_MPU_TEMP 0x41
#define AARUNO_MPU_GYRO 0x43

AarunoMPU6050::AarunoMPU6050()
    : _addr(0x68), _begun(false), _err(AARUNO_NOT_INITIALIZED) {}

AarunoError AarunoMPU6050::begin(uint8_t addr) {
#if defined(ARDUINO)
  if (addr < 0x08 || addr > 0x77) { _err = AARUNO_INVALID_CONFIG; return _err; }
  Wire.begin();
  _addr = addr;
  Wire.beginTransmission(_addr);
  Wire.write(AARUNO_MPU_PWR);
  Wire.write(0);  // wake up
  if (Wire.endTransmission() != 0) {
    _err = AARUNO_INIT_FAIL;  // NACK: no MPU at this address
    _begun = false;
    return _err;
  }
  _begun = true;
  _err = AARUNO_OK;
  return AARUNO_OK;
#else
  (void)addr;
  _err = AARUNO_UNSUPPORTED;
  return _err;
#endif
}

bool AarunoMPU6050::readBurst(uint8_t reg, uint8_t len, uint8_t *buf) {
#if defined(ARDUINO)
  if (!_begun) { _err = AARUNO_NOT_INITIALIZED; return false; }
  Wire.beginTransmission(_addr);
  Wire.write(reg);
  if (Wire.endTransmission(false) != 0) { _err = AARUNO_READ_FAIL; return false; }
  if (Wire.requestFrom((int)_addr, (int)len) != len) {
    _err = AARUNO_READ_FAIL;
    return false;
  }
  for (uint8_t i = 0; i < len; i++) buf[i] = Wire.read();
  _err = AARUNO_OK;
  return true;
#else
  (void)reg; (void)len; (void)buf;
  _err = AARUNO_UNSUPPORTED;
  return false;
#endif
}

bool AarunoMPU6050::readAccel(float &x, float &y, float &z) {
  uint8_t b[6];
  if (!readBurst(AARUNO_MPU_ACCEL, 6, b)) return false;
  int16_t ax = (int16_t)((b[0] << 8) | b[1]);
  int16_t ay = (int16_t)((b[2] << 8) | b[3]);
  int16_t az = (int16_t)((b[4] << 8) | b[5]);
  x = (float)ax / 16384.0f;  // +-2 g default
  y = (float)ay / 16384.0f;
  z = (float)az / 16384.0f;
  return true;
}

bool AarunoMPU6050::readGyro(float &x, float &y, float &z) {
  uint8_t b[6];
  if (!readBurst(AARUNO_MPU_GYRO, 6, b)) return false;
  int16_t gx = (int16_t)((b[0] << 8) | b[1]);
  int16_t gy = (int16_t)((b[2] << 8) | b[3]);
  int16_t gz = (int16_t)((b[4] << 8) | b[5]);
  x = (float)gx / 131.0f;  // +-250 dps default
  y = (float)gy / 131.0f;
  z = (float)gz / 131.0f;
  return true;
}

bool AarunoMPU6050::readTempC(float &c) {
  uint8_t b[2];
  if (!readBurst(AARUNO_MPU_TEMP, 2, b)) return false;
  int16_t t = (int16_t)((b[0] << 8) | b[1]);
  c = (float)t / 340.0f + 36.53f;
  return true;
}
