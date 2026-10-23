#include "AarunoKeypad.h"
#include "../AARUNOBoard.h"
#if defined(ARDUINO)
#include <Arduino.h>
#else
static void pinMode(int, int) {}
static void digitalWrite(int, int) {}
static int digitalRead(int) { return 1; }
static void delayMicroseconds(unsigned int) {}
#ifndef OUTPUT
#define OUTPUT 1
#endif
#ifndef INPUT_PULLUP
#define INPUT_PULLUP 2
#endif
#ifndef HIGH
#define HIGH 1
#endif
#ifndef LOW
#define LOW 0
#endif
#endif

static const char AARUNO_KEYMAP[4][4] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}};

AarunoKeypad::AarunoKeypad()
    : _begun(false), _err(AARUNO_NOT_INITIALIZED) {
  for (int i = 0; i < 4; i++) { _rows[i] = 255; _cols[i] = 255; }
}

AarunoError AarunoKeypad::begin(uint8_t r0, uint8_t r1, uint8_t r2, uint8_t r3,
                                uint8_t c0, uint8_t c1, uint8_t c2,
                                uint8_t c3) {
  uint8_t pins[8] = {r0, r1, r2, r3, c0, c1, c2, c3};
  for (int i = 0; i < 8; i++) {
    for (int j = i + 1; j < 8; j++) {
      if (pins[i] == pins[j]) { _err = AARUNO_INVALID_CONFIG; _begun = false; return _err; }
    }
  }
  for (int i = 0; i < 8; i++) {
    AarunoError e = aarunoValidateDigitalPin(pins[i], true);
    if (e != AARUNO_OK) { _err = e; _begun = false; return e; }
  }
  _rows[0] = r0; _rows[1] = r1; _rows[2] = r2; _rows[3] = r3;
  _cols[0] = c0; _cols[1] = c1; _cols[2] = c2; _cols[3] = c3;
#if defined(ARDUINO)
  for (int i = 0; i < 4; i++) pinMode(_rows[i], OUTPUT);
  for (int i = 0; i < 4; i++) pinMode(_cols[i], INPUT_PULLUP);
#endif
  _begun = true;
  _err = AARUNO_OK;
  return AARUNO_OK;
}

char AarunoKeypad::readKey() {
  if (!_begun) { _err = AARUNO_NOT_INITIALIZED; return 0; }
#if defined(ARDUINO)
  for (int r = 0; r < 4; r++) {
    digitalWrite(_rows[r], LOW);
    delayMicroseconds(10);
    for (int c = 0; c < 4; c++) {
      if (digitalRead(_cols[c]) == LOW) {
        digitalWrite(_rows[r], HIGH);
        return AARUNO_KEYMAP[r][c];
      }
    }
    digitalWrite(_rows[r], HIGH);
  }
  _err = AARUNO_OK;
  return 0;
#else
  _err = AARUNO_UNSUPPORTED;
  return 0;
#endif
}
