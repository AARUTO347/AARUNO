#include "AarunoStepper.h"
#include "../AARUNOBoard.h"
#if defined(ARDUINO)
#include <Arduino.h>
#else
static void pinMode(int, int) {}
static void digitalWrite(int, int) {}
static void delay(unsigned long) {}
static void delayMicroseconds(unsigned int) {}
#ifndef OUTPUT
#define OUTPUT 1
#endif
#ifndef HIGH
#define HIGH 1
#endif
#ifndef LOW
#define LOW 0
#endif
#endif

// Half-step sequence (nibble: IN4 IN3 IN2 IN1 -> expanded per pin).
static const uint8_t AARUNO_STEPPER_SEQ[8] = {
    0b0001, 0b0011, 0b0010, 0b0110,
    0b0100, 0b1100, 0b1000, 0b1001};

AarunoStepper::AarunoStepper()
    : _rpm(15.0), _pos(0), _phase(0), _begun(false),
      _err(AARUNO_NOT_INITIALIZED) {
  for (int i = 0; i < 4; i++) _pins[i] = 255;
}

AarunoError AarunoStepper::begin(uint8_t in1, uint8_t in2, uint8_t in3,
                                 uint8_t in4) {
  uint8_t p[4] = {in1, in2, in3, in4};
  for (int i = 0; i < 4; i++) {
    for (int j = i + 1; j < 4; j++) {
      if (p[i] == p[j]) { _err = AARUNO_INVALID_CONFIG; _begun = false; return _err; }
    }
    AarunoError e = aarunoValidateDigitalPin(p[i], true);
    if (e != AARUNO_OK) { _err = e; _begun = false; return e; }
  }
  for (int i = 0; i < 4; i++) _pins[i] = p[i];
#if defined(ARDUINO)
  for (int i = 0; i < 4; i++) { pinMode(_pins[i], OUTPUT); digitalWrite(_pins[i], LOW); }
#endif
  _pos = 0;
  _phase = 0;
  _begun = true;
  _err = AARUNO_OK;
  return AARUNO_OK;
}

void AarunoStepper::setRpm(double rpm) {
  if (rpm < 1.0) rpm = 1.0;
  if (rpm > 20.0) rpm = 20.0;  // 28BYJ-48 slips beyond ~20 rpm
  _rpm = rpm;
}

void AarunoStepper::writePhase(int phase) {
  uint8_t nib = AARUNO_STEPPER_SEQ[phase & 7];
#if defined(ARDUINO)
  for (int i = 0; i < 4; i++) {
    digitalWrite(_pins[i], (nib >> i) & 1 ? HIGH : LOW);
  }
#else
  (void)nib;
#endif
}

AarunoError AarunoStepper::step(long steps) {
  if (!_begun) return setErr(AARUNO_NOT_INITIALIZED);
  // ms per half-step = 60_000 / (rpm * 2048)
  double msPerStep = 60000.0 / ((double)_rpm * 2048.0);
  unsigned long dly = (unsigned long)(msPerStep + 0.5);
  if (dly < 1) dly = 1;
  int dir = (steps >= 0) ? 1 : -1;
  long remaining = steps >= 0 ? steps : -steps;
  while (remaining-- > 0) {
    _phase = (_phase + dir + 8) & 7;
    writePhase(_phase);
#if defined(ARDUINO)
    delay(dly);
#else
    delay(dly);
#endif
    _pos += dir;
  }
  return setErr(AARUNO_OK);
}

void AarunoStepper::release() {
#if defined(ARDUINO)
  for (int i = 0; i < 4; i++) digitalWrite(_pins[i], LOW);
#endif
}
