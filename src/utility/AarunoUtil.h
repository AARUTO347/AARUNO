#ifndef AARUNO_UTIL_H
#define AARUNO_UTIL_H

#include <stdint.h>

// Pure, hardware-independent helpers. Safe for host unit tests.
// AARUNO — MADE BY AARUTO

#ifdef __cplusplus
extern "C" {
#endif

// HC-SR04: distance_cm = echo_us / 58.0. Returns negative on invalid input.
static inline float aarunoDistanceCmFromUs(unsigned long echoUs) {
  if (echoUs == 0) return -1.0f;
  return (float)echoUs / 58.0f;
}

// Inverse: expected echo microseconds for a distance.
static inline unsigned long aarunoEchoUsFromCm(float cm) {
  if (cm <= 0.0f) return 0;
  return (unsigned long)(cm * 58.0f);
}

// Inches: echo_us / 148 (HC-SR04 datasheet approximation).
static inline float aarunoDistanceInchesFromUs(unsigned long echoUs) {
  if (echoUs == 0) return -1.0f;
  return (float)echoUs / 148.0f;
}

// Map raw ADC to millivolts given adcMax and vref. Returns -1 on bad args.
static inline long aarunoAdcToMillivolts(long raw, long adcMax, float vrefV,
                                         float dividerRatio) {
  if (adcMax <= 0 || vrefV <= 0.0f || dividerRatio <= 0.0f) return -1;
  if (raw < 0 || raw > adcMax) return -1;
  float v = ((float)raw / (float)adcMax) * vrefV * dividerRatio;
  return (long)(v * 1000.0f);
}

// Percent 0-100 from raw ADC. Returns -1 on bad args.
static inline float aarunoPercentFromRaw(long raw, long adcMax) {
  if (adcMax <= 0 || raw < 0 || raw > adcMax) return -1.0f;
  return ((float)raw / (float)adcMax) * 100.0f;
}

// LM35: 10 mV per degree C.
static inline float aarunoLm35C(long mv) {
  if (mv < 0) return -1.0f;
  return (float)mv * 0.1f;
}

// TMP36: 500 mV offset, 10 mV per degree C.
static inline float aarunoTmp36C(long mv) {
  if (mv < 0) return -1.0f;
  return ((float)mv - 500.0f) / 10.0f;
}

// NTC thermistor (beta equation). seriesR = fixed resistor to GND with
// NTC to VCC (or invert wiring for opposite direction). r0 at t0.
static inline float aarunoNtcC(long raw, long adcMax, float seriesR,
                               float beta, float r0, float t0C) {
  if (adcMax <= 0 || raw <= 0 || raw >= adcMax || seriesR <= 0 || r0 <= 0 ||
      beta <= 0)
    return -1.0f;
  float r = seriesR * ((float)adcMax - (float)raw) / (float)raw;
  if (r <= 0) return -1.0f;
  float inv = 1.0f / (t0C + 273.15f) +
              (1.0f / beta) * logf(r / r0);
  return (1.0f / inv) - 273.15f;
}

// ACS712 hall current sensor: A = (Vout - zeroV) / sensitivity.
static inline float aarunoAcs712Amps(long mv, float mvPerAmp, float zeroMv) {
  if (mv < 0 || mvPerAmp <= 0) return -1.0f;
  return ((float)mv - zeroMv) / mvPerAmp;
}

// MQ-series gas sensor: RS/RL-style ratio (needs datasheet calibration for
// ppm; this is the raw normalized ratio). ratio = (max-raw)/raw.
static inline float aarunoMqRatio(long raw, long adcMax) {
  if (adcMax <= 0 || raw <= 0 || raw >= adcMax) return -1.0f;
  return (float)(adcMax - raw) / (float)raw;
}

// Debounce step: returns new stable state. Caller tracks lastChangeMs.
// stable: current debounced value; raw: latest reading; nowMs: millis().
static inline int aarunoDebounceStep(int stable, int raw, unsigned long nowMs,
                                     unsigned long *lastChangeMs,
                                     unsigned long debounceMs) {
  if (raw != stable) {
    if ((nowMs - *lastChangeMs) >= debounceMs) {
      *lastChangeMs = nowMs;
      return raw;
    }
  } else {
    *lastChangeMs = nowMs;
  }
  return stable;
}

static inline const char *aarunoErrorName(int code) {
  switch (code) {
    case AARUNO_OK: return "AARUNO_OK";
    case AARUNO_ERROR: return "AARUNO_ERROR";
    case AARUNO_INVALID_PIN: return "AARUNO_INVALID_PIN";
    case AARUNO_INVALID_CONFIG: return "AARUNO_INVALID_CONFIG";
    case AARUNO_TIMEOUT: return "AARUNO_TIMEOUT";
    case AARUNO_READ_FAIL: return "AARUNO_READ_FAIL";
    case AARUNO_UNSUPPORTED: return "AARUNO_UNSUPPORTED";
    case AARUNO_MISSING_DEP: return "AARUNO_MISSING_DEP";
    case AARUNO_INIT_FAIL: return "AARUNO_INIT_FAIL";
    case AARUNO_NOT_INITIALIZED: return "AARUNO_NOT_INITIALIZED";
    default: return "UNKNOWN";
  }
}

#ifdef __cplusplus
}
#endif

// math.h for logf in NTC (kept outside extern "C" guard for C++ linkers)
#include <math.h>

#endif
