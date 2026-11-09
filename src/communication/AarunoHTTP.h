#ifndef AARUNO_HTTP_H
#define AARUNO_HTTP_H

#include <stdint.h>
#include "../AARUNOTypes.h"

// ESP-only HTTP GET helper. Streams the response body to the given Stream
// without buffering the whole body in heap memory. Supports configurable
// connect/read timeouts and a maximum response size to prevent heap
// exhaustion on large or malicious responses.
// On non-ESP boards returns -1 with AARUNO_UNSUPPORTED.
#if !defined(ARDUINO)
// Minimal host fallback so header is parseable without Arduino.h.
class Stream {
 public:
  virtual size_t write(const uint8_t *buffer, size_t size) { return 0; }
  virtual size_t write(uint8_t b) { return 0; }
};
#endif

class AarunoHTTP {
 public:
  AarunoHTTP();

  // Set connect timeout in milliseconds (default 5000).
  void setConnectTimeout(unsigned long ms);
  // Set read timeout in milliseconds (default 5000).
  void setReadTimeout(unsigned long ms);
  // Set maximum response body bytes to stream (default 4096).
  // 0 = unlimited (not recommended on memory-constrained devices).
  void setMaxResponseSize(unsigned long bytes);
  // Set the output stream for the response body. If nullptr, body is
  // discarded (headers/status still returned).
  void setStream(Stream *s);

  // Perform a GET request. Returns HTTP status code (>0) on success,
  // -1 on connection failure, -2 on timeout, -3 on response too large.
  int get(const char *url);

  // Last HTTP status code from the most recent get() call.
  int lastStatusCode() const { return _lastStatusCode; }
  // Number of body bytes streamed in the most recent get() call.
  unsigned long lastBodyBytes() const { return _lastBodyBytes; }

 private:
  unsigned long _connectTimeoutMs;
  unsigned long _readTimeoutMs;
  unsigned long _maxResponseSizeBytes;
  Stream *_stream;
  int _lastStatusCode;
  unsigned long _lastBodyBytes;
};

#endif
