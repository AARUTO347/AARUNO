#include <AARUNO.h>

// XY joystick module: VRx, VRy (analog) + SW (button, active-low).
// Uno: x=A0 y=A1 sw=2. ESP32: x=34 y=35 sw=4. ESP8266: x=A0 y=A0* sw=5.
// *ESP8266 has ONE analog input; wire X and Y through a 2-way switch or
// use an ADS1115 for both axes on ESP8266.
#if defined(ESP32) || defined(ARDUINO_ARCH_ESP32)
#define JOY_X 34
#define JOY_Y 35
#define JOY_SW 4
#elif defined(ESP8266) || defined(ARDUINO_ARCH_ESP8266)
#define JOY_X A0
#define JOY_Y A0
#define JOY_SW 5
#else
#define JOY_X A0
#define JOY_Y A1
#define JOY_SW 2
#endif

void setup() {
  Serial.begin(115200);
  AARUNO.begin();
  if (AARUNO.joystick(JOY_X, JOY_Y, JOY_SW) != AARUNO_OK) {
    Serial.println(AARUNO.errorString());
  }
}

void loop() {
  AARUNO.update();
  Serial.print("X=");
  Serial.print(AARUNO.readJoyX());
  Serial.print(" Y=");
  Serial.print(AARUNO.readJoyY());
  Serial.print(" btn=");
  Serial.println(AARUNO.readJoyButton());
  delay(200);
}
