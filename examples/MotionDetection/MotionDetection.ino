#include <AARUNO.h>

// PIR motion sensor (digital HIGH on motion).
// Uno: pir=3 led=13 buzzer=9. ESP32: pir=13 led=2 buzzer=15.
#if defined(ESP32) || defined(ARDUINO_ARCH_ESP32)
#define PIR_PIN 13
#define LED_PIN 2
#define BUZZER_PIN 15
#elif defined(ESP8266) || defined(ARDUINO_ARCH_ESP8266)
#define PIR_PIN 5
#define LED_PIN 2
#define BUZZER_PIN 14
#else
#define PIR_PIN 3
#define LED_PIN 13
#define BUZZER_PIN 9
#endif

void setup() {
  Serial.begin(115200);
  AARUNO.begin();
  AARUNO.pir(PIR_PIN);
  AARUNO.led(LED_PIN);
  AARUNO.buzzer(BUZZER_PIN);
}

void loop() {
  AARUNO.update();
  if (AARUNO.readPIR()) {
    Serial.println("Motion!");
    AARUNO.setLED(true);
    AARUNO.beep(2000, 150);
  } else {
    AARUNO.setLED(false);
  }
  delay(200);
}
