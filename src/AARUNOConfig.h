#ifndef AARUNO_CONFIG_H
#define AARUNO_CONFIG_H

// User-tunable defaults. Override by defining these before #include <AARUNO.h>
// or by editing this file in a local copy.

// Ultrasonic echo timeout (microseconds). 30000 us ~= 5.1 m max range.
#ifndef AARUNO_ULTRASONIC_TIMEOUT_US
#define AARUNO_ULTRASONIC_TIMEOUT_US 30000UL
#endif

// DHT minimum sampling interval (DHT11 needs ~1000-2000 ms).
#ifndef AARUNO_DHT_MIN_INTERVAL_MS
#define AARUNO_DHT_MIN_INTERVAL_MS 2000UL
#endif

// Button debounce interval.
#ifndef AARUNO_BUTTON_DEBOUNCE_MS
#define AARUNO_BUTTON_DEBOUNCE_MS 50UL
#endif

// Enable debug log output. 0 = off, 1 = on (requires setDebugStream).
#ifndef AARUNO_ENABLE_DEBUG
#define AARUNO_ENABLE_DEBUG 0
#endif

// Feature guards for optional dependencies. The library auto-disables a
// module with AARUNO_MISSING_DEP when the third-party header is absent,
// so users never need to edit these.
// Servo.h presence is auto-detected (no macro needed).
#ifndef AARUNO_ENABLE_DHT
#define AARUNO_ENABLE_DHT 1
#endif
#ifndef AARUNO_ENABLE_DISPLAY
#define AARUNO_ENABLE_DISPLAY 1
#endif
#ifndef AARUNO_ENABLE_WIFI
#define AARUNO_ENABLE_WIFI 1
#endif

#endif
