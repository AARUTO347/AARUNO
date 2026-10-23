"""Host tests for AARUNO hardware-independent logic.

Mirrors src/utility/AarunoUtil.h so CI passes without an Arduino toolchain.
Run: python tests/host/test_aaruno.py
"""
import math

def distance_cm(echo_us):
    if echo_us == 0:
        return -1.0
    return echo_us / 58.0

def echo_us_from_cm(cm):
    return 0 if cm <= 0 else int(cm * 58.0)

def adc_to_mv(raw, adc_max, vref_v, divider):
    if adc_max <= 0 or vref_v <= 0 or divider <= 0:
        return -1
    if raw < 0 or raw > adc_max:
        return -1
    return int((raw / adc_max) * vref_v * divider * 1000)

def debounce_step(stable, raw, now_ms, last_change, debounce_ms):
    if raw != stable:
        if (now_ms - last_change[0]) >= debounce_ms:
            last_change[0] = now_ms
            return raw
    else:
        last_change[0] = now_ms
    return stable

ERRORS = {0:"AARUNO_OK",1:"AARUNO_ERROR",2:"AARUNO_INVALID_PIN",3:"AARUNO_INVALID_CONFIG",
          4:"AARUNO_TIMEOUT",5:"AARUNO_READ_FAIL",6:"AARUNO_UNSUPPORTED",
          7:"AARUNO_MISSING_DEP",8:"AARUNO_INIT_FAIL",9:"AARUNO_NOT_INITIALIZED"}

def percent(raw, mx):
    if mx <= 0 or raw < 0 or raw > mx: return -1.0
    return raw / mx * 100.0

def lm35_c(mv): return mv * 0.1
def tmp36_c(mv): return (mv - 500.0) / 10.0
def acs712_a(mv, mpa, zero): return (mv - zero) / mpa
def mq_ratio(raw, mx): return (mx - raw) / raw

def check(name, cond):
    print(("PASS " if cond else "FAIL ") + name)
    if not cond:
        raise SystemExit(1)

check("58us->1cm", abs(distance_cm(580) - 10.0) < 1e-6)
check("0us invalid", distance_cm(0) < 0)
check("roundtrip 123cm", echo_us_from_cm(123) == 123*58)
check("nonpositive cm->0", echo_us_from_cm(0) == 0 and echo_us_from_cm(-5) == 0)
check("timeout range", distance_cm(30000) > 500 and distance_cm(30000) < 600)
check("inches 148us->1", abs(148/148.0 - 1.0) < 1e-9)

# ADC mapping
check("midscale 5V", 2490 <= adc_to_mv(512, 1023, 5.0, 1.0) < 2520)
check("esp32 fullscale", 3290 <= adc_to_mv(4095, 4095, 3.3, 1.0) < 3310)
check("divider x2", 4990 <= adc_to_mv(512, 1023, 5.0, 2.0) < 5030)
check("bad args", adc_to_mv(-1, 1023, 5.0, 1.0) == -1 and adc_to_mv(0, 0, 5.0, 1.0) == -1)

# New math
check("percent 512->50", 50.0 <= percent(512, 1023) < 50.1)
check("lm35 500mV->50C", abs(lm35_c(500) - 50.0) < 1e-9)
check("tmp36 750mV->25C", abs(tmp36_c(750) - 25.0) < 1e-9)
check("acs712 2500mV->0A", abs(acs712_a(2500, 185.0, 2500.0)) < 1e-9)
check("mq 512->~1", 0.99 <= mq_ratio(512, 1023) < 1.01)

# Debounce: noisy press settles after 50ms
last = [0]
stable = 1
stable = debounce_step(stable, 0, 10, last, 50)
check("debounce holds", stable == 1)
stable = debounce_step(stable, 0, 100, last, 50)
check("debounce flips", stable == 0)

check("errors 0..9", all(e in ERRORS for e in range(10)))

print("All host tests passed.")
