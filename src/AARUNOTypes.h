#ifndef AARUNO_TYPES_H
#define AARUNO_TYPES_H

#include <stdint.h>

// Version (keep in sync with library.properties)
#define AARUNO_VERSION_MAJOR 1
#define AARUNO_VERSION_MINOR 1
#define AARUNO_VERSION_PATCH 1
#define AARUNO_VERSION_STRING "1.1.1"

// Error / status codes. 0 == success.
typedef enum {
  AARUNO_OK = 0,
  AARUNO_ERROR = 1,             // generic failure
  AARUNO_INVALID_PIN = 2,       // pin not usable on this board
  AARUNO_INVALID_CONFIG = 3,    // bad argument / not configured
  AARUNO_TIMEOUT = 4,           // sensor timeout (e.g. ultrasonic echo)
  AARUNO_READ_FAIL = 5,         // checksum / read failure (e.g. DHT)
  AARUNO_UNSUPPORTED = 6,       // feature not available on this board
  AARUNO_MISSING_DEP = 7,       // optional library not installed
  AARUNO_INIT_FAIL = 8,         // peripheral init failed
  AARUNO_NOT_INITIALIZED = 9    // begin() not called / module not set up
} AarunoError;

// Board families detected at compile time.
typedef enum {
  AARUNO_FAMILY_UNKNOWN = 0,
  AARUNO_FAMILY_UNO_R3 = 1,
  AARUNO_FAMILY_UNO_R4_MINIMA = 2,
  AARUNO_FAMILY_UNO_R4_WIFI = 3,
  AARUNO_FAMILY_ESP32 = 4,
  AARUNO_FAMILY_ESP8266 = 5
} AarunoFamily;

// Non-blocking WiFi connection states (serviced by AARUNO.update()).
typedef enum {
  AARUNO_WIFI_IDLE = 0,
  AARUNO_WIFI_CONNECTING = 1,
  AARUNO_WIFI_CONNECTED = 2,
  AARUNO_WIFI_FAILED = 3,
  AARUNO_WIFI_UNSUPPORTED = 4
} AarunoWifiState;

// Invalid float reading. Use isnan() to detect, or check lastError().
#ifndef AARUNO_INVALID_READING
#define AARUNO_INVALID_READING (0.0f / 0.0f)
#endif

#endif
