#ifndef AARUNO_WIFI_H
#define AARUNO_WIFI_H

#include <stdint.h>
#include "../AARUNOTypes.h"

// ESP-only WiFi helper. On non-ESP boards every call returns
// AARUNO_UNSUPPORTED / AARUNO_WIFI_UNSUPPORTED. Never include WiFi
// headers in portable builds.
//
// Two flavors:
//  - begin(ssid, pass, timeout): blocking helper (delays up to timeout).
//    Documented as blocking; prefer start()+update() in responsive apps.
//  - start(ssid, pass, timeout) + update() + state(): non-blocking,
//    serviced from AARUNO.update().
class AarunoWifi {
 public:
  AarunoWifi();
  // Blocking (delays in 250 ms slices until timeout).
  AarunoError begin(const char *ssid, const char *pass,
                    unsigned long timeoutMs = 15000);
  // Non-blocking: returns OK when the attempt started.
  AarunoError start(const char *ssid, const char *pass,
                    unsigned long timeoutMs = 15000);
  void update();  // call from AARUNO.update()
  AarunoWifiState state() const { return _state; }
  bool connected();
  long rssi();  // dBm, or 0 when not connected / unsupported
  void disconnect();
  // Configure retry behavior. retries=0 means no retry after first attempt.
  // retryDelayMs is the delay between retries.
  void setRetryPolicy(uint8_t retries, unsigned long retryDelayMs);
  // Number of reconnection attempts made so far (reset on new start()).
  uint8_t retryCount() const { return _retryCount; }

 private:
  AarunoWifiState _state;
  unsigned long _startMs;
  unsigned long _timeoutMs;
  unsigned long _retryDelayMs;
  uint8_t _maxRetries;
  uint8_t _retryCount;
  bool _begun;
};

#endif
