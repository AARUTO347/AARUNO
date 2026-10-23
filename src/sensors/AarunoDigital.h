#ifndef AARUNO_DIGITAL_H
#define AARUNO_DIGITAL_H

#include <stdint.h>
#include "../AARUNOTypes.h"

// Debounced push-button helper (non-blocking).
// pressed() is logical (accounts for activeLow).
// rose() == press event, fell() == release event (both logical, edge-cleared).
class AarunoButton {
 public:
  AarunoButton();
  AarunoError begin(uint8_t pin, bool activeLow = true, bool usePullup = true);
  void update(unsigned long nowMs);  // call from AARUNO.update() or loop()
  // Test-only hook: inject a raw reading (bypasses digitalRead).
  void injectRaw(int raw, unsigned long nowMs);
  int read() const { return _stable; }          // debounced level (0/1)
  bool pressed() const;                          // true if logically pressed
  bool rose();                                   // press event (clears flag)
  bool fell();                                   // release event (clears flag)
  bool pressedEvent() { return rose(); }         // alias: press
  bool releasedEvent() { return fell(); }        // alias: release
  bool begun() const { return _begun; }
  AarunoError lastError() const { return _err; }

 private:
  bool logicalPressed(int level) const {
    return _activeLow ? (level == 0) : (level == 1);
  }
  uint8_t _pin;
  bool _activeLow;
  int _stable;
  int _lastRaw;
  unsigned long _lastChangeMs;
  bool _roseFlag;   // press event pending
  bool _fellFlag;   // release event pending
  AarunoError _err;
  bool _begun;
};

#endif
