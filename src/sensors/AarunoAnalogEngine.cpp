#include "AarunoAnalogEngine.h"
#include "../AARUNOBoard.h"
#include "../utility/AarunoUtil.h"
#if defined(ARDUINO)
#include <Arduino.h>
#else
static int analogRead(int) { return 0; }
#endif
#include <math.h>

AarunoAnalogEngine::AarunoAnalogEngine()
    : _pin(255), _kind(AARUNO_AN_RAW), _seriesR(10000.0f), _beta(3950.0f),
      _r0(10000.0f), _t0(25.0f), _mvPerAmp(185.0f), _zeroMv(2500.0f),
      _configured(false), _err(AARUNO_NOT_INITIALIZED) {}

AarunoError AarunoAnalogEngine::begin(uint8_t pin, AarunoAnalogKind kind) {
  AarunoError e = aarunoValidateAnalogPin((int)pin);
  if (e != AARUNO_OK) { _err = e; _configured = false; return e; }
  _pin = pin;
  _kind = kind;
  _configured = true;
  _err = AARUNO_OK;
  return AARUNO_OK;
}

void AarunoAnalogEngine::setNtcParams(float seriesR, float beta, float r0,
                                      float t0C) {
  _seriesR = seriesR; _beta = beta; _r0 = r0; _t0 = t0C;
}

void AarunoAnalogEngine::setCurrentParams(float mvPerAmp, float zeroMv) {
  _mvPerAmp = mvPerAmp; _zeroMv = zeroMv;
}

int AarunoAnalogEngine::readRaw() {
  if (!_configured) { _err = AARUNO_NOT_INITIALIZED; return -1; }
#if defined(ARDUINO)
  _err = AARUNO_OK;
  return analogRead(_pin);
#else
  _err = AARUNO_UNSUPPORTED;
  return -1;
#endif
}

float AarunoAnalogEngine::readPercent() {
  int raw = readRaw();
  if (raw < 0) return AARUNO_INVALID_READING;
  float p = aarunoPercentFromRaw(raw, AARUNO_ADC_MAX);
  if (p < 0) { _err = AARUNO_READ_FAIL; return AARUNO_INVALID_READING; }
  return p;
}

float AarunoAnalogEngine::readVolts() {
  int raw = readRaw();
  if (raw < 0) return AARUNO_INVALID_READING;
  long mv = aarunoAdcToMillivolts(raw, AARUNO_ADC_MAX, AARUNO_ADC_VREF, 1.0f);
  if (mv < 0) { _err = AARUNO_READ_FAIL; return AARUNO_INVALID_READING; }
  return (float)mv / 1000.0f;
}

float AarunoAnalogEngine::readSpecial() {
  int raw = readRaw();
  if (raw < 0) return AARUNO_INVALID_READING;
  float out;
  switch (_kind) {
    case AARUNO_AN_LM35: {
      long mv = aarunoAdcToMillivolts(raw, AARUNO_ADC_MAX, AARUNO_ADC_VREF, 1.0f);
      out = aarunoLm35C(mv);
      break;
    }
    case AARUNO_AN_TMP36: {
      long mv = aarunoAdcToMillivolts(raw, AARUNO_ADC_MAX, AARUNO_ADC_VREF, 1.0f);
      out = aarunoTmp36C(mv);
      break;
    }
    case AARUNO_AN_NTC:
      out = aarunoNtcC(raw, AARUNO_ADC_MAX, _seriesR, _beta, _r0, _t0);
      break;
    case AARUNO_AN_MQ:
      out = aarunoMqRatio(raw, AARUNO_ADC_MAX);
      break;
    case AARUNO_AN_CURRENT: {
      long mv = aarunoAdcToMillivolts(raw, AARUNO_ADC_MAX, AARUNO_ADC_VREF, 1.0f);
      out = aarunoAcs712Amps(mv, _mvPerAmp, _zeroMv);
      break;
    }
    case AARUNO_AN_PLAIN_PCT:
      return readPercent();
    case AARUNO_AN_RAW:
    default:
      _err = AARUNO_OK;
      return (float)raw;
  }
  if (out < 0) { _err = AARUNO_READ_FAIL; return AARUNO_INVALID_READING; }
  _err = AARUNO_OK;
  return out;
}
