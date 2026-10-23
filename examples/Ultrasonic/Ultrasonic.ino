#include <AARUNO.h>

// Uno: trig=7 echo=8. ESP32: trig=5 echo=18. ESP8266: trig=5 echo=4 (D1/D2).
// Change these to match your wiring; same GPIO numbers are NOT valid everywhere.
#if defined(ESP32) || defined(ARDUINO_ARCH_ESP32)
#define TRIG_PIN 5
#define ECHO_PIN 18
#elif defined(ESP8266) || defined(ARDUINO_ARCH_ESP8266)
#define TRIG_PIN 5
#define ECHO_PIN 4
#else
#define TRIG_PIN 7
#define ECHO_PIN 8
#endif

void setup() {
  Serial.begin(115200);
  AARUNO.begin();
  if (AARUNO.ultrasonic(TRIG_PIN, ECHO_PIN) != AARUNO_OK) {
    Serial.print("Ultrasonic setup failed: ");
    Serial.println(AARUNO.errorString());
  }
  // Optional: shorten timeout for faster loop (default 30000 us).
  AARUNO.setUltrasonicTimeout(25000);
}

void loop() {
  AARUNO.update();
  float cm = AARUNO.readUltrasonic();
  if (AARUNO.lastError() != AARUNO_OK) {
    Serial.print("Sensor error: ");
    Serial.println(AARUNO.errorString());
  } else {
    Serial.print("Distance: ");
    Serial.print(cm);
    Serial.println(" cm");
  }
  delay(300);
}
