#ifndef AARUNO_HTTP_H
#define AARUNO_HTTP_H

#include <stdint.h>
#include "../AARUNOTypes.h"

// ESP-only HTTP GET helper. Returns HTTP status code (>0) and appends the
// response body to the given Stream (no heap buffering of the whole body).
// On non-ESP boards returns -1 with UNSUPPORTED.
class AarunoHTTP {
 public:
  static int get(const char *url, void *streamOut);
};

#endif
