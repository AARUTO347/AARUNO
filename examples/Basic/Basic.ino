#include <AARUNO.h>

// Works on all boards. LED pin 13 is safe on Uno R3/R4; on ESP use 2.
#if defined(ESP32) || defined(ESP8266) || defined(ARDUINO_ARCH_ESP32) || defined(ARDUINO_ARCH_ESP8266)
#define DEMO_LED 2
#else
#define DEMO_LED 13
#endif

void setup() {
  Serial.begin(115200);
  AARUNO.begin();
  if (AARUNO.led(DEMO_LED) != AARUNO_OK) {
    Serial.println(AARUNO.errorString());
  }
}

void loop() {
  AARUNO.update();
  AARUNO.setLED(true);
  delay(500);
  AARUNO.setLED(false);
  delay(500);
}
