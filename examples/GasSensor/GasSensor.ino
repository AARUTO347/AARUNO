#include <AARUNO.h>

// MQ-2 gas/smoke sensor (analog out). Works the same for MQ-3..MQ-135:
// just call gasMQ3(pin) ... gasMQ135(pin) instead.
// Uno: A0. ESP32: 34. ESP8266: A0 (only option).
#if defined(ESP32) || defined(ARDUINO_ARCH_ESP32)
#define MQ_PIN 34
#elif defined(ESP8266) || defined(ARDUINO_ARCH_ESP8266)
#define MQ_PIN A0
#else
#define MQ_PIN A0
#endif

void setup() {
  Serial.begin(115200);
  AARUNO.begin();
  AARUNO.gasMQ2(MQ_PIN);
}

void loop() {
  AARUNO.update();
  // Raw normalized RS/RL-style ratio. Calibrate with known gas for ppm.
  float ratio = AARUNO.readGas();
  if (AARUNO.lastError() != AARUNO_OK) {
    Serial.println(AARUNO.errorString());
  } else {
    Serial.print("Gas ratio: ");
    Serial.println(ratio);
  }
  delay(1000);
}
