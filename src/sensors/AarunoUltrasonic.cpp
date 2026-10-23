#include "AarunoUltrasonic.h"
#include "../AARUNOBoard.h"
#include "../utility/AarunoUtil.h"
#if defined(ARDUINO)
#include <Arduino.h>
#else
// Host compile fallback (unit tests never call hardware paths).
static unsigned long millis() { return 0; }
static void pinMode(uint8_t, uint8_t) {}
static void digitalWrite(uint8_t, uint8_t) {}
static void delayMicroseconds(unsigned int) {}
static unsigned long pulseIn(uint8_t, uint8_t, unsigned long) { return 0; }
#ifndef OUTPUT
#define OUTPUT 1
#endif
#ifndef INPUT
#define INPUT 0
#endif
#ifndef LOW
#define LOW 0
#endif
#ifndef HIGH
#define HIGH 1
#endif
#endif
#include <math.h>

AarunoUltrasonic::AarunoUltrasonic()
    : _trig(255), _echo(255), _timeoutUs(AARUNO_ULTRASONIC_TIMEOUT_US),
      _configured(false), _lastError(AARUNO_NOT_INITIALIZED) {}

AarunoError AarunoUltrasonic::begin(uint8_t trigPin, uint8_t echoPin,
                                    unsigned long timeoutUs) {
  AarunoError e1 = aarunoValidateDigitalPin(trigPin, true);
  if (e1 != AARUNO_OK) { _lastError = e1; _configured = false; return e1; }
  AarunoError e2 = aarunoValidateDigitalPin(echoPin, false);
  if (e2 != AARUNO_OK) { _lastError = e2; _configured = false; return e2; }
  if (trigPin == echoPin) {
    _lastError = AARUNO_INVALID_CONFIG;
    _configured = false;
    return AARUNO_INVALID_CONFIG;
  }
  if (timeoutUs == 0) timeoutUs = AARUNO_ULTRASONIC_TIMEOUT_US;
  _trig = trigPin;
  _echo = echoPin;
  _timeoutUs = timeoutUs;
#if defined(ARDUINO)
  pinMode(_trig, OUTPUT);
  pinMode(_echo, INPUT);
  digitalWrite(_trig, LOW);
#endif
  _configured = true;
  _lastError = AARUNO_OK;
  return AARUNO_OK;
}

void AarunoUltrasonic::end() {
  _configured = false;
  _trig = 255;
  _echo = 255;
  _lastError = AARUNO_NOT_INITIALIZED;
}

float AarunoUltrasonic::readCm() {
  float cm = 0;
  if (!readCm(cm)) return AARUNO_INVALID_READING;
  return cm;
}

bool AarunoUltrasonic::readCm(float &outCm) {
  outCm = AARUNO_INVALID_READING;
  if (!_configured) { _lastError = AARUNO_NOT_INITIALIZED; return false; }
#if defined(ARDUINO)
  digitalWrite(_trig, LOW);
  delayMicroseconds(2);
  digitalWrite(_trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(_trig, LOW);
  unsigned long dur = pulseIn(_echo, HIGH, _timeoutUs);
  if (dur == 0) { _lastError = AARUNO_TIMEOUT; return false; }
  outCm = aarunoDistanceCmFromUs(dur);
  _lastError = AARUNO_OK;
  return true;
#else
  _lastError = AARUNO_UNSUPPORTED;
  return false;
#endif
}
