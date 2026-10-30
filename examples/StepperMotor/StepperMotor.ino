#include <AARUNO.h>

// 28BYJ-48 stepper via ULN2003 driver (IN1..IN4). step() BLOCKS per step.
// Uno: 8,9,10,11. ESP32: 26,27,14,12. ESP8266: 5,4,14,12 (avoid GPIO0/GPIO2 boot pins).
#if defined(ESP32) || defined(ARDUINO_ARCH_ESP32)
#define ST_IN1 26
#define ST_IN2 27
#define ST_IN3 14
#define ST_IN4 12
#elif defined(ESP8266) || defined(ARDUINO_ARCH_ESP8266)
#define ST_IN1 5
#define ST_IN2 4
#define ST_IN3 14
#define ST_IN4 12  // Avoid GPIO0/GPIO2 (boot strapping pins)
// WARNING: GPIO0 and GPIO2 are ESP8266 boot strapping pins. Using them for
// stepper IN3/IN4 can prevent normal boot depending on wiring/power-up state.
// Use GPIO5, GPIO4, GPIO14, GPIO12 (D1, D2, D5, D6) instead where possible.
#else
#define ST_IN1 8
#define ST_IN2 9
#define ST_IN3 10
#define ST_IN4 11
#endif

void setup() {
  Serial.begin(115200);
  AARUNO.begin();
  if (AARUNO.stepperAttach(ST_IN1, ST_IN2, ST_IN3, ST_IN4) != AARUNO_OK) {
    Serial.println(AARUNO.errorString());
  }
  AARUNO.stepperSetRpm(10);
}

void loop() {
  AARUNO.update();
  Serial.println("1 turn...");
  // 2048 half-steps = 1 full revolution (28BYJ-48 gearbox).
  AARUNO.stepperStep(2048);
  delay(1000);
  Serial.println("half back...");
  AARUNO.stepperStep(-1024);
  delay(1000);
}
