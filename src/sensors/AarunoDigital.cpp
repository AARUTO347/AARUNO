#include "AarunoDigital.h"
#include "../AARUNOBoard.h"
#include "../AARUNOConfig.h"
#if defined(ARDUINO)
#include <Arduino.h>
#else
static unsigned long millis() { return 0; }
static int digitalRead(uint8_t) { return 0; }
static void pinMode(uint8_t, uint8_t) {}
#ifndef INPUT
#define INPUT 0
#endif
#ifndef INPUT_PULLUP
#define INPUT_PULLUP 2
#endif
#endif

AarunoButton::AarunoButton()
    : _pin(255), _activeLow(true), _stable(1), _lastRaw(1), _lastChangeMs(0),
      _roseFlag(false), _fellFlag(false), _err(AARUNO_NOT_INITIALIZED),
      _begun(false) {}

AarunoError AarunoButton::begin(uint8_t pin, bool activeLow, bool usePullup) {
  AarunoError e = aarunoValidateDigitalPin(pin, false);
  if (e != AARUNO_OK) { _err = e; _begun = false; return e; }
  _pin = pin;
  _activeLow = activeLow;
#if defined(ARDUINO)
  if (usePullup) pinMode(pin, INPUT_PULLUP);
  else pinMode(pin, INPUT);
  _lastRaw = digitalRead(pin);
#else
  (void)usePullup;
  _lastRaw = activeLow ? 1 : 0;
#endif
  _stable = _lastRaw;
  _lastChangeMs = 0;
  _roseFlag = _fellFlag = false;
  _begun = true;
  _err = AARUNO_OK;
  return AARUNO_OK;
}

void AarunoButton::update(unsigned long nowMs) {
  if (!_begun) return;
#if defined(ARDUINO)
  int raw = digitalRead(_pin);
#else
  int raw = _lastRaw;
#endif
  if (raw != _stable) {
    if ((nowMs - _lastChangeMs) >= AARUNO_BUTTON_DEBOUNCE_MS) {
      bool wasPressed = logicalPressed(_stable);
      bool nowPressed = logicalPressed(raw);
      _stable = raw;
      _lastChangeMs = nowMs;
      if (!wasPressed && nowPressed) _roseFlag = true;   // press event
      if (wasPressed && !nowPressed) _fellFlag = true;   // release event
    }
  } else {
    _lastChangeMs = nowMs;
  }
  _lastRaw = raw;
}

// Test/host hook: same debounce core with an injected raw level.
void AarunoButton::injectRaw(int raw, unsigned long nowMs) {
  if (!_begun) return;
  _lastRaw = raw;
  if (raw != _stable) {
    if ((nowMs - _lastChangeMs) >= AARUNO_BUTTON_DEBOUNCE_MS) {
      bool wasPressed = logicalPressed(_stable);
      bool nowPressed = logicalPressed(raw);
      _stable = raw;
      _lastChangeMs = nowMs;
      if (!wasPressed && nowPressed) _roseFlag = true;
      if (wasPressed && !nowPressed) _fellFlag = true;
    }
  } else {
    _lastChangeMs = nowMs;
  }
}

bool AarunoButton::pressed() const {
  if (!_begun) return false;
  return _activeLow ? (_stable == 0) : (_stable == 1);
}

bool AarunoButton::rose() {
  bool v = _roseFlag;
  _roseFlag = false;
  return v;
}

bool AarunoButton::fell() {
  bool v = _fellFlag;
  _fellFlag = false;
  return v;
}
