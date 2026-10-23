#include <AARUNO.h>

// LDR voltage divider to analog input.
// Uno: A0. ESP32: 34 (input-only ADC). ESP8266: A0.
#if defined(ESP32) || defined(ARDUINO_ARCH_ESP32)
#define LDR_PIN 34
#define LED_PIN 2
#elif defined(ESP8266) || defined(ARDUINO_ARCH_ESP8266)
#define LDR_PIN A0
#define LED_PIN 2
#else
#define LDR_PIN A0
#define LED_PIN 13
#endif

void setup() {
  Serial.begin(115200);
  AARUNO.begin();
  AARUNO.ldr(LDR_PIN);
  AARUNO.led(LED_PIN);
}

void loop() {
  AARUNO.update();
  int light = AARUNO.readLDR();
  Serial.print("LDR raw: ");
  Serial.println(light);
  // Raw ADC counts are NOT lux. Threshold depends on your divider + board.
  AARUNO.setLED(light < 500);
  delay(300);
}
