#ifndef AARUNO_BOARD_H
#define AARUNO_BOARD_H

#include "AARUNOTypes.h"

// ---- Compile-time board detection (never guess from GPIO numbers) ----
#if defined(ESP32) || defined(ARDUINO_ARCH_ESP32)
  #define AARUNO_BOARD_ESP32 1
  #define AARUNO_BOARD_NAME "ESP32"
  #define AARUNO_BOARD_FAMILY AARUNO_FAMILY_ESP32
#elif defined(ESP8266) || defined(ARDUINO_ARCH_ESP8266)
  #define AARUNO_BOARD_ESP8266 1
  #define AARUNO_BOARD_NAME "ESP8266"
  #define AARUNO_BOARD_FAMILY AARUNO_FAMILY_ESP8266
#elif defined(ARDUINO_UNOR4_WIFI)
  #define AARUNO_BOARD_UNO_R4_WIFI 1
  #define AARUNO_BOARD_NAME "Uno R4 WiFi"
  #define AARUNO_BOARD_FAMILY AARUNO_FAMILY_UNO_R4_WIFI
#elif defined(ARDUINO_UNOR4_MINIMA) || defined(ARDUINO_MINIMA)
  #define AARUNO_BOARD_UNO_R4_MINIMA 1
  #define AARUNO_BOARD_NAME "Uno R4 Minima"
  #define AARUNO_BOARD_FAMILY AARUNO_FAMILY_UNO_R4_MINIMA
#elif defined(__AVR_ATmega328P__) || defined(ARDUINO_AVR_UNO)
  #define AARUNO_BOARD_UNO_R3 1
  #define AARUNO_BOARD_NAME "Uno R3"
  #define AARUNO_BOARD_FAMILY AARUNO_FAMILY_UNO_R3
#else
  #define AARUNO_BOARD_UNKNOWN 1
  #define AARUNO_BOARD_NAME "Unknown"
  #define AARUNO_BOARD_FAMILY AARUNO_FAMILY_UNKNOWN
#endif

// ADC characteristics per family.
#if defined(AARUNO_BOARD_ESP32)
  #define AARUNO_ADC_BITS 12
  #define AARUNO_ADC_MAX 4095
  #define AARUNO_ADC_VREF 3.3f
#elif defined(AARUNO_BOARD_ESP8266)
  #define AARUNO_ADC_BITS 10
  #define AARUNO_ADC_MAX 1023
  #define AARUNO_ADC_VREF 3.3f
#elif defined(AARUNO_BOARD_UNO_R4_MINIMA) || defined(AARUNO_BOARD_UNO_R4_WIFI)
  #define AARUNO_ADC_BITS 14
  #define AARUNO_ADC_MAX 16383
  #define AARUNO_ADC_VREF 5.0f
#else
  // Uno R3 + unknown fallback: 10-bit AVR ADC.
  #define AARUNO_ADC_BITS 10
  #define AARUNO_ADC_MAX 1023
  #define AARUNO_ADC_VREF 5.0f
#endif

// Returns true if a pin can only be an input (e.g. ESP32 34-39).
inline bool aarunoIsInputOnlyPin(uint8_t pin) {
#if defined(AARUNO_BOARD_ESP32)
  return (pin >= 34 && pin <= 39);
#else
  (void)pin;
  return false;
#endif
}

// Returns true for ESP strapping / boot-sensitive pins (usable with care).
inline bool aarunoIsStrappingPin(uint8_t pin) {
#if defined(AARUNO_BOARD_ESP32)
  return (pin == 0 || pin == 2 || pin == 5 || pin == 12 || pin == 15);
#elif defined(AARUNO_BOARD_ESP8266)
  return (pin == 0 || pin == 2 || pin == 15 || pin == 16);
#else
  (void)pin;
  return false;
#endif
}

// Validate a digital GPIO number where detectable. Returns AarunoError.
// NOTE (unknown boards): validation is permissive by design so new variants
// still compile; the Arduino core enforces the final limits. See docs.
inline AarunoError aarunoValidateDigitalPin(uint8_t pin, bool needOutput) {
#if defined(AARUNO_BOARD_UNO_R3) || defined(AARUNO_BOARD_UNO_R4_MINIMA) || defined(AARUNO_BOARD_UNO_R4_WIFI)
  // Uno: digital 0-13, analog A0-A5 also usable as digital.
  // A0..A5 map to 14..19 on AVR, 14..19+ on Renesas; accept 0..19.
  if (pin > 19) return AARUNO_INVALID_PIN;
  (void)needOutput;
  return AARUNO_OK;
#elif defined(AARUNO_BOARD_ESP8266)
  // Common usable GPIOs: 0,1,2,3,4,5,12,13,14,15,16 (16 no PWM/interrupt limits).
  switch (pin) {
    case 0: case 1: case 2: case 3: case 4: case 5:
    case 12: case 13: case 14: case 15: case 16:
      return AARUNO_OK;
    default:
      return AARUNO_INVALID_PIN;
  }
#elif defined(AARUNO_BOARD_ESP32)
  // Reject flash pins + out-of-range; flag input-only when output needed.
  if (pin > 39) return AARUNO_INVALID_PIN;
  if (pin >= 6 && pin <= 11) return AARUNO_INVALID_PIN;  // SPI flash
  if (needOutput && aarunoIsInputOnlyPin(pin)) return AARUNO_INVALID_PIN;
  return AARUNO_OK;
#else
  (void)pin; (void)needOutput;
  return AARUNO_OK;  // unknown board: permissive fallback (documented)
#endif
}

// Validate that a pin supports PWM output on this board.
inline AarunoError aarunoValidatePwmPin(uint8_t pin) {
  AarunoError d = aarunoValidateDigitalPin(pin, true);
  if (d != AARUNO_OK) return d;
#if defined(AARUNO_BOARD_UNO_R3) || defined(AARUNO_BOARD_UNO_R4_MINIMA) || defined(AARUNO_BOARD_UNO_R4_WIFI)
  // Uno R3 + R4: hardware PWM on 3,5,6,9,10,11.
  switch (pin) {
    case 3: case 5: case 6: case 9: case 10: case 11:
      return AARUNO_OK;
    default:
      return AARUNO_INVALID_PIN;
  }
#elif defined(AARUNO_BOARD_ESP8266)
  // ESP8266: analogWrite works on most GPIOs; 16 has no PWM/interrupts.
  if (pin == 16) return AARUNO_INVALID_PIN;
  return AARUNO_OK;
#elif defined(AARUNO_BOARD_ESP32)
  // ESP32 Arduino core emulates analogWrite via LEDC on any output GPIO.
  return AARUNO_OK;  // input-only/flash already rejected above
#else
  return AARUNO_OK;  // unknown board: permissive fallback (documented)
#endif
}

// Validate that a pin is a usable analog input. Accepts Arduino pin numbers
// (including A0..Ax aliases). Returns AarunoError.
inline AarunoError aarunoValidateAnalogPin(int pin) {
  if (pin < 0) return AARUNO_INVALID_PIN;
#if defined(AARUNO_BOARD_ESP8266)
  // ESP8266 has a single ADC: A0 only. In the Arduino core A0 == 17.
  #if defined(A0)
    if (pin == (int)A0 || pin == 17) return AARUNO_OK;
  #else
    if (pin == 17 || pin == 0) return AARUNO_OK;
  #endif
  return AARUNO_INVALID_PIN;
#elif defined(AARUNO_BOARD_ESP32)
  // ADC1 (32-39, safe with WiFi) + ADC2 (0,2,4,12-15,25-27, WiFi conflicts).
  switch (pin) {
    case 32: case 33: case 34: case 35: case 36: case 37: case 38: case 39:
    case 0: case 2: case 4: case 12: case 13: case 14: case 15:
    case 25: case 26: case 27:
      return AARUNO_OK;
    default:
      return AARUNO_INVALID_PIN;
  }
#elif defined(AARUNO_BOARD_UNO_R3) || defined(AARUNO_BOARD_UNO_R4_MINIMA) || defined(AARUNO_BOARD_UNO_R4_WIFI)
  // Accept A0..A5 aliases (14..19) and channel numbers 0..5.
  #if defined(A0) && defined(A5)
    if (pin >= (int)A0 && pin <= (int)A5) return AARUNO_OK;
  #endif
  if ((pin >= 14 && pin <= 19) || (pin >= 0 && pin <= 5)) return AARUNO_OK;
  return AARUNO_INVALID_PIN;
#else
  (void)pin;
  return AARUNO_OK;  // unknown board: permissive fallback (documented)
#endif
}

#endif
