#include <AARUNO.h>

// XY joystick module: VRx, VRy (analog) + SW (button, active-low).
// Uno: x=A0 y=A1 sw=2. ESP32: x=34 y=35 sw=4. ESP8266: x=A0, Y needs external ADC.
// *ESP8266 has ONE analog input (A0); wire Y through an external ADC (e.g. ADS1115).
#if defined(ESP32) || defined(ARDUINO_ARCH_ESP32)
#define JOY_X 34
#define JOY_Y 35
#define JOY_SW 4
#elif defined(ESP8266) || defined(ARDUINO_ARCH_ESP8266)
#define JOY_X A0
#define JOY_Y A0  // ESP8266 has ONE analog input; Y axis needs external ADC
#define JOY_SW 5
#else
#define JOY_X A0
#define JOY_Y A1
#define JOY_SW 2
#endif

void setup() {
  Serial.begin(115200);
  AARUNO.begin();
#if defined(ESP8266) || defined(ARDUINO_ARCH_ESP8266)
  if (JOY_X == JOY_Y) {
    Serial.println("ESP8266 has single ADC; joystick Y needs external ADS1115.");
  } else if (AARUNO.joystick(JOY_X, JOY_Y, JOY_SW) != AARUNO_OK) {
    Serial.println(AARUNO.errorString());
  }
#else
  if (AARUNO.joystick(JOY_X, JOY_Y, JOY_SW) != AARUNO_OK) {
    Serial.println(AARUNO.errorString());
  }
#endif
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
