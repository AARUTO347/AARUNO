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

AarunoHTTP::AarunoHTTP()
    : _connectTimeoutMs(5000),
      _readTimeoutMs(5000),
      _maxResponseSizeBytes(4096),
      _stream(nullptr),
      _lastStatusCode(0),
      _lastBodyBytes(0) {}

void AarunoHTTP::setConnectTimeout(unsigned long ms) {
  _connectTimeoutMs = ms;
}

void AarunoHTTP::setReadTimeout(unsigned long ms) {
  _readTimeoutMs = ms;
}

void AarunoHTTP::setMaxResponseSize(unsigned long bytes) {
  _maxResponseSizeBytes = bytes;
}

void AarunoHTTP::setStream(Stream *s) {
  _stream = s;
}

int AarunoHTTP::get(const char *url) {
#if defined(AARUNO_HAS_HTTP)
  if (!url || !url[0]) return -1;

  _lastStatusCode = 0;
  _lastBodyBytes = 0;

#if defined(ESP32) || defined(ARDUINO_ARCH_ESP32)
  HTTPClient http;
  http.setConnectTimeout(_connectTimeoutMs);
  http.setReadTimeout(_readTimeoutMs);
  if (!http.begin(url)) {
    _lastStatusCode = 0;
    http.end();
    return -1;  // connection failure
  }
  int code = http.GET();
  if (code <= 0) {
    _lastStatusCode = 0;
    http.end();
    return -1;  // connection failure
  }
  _lastStatusCode = code;

  // Stream the response body in chunks, respecting max size.
  // readBytes() blocks with the configured read timeout; returns 0 on
  // connection close or timeout.
  Stream *stream = http.getStreamPtr();
  if (stream && _stream) {
    unsigned long remaining = _maxResponseSizeBytes;
    uint8_t buf[128];
    while (true) {
      size_t toRead = sizeof(buf);
      if (remaining != 0 && toRead > remaining) toRead = (size_t)remaining;
      if (toRead == 0) {
        // Response too large — stop reading but keep the status code.
        http.end();
        return -3;  // response too large
      }
      size_t got = stream->readBytes(buf, toRead);
      if (got == 0) break;
      _stream->write(buf, got);
      _lastBodyBytes += got;
      if (remaining != 0) remaining -= got;
    }
  }
  http.end();
  return code;

#else  // ESP8266
  WiFiClient client;
  client.setTimeout(_readTimeoutMs);
  HTTPClient http;
  if (!http.begin(client, url)) {
    _lastStatusCode = 0;
    http.end();
    return -1;
  }
  http.setTimeout(_readTimeoutMs);
  int code = http.GET();
  if (code <= 0) {
    _lastStatusCode = 0;
    http.end();
    return -1;
  }
  _lastStatusCode = code;

  Stream *stream = http.getStreamPtr();
  if (stream && _stream) {
    unsigned long remaining = _maxResponseSizeBytes;
    uint8_t buf[128];
    while (true) {
      size_t toRead = sizeof(buf);
      if (remaining != 0 && toRead > remaining) toRead = (size_t)remaining;
      if (toRead == 0) {
        http.end();
        return -3;
      }
      size_t got = stream->readBytes(buf, toRead);
      if (got == 0) break;
      _stream->write(buf, got);
      _lastBodyBytes += got;
      if (remaining != 0) remaining -= got;
    }
  }
  http.end();
  return code;
#endif
#else
  (void)url;
  _lastStatusCode = 0;
  _lastBodyBytes = 0;
  return -1;
#endif
}
