#include "AarunoDigitalEngine.h"
#include "../AARUNOBoard.h"
#if defined(ARDUINO)
#include <Arduino.h>
#else
static int digitalRead(int) { return 0; }
static void pinMode(int, int) {}
#ifndef INPUT
#define INPUT 0
#endif
#ifndef INPUT_PULLUP
#define INPUT_PULLUP 2
#endif
#endif

AarunoDigitalEngine::AarunoDigitalEngine()
    : _pin(255), _activeLow(false), _configured(false),
      _err(AARUNO_NOT_INITIALIZED) {}

AarunoError AarunoDigitalEngine::begin(uint8_t pin, bool activeLow,
                                       bool pullup) {
  AarunoError e = aarunoValidateDigitalPin(pin, false);
  if (e != AARUNO_OK) { _err = e; _configured = false; return e; }
  _pin = pin;
  _activeLow = activeLow;
#if defined(ARDUINO)
  pinMode(pin, pullup ? INPUT_PULLUP : INPUT);
#endif
  _configured = true;
  _err = AARUNO_OK;
  return AARUNO_OK;
}

int AarunoDigitalEngine::readRaw() {
  if (!_configured) { _err = AARUNO_NOT_INITIALIZED; return -1; }
#if defined(ARDUINO)
  _err = AARUNO_OK;
  return digitalRead(_pin);
#else
  _err = AARUNO_UNSUPPORTED;
  return -1;
#endif
}

bool AarunoDigitalEngine::triggered() {
  int v = readRaw();
  if (v < 0) return false;
  return _activeLow ? (v == 0) : (v == 1);
}
