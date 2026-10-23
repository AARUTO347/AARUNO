#ifndef AARUNO_ULTRASONIC_H
#define AARUNO_ULTRASONIC_H

#include <stdint.h>
#include "../AARUNOTypes.h"
#include "../AARUNOConfig.h"

// Independent HC-SR04 instance. Use this when you need more than one sensor.
// The AARUNO singleton wraps one of these for the beginner-friendly API.
class AarunoUltrasonic {
 public:
  AarunoUltrasonic();
  AarunoError begin(uint8_t trigPin, uint8_t echoPin,
                    unsigned long timeoutUs = AARUNO_ULTRASONIC_TIMEOUT_US);
  void end();
  bool configured() const { return _configured; }
  void setTimeout(unsigned long timeoutUs) { _timeoutUs = timeoutUs; }

  // Returns distance in cm, or NaN on failure (check lastError()).
  float readCm();
  // Status-aware variant: returns false on failure.
  bool readCm(float &outCm);

  AarunoError lastError() const { return _lastError; }

 private:
  uint8_t _trig;
  uint8_t _echo;
  unsigned long _timeoutUs;
  bool _configured;
  AarunoError _lastError;
};

#endif
