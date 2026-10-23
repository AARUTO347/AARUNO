#ifndef AARUNO_MPU6050_H
#define AARUNO_MPU6050_H

#include <stdint.h>
#include "../AARUNOTypes.h"

// Minimal real MPU6050 driver over Wire (no external library needed).
// Wake register write + burst reads of the data registers.
class AarunoMPU6050 {
 public:
  AarunoMPU6050();
  AarunoError begin(uint8_t addr = 0x68);
  bool configured() const { return _begun; }
  // Accel in g, gyro in deg/s. false + READ_FAIL on NACK.
  bool readAccel(float &x, float &y, float &z);
  bool readGyro(float &x, float &y, float &z);
  bool readTempC(float &c);
  AarunoError lastError() const { return _err; }

 private:
  bool readBurst(uint8_t reg, uint8_t len, uint8_t *buf);
  uint8_t _addr;
  bool _begun;
  AarunoError _err;
};

#endif
