#include <AARUNO.h>

// Button press toggles LED; release beeps. Brightness uses a PWM-capable pin.
// Uno: button=4 led=11 (PWM) buzzer=9. ESP32: button=0 led=2 buzzer=15.
// ESP8266: button=0 led=14 (PWM) buzzer=12.
#if defined(ESP32) || defined(ARDUINO_ARCH_ESP32)
#define BTN_PIN 0
#define LED_PIN 2
#define BUZZER_PIN 15
#elif defined(ESP8266) || defined(ARDUINO_ARCH_ESP8266)
#define BTN_PIN 0
#define LED_PIN 14
#define BUZZER_PIN 12
#else
#define BTN_PIN 4
#define LED_PIN 11
#define BUZZER_PIN 9
#endif

void setup() {
  Serial.begin(115200);
  AARUNO.begin();
  if (AARUNO.button(BTN_PIN) != AARUNO_OK) Serial.println(AARUNO.errorString());
  if (AARUNO.led(LED_PIN) != AARUNO_OK) Serial.println(AARUNO.errorString());
  if (AARUNO.buzzer(BUZZER_PIN) != AARUNO_OK) Serial.println(AARUNO.errorString());
}

void loop() {
  AARUNO.update();
  if (AARUNO.buttonRose()) {  // logical press event
    Serial.println("Pressed");
    AARUNO.toggleLED();
    AARUNO.beep(2000, 100);
  }
  if (AARUNO.buttonFell()) {  // logical release event
    Serial.println("Released");
  }
  if (AARUNO.setBrightness(128) == AARUNO_INVALID_PIN) {
    Serial.println("LED pin has no PWM on this board");
    delay(1000);
  }
  delay(20);
}
