// Host C++ test for hardware-independent logic (AARUNO — MADE BY AARUTO).
// Compiles WITHOUT Arduino headers. Run: g++ -std=c++11 -Wall -Wextra -I src tests/host/test_util.cpp src/sensors/AarunoDigital.cpp src/sensors/AarunoEncoder.cpp -o /tmp/t && /tmp/t
#include <cassert>
#include <cmath>
#include <cstdio>

#include "../../src/utility/AarunoUtil.h"
#include "../../src/sensors/AarunoDigital.h"
#include "../../src/sensors/AarunoEncoder.h"

int main() {
  // Distance conversion
  assert(std::fabs(aarunoDistanceCmFromUs(580) - 10.0f) < 1e-4);
  assert(aarunoDistanceCmFromUs(0) < 0);
  assert(aarunoEchoUsFromCm(123) == 123u * 58u);
  assert(aarunoEchoUsFromCm(0) == 0);
  assert(std::fabs(aarunoDistanceInchesFromUs(148) - 1.0f) < 0.01f);

  // ADC mapping
  long mid = aarunoAdcToMillivolts(512, 1023, 5.0f, 1.0f);
  assert(mid > 2490 && mid < 2520);
  assert(aarunoAdcToMillivolts(-1, 1023, 5.0f, 1.0f) == -1);

  // Percent
  float p = aarunoPercentFromRaw(512, 1023);
  assert(p > 50.0f && p < 50.1f);
  assert(aarunoPercentFromRaw(-1, 100) < 0);

  // LM35: 500 mV -> 50 C
  assert(std::fabs(aarunoLm35C(500) - 50.0f) < 1e-4);
  // TMP36: 750 mV -> 25 C
  assert(std::fabs(aarunoTmp36C(750) - 25.0f) < 1e-4);

  // NTC: at Rntc == r0 with beta large, temp ~ t0. Choose raw so r==10000:
  // r = series*(max-raw)/raw => 10000 = 10000*(1023-raw)/raw => raw = 511.5
  float t = aarunoNtcC(512, 1023, 10000.0f, 3950.0f, 10000.0f, 25.0f);
  assert(t > 23.0f && t < 27.0f);
  assert(aarunoNtcC(0, 1023, 10000, 3950, 10000, 25) < 0);
  assert(aarunoNtcC(1023, 1023, 10000, 3950, 10000, 25) < 0);

  // ACS712 5A: 2500 mV -> 0 A; 3250 mV -> +4.054 A
  assert(std::fabs(aarunoAcs712Amps(2500, 185.0f, 2500.0f)) < 1e-4);
  assert(std::fabs(aarunoAcs712Amps(3250, 185.0f, 2500.0f) - 750.0f / 185.0f) < 1e-3);

  // MQ ratio: raw 512 -> (1023-512)/512 ~= 0.998
  float mq = aarunoMqRatio(512, 1023);
  assert(mq > 0.99f && mq < 1.01f);
  assert(aarunoMqRatio(0, 1023) < 0);

  // Debounce step helper
  unsigned long last = 0;
  int s = 1;
  s = aarunoDebounceStep(s, 0, 10, &last, 50);
  assert(s == 1);
  s = aarunoDebounceStep(s, 0, 100, &last, 50);
  assert(s == 0);

  // Button logical events: active-low press (1->0) must raise rose(), not fell().
  {
    AarunoButton b;
    assert(b.begin(4, true, true) == AARUNO_OK);
    b.injectRaw(1, 0);
    b.injectRaw(0, 10);
    assert(!b.rose() && !b.fell());
    b.injectRaw(0, 100);
    assert(b.pressed());
    assert(b.rose() && !b.fell());
    b.injectRaw(1, 200);
    assert(!b.pressed());
    assert(b.fell() && !b.rose());
  }
  {
    AarunoButton b;
    assert(b.begin(4, false, false) == AARUNO_OK);
    b.injectRaw(0, 0);
    b.injectRaw(1, 100);
    assert(b.pressed() && b.rose() && !b.fell());
  }

  // Encoder quadrature: CLK edge with DT opposite => +1, same => -1.
  {
    AarunoEncoder e;
    assert(e.begin(2, 3) == AARUNO_OK);
    e.inject(0, 0, 0);    // first sample sets lastClk
    e.inject(1, 0, 10);   // rising, dt==0 != clk => +1
    assert(e.count() == 1);
    e.inject(0, 0, 20);   // falling, dt==0 == clk => -1
    assert(e.count() == 0);
    e.inject(1, 1, 30);   // rising, dt==1 == clk => -1
    assert(e.count() == -1);
    e.reset();
    assert(e.count() == 0);
  }

  std::printf("All C++ host tests passed.\n");
  return 0;
}
