#include "AarunoHTTP.h"
#include "../AARUNOConfig.h"

#if defined(ARDUINO) && AARUNO_ENABLE_WIFI
  #if defined(ESP32) || defined(ARDUINO_ARCH_ESP32)
    #include <WiFi.h>
    #include <HTTPClient.h>
    #define AARUNO_HAS_HTTP 1
  #elif defined(ESP8266) || defined(ARDUINO_ARCH_ESP8266)
    #include <ESP8266WiFi.h>
    #include <ESP8266HTTPClient.h>
    #define AARUNO_HAS_HTTP 1
  #endif
#endif

int AarunoHTTP::get(const char *url, void *streamOut) {
#if defined(AARUNO_HAS_HTTP)
  if (!url || !url[0]) return -1;
#if defined(ESP32) || defined(ARDUINO_ARCH_ESP32)
  HTTPClient http;
  if (!http.begin(url)) return -1;
  int code = http.GET();
  if (code > 0 && streamOut) {
    Stream *out = (Stream *)streamOut;
    out->print(http.getString());
  }
  http.end();
  return code;
#else  // ESP8266
  WiFiClient client;
  HTTPClient http;
  if (!http.begin(client, url)) return -1;
  int code = http.GET();
  if (code > 0 && streamOut) {
    Stream *out = (Stream *)streamOut;
    out->print(http.getString());
  }
  http.end();
  return code;
#endif
#else
  (void)url; (void)streamOut;
  return -1;
#endif
}
