#include <AARUNO.h>

// RGB LED (common cathode) on three PWM pins.
// Uno: 9,10,11. ESP32: 25,26,27. ESP8266: 12,13,14 (PWM-capable, not 16).
#if defined(ESP32) || defined(ARDUINO_ARCH_ESP32)
#define RGB_R 25
#define RGB_G 26
#define RGB_B 27
#elif defined(ESP8266) || defined(ARDUINO_ARCH_ESP8266)
#define RGB_R 12
#define RGB_G 13
#define RGB_B 14
#else
#define RGB_R 9
#define RGB_G 10
#define RGB_B 11
#endif

void setup() {
  Serial.begin(115200);
  AARUNO.begin();
  if (AARUNO.rgb(RGB_R, RGB_G, RGB_B) != AARUNO_OK) {
    Serial.println(AARUNO.errorString());
  }
}

void loop() {
  AARUNO.update();
  AARUNO.setRGBRGB(255, 0, 0);  // red
  delay(500);
  AARUNO.setRGBRGB(0, 255, 0);  // green
  delay(500);
  AARUNO.setRGBRGB(0, 0, 255);  // blue
  delay(500);
}
