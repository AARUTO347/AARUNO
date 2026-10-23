#include "AarunoWifi.h"

#if defined(ARDUINO) && AARUNO_ENABLE_WIFI
  #if defined(ESP32) || defined(ARDUINO_ARCH_ESP32)
    #include <WiFi.h>
    #define AARUNO_HAS_WIFI 1
  #elif defined(ESP8266) || defined(ARDUINO_ARCH_ESP8266)
    #include <ESP8266WiFi.h>
    #define AARUNO_HAS_WIFI 1
  #endif
#endif
#include "../AARUNOConfig.h"
#if defined(ARDUINO)
#include <Arduino.h>
#else
static unsigned long millis() { return 0; }
static void delay(unsigned long) {}
#endif

AarunoWifi::AarunoWifi()
    : _state(AARUNO_WIFI_IDLE), _startMs(0), _timeoutMs(15000), _begun(false) {}

AarunoError AarunoWifi::start(const char *ssid, const char *pass,
                              unsigned long timeoutMs) {
#if defined(AARUNO_HAS_WIFI)
  if (!ssid || !ssid[0]) { _state = AARUNO_WIFI_FAILED; return AARUNO_INVALID_CONFIG; }
  if (timeoutMs == 0) timeoutMs = 15000;
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass ? pass : "");
  _startMs = millis();
  _timeoutMs = timeoutMs;
  _begun = true;
  _state = AARUNO_WIFI_CONNECTING;
  update();  // fast-path: already connected (e.g. reconnect)
  return AARUNO_OK;
#else
  (void)ssid; (void)pass; (void)timeoutMs;
  _state = AARUNO_WIFI_UNSUPPORTED;
  return AARUNO_UNSUPPORTED;
#endif
}

void AarunoWifi::update() {
#if defined(AARUNO_HAS_WIFI)
  if (!_begun || _state != AARUNO_WIFI_CONNECTING) return;
  if (WiFi.status() == WL_CONNECTED) {
    _state = AARUNO_WIFI_CONNECTED;
    return;
  }
  if ((millis() - _startMs) >= _timeoutMs) {
    _state = AARUNO_WIFI_FAILED;
  }
#endif
}

AarunoError AarunoWifi::begin(const char *ssid, const char *pass,
                              unsigned long timeoutMs) {
#if defined(AARUNO_HAS_WIFI)
  // NOTE: blocking. For responsive apps use start() + update() instead.
  AarunoError e = start(ssid, pass, timeoutMs);
  if (e != AARUNO_OK) return e;
  while (_state == AARUNO_WIFI_CONNECTING) {
    delay(250);
    update();
  }
  return (_state == AARUNO_WIFI_CONNECTED) ? AARUNO_OK : AARUNO_TIMEOUT;
#else
  (void)ssid; (void)pass; (void)timeoutMs;
  _state = AARUNO_WIFI_UNSUPPORTED;
  return AARUNO_UNSUPPORTED;
#endif
}

bool AarunoWifi::connected() {
#if defined(AARUNO_HAS_WIFI)
  if (_state == AARUNO_WIFI_CONNECTED && WiFi.status() != WL_CONNECTED) {
    _state = AARUNO_WIFI_FAILED;  // dropped after connect
  }
  return _state == AARUNO_WIFI_CONNECTED;
#else
  return false;
#endif
}

long AarunoWifi::rssi() {
#if defined(AARUNO_HAS_WIFI)
  if (!connected()) return 0;
  return WiFi.RSSI();
#else
  return 0;
#endif
}

void AarunoWifi::disconnect() {
#if defined(AARUNO_HAS_WIFI)
  WiFi.disconnect();
#endif
  _state = AARUNO_WIFI_IDLE;
  _begun = false;
}
