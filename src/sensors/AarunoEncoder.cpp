#include "AarunoEncoder.h"
#include "../AARUNOBoard.h"
#if defined(ARDUINO)
#include <Arduino.h>
#else
static unsigned long millis() { return 0; }
static int digitalRead(int) { return 0; }
static void pinMode(int, int) {}
#ifndef INPUT_PULLUP
#define INPUT_PULLUP 2
#endif
#ifndef HIGH
#define HIGH 1
#endif
#endif

AarunoEncoder::AarunoEncoder()
    : _clk(255), _dt(255), _sw(-1), _lastClk(-1), _count(0), _lastMs(0),
      _begun(false), _err(AARUNO_NOT_INITIALIZED) {}

AarunoError AarunoEncoder::begin(uint8_t clkPin, uint8_t dtPin, int swPin) {
  AarunoError e = aarunoValidateDigitalPin(clkPin, false);
  if (e != AARUNO_OK) { _err = e; _begun = false; return e; }
  e = aarunoValidateDigitalPin(dtPin, false);
  if (e != AARUNO_OK) { _err = e; _begun = false; return e; }
  if (swPin >= 0) {
    e = aarunoValidateDigitalPin((uint8_t)swPin, false);
    if (e != AARUNO_OK) { _err = e; _begun = false; return e; }
    if (swPin == (int)clkPin || swPin == (int)dtPin) {
      _err = AARUNO_INVALID_CONFIG; _begun = false; return _err;
    }
  }
  _clk = clkPin;
  _dt = dtPin;
  _sw = swPin;
#if defined(ARDUINO)
  pinMode(clkPin, INPUT_PULLUP);
  pinMode(dtPin, INPUT_PULLUP);
  if (swPin >= 0) pinMode((uint8_t)swPin, INPUT_PULLUP);
#endif
  _lastClk = -1;  // force first-sample edge skip
  _count = 0;
  _begun = true;
  _err = AARUNO_OK;
  return AARUNO_OK;
}

void AarunoEncoder::update() { update(millis()); }

void AarunoEncoder::update(unsigned long nowMs) {
  (void)nowMs;
  if (!_begun) return;
#if defined(ARDUINO)
  int clk = digitalRead(_clk);
  if (_lastClk == -1) { _lastClk = clk; return; }
  if (clk != _lastClk) {
    // Direction: CLK edge with DT opposite => one way, same => other.
    int dt = digitalRead(_dt);
    if (dt != clk) _count++;
    else _count--;
    _lastClk = clk;
  }
#endif
}

bool AarunoEncoder::buttonPressed() {
  if (!_begun || _sw < 0) return false;
#if defined(ARDUINO)
  return digitalRead(_sw) == LOW;  // active-low with pull-up
#else
  return false;
#endif
}

void AarunoEncoder::inject(uint8_t clk, uint8_t dt, unsigned long nowMs) {
  (void)dt; (void)nowMs;
  if (!_begun) return;
  if (_lastClk == -1) { _lastClk = clk; return; }
  if ((int)clk != _lastClk) {
    if ((int)dt != (int)clk) _count++;
    else _count--;
    _lastClk = clk;
  }
}
