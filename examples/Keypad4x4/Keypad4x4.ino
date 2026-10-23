#include <AARUNO.h>

// 4x4 matrix keypad, 8 pins (4 rows + 4 cols).
// Uno: rows 2,3,4,5 cols 6,7,8,9. ESP32: rows 13,14,15,16 cols 17,18,19,21.
#if defined(ESP32) || defined(ARDUINO_ARCH_ESP32)
#define KP_R0 13
#define KP_R1 14
#define KP_R2 15
#define KP_R3 16
#define KP_C0 17
#define KP_C1 18
#define KP_C2 19
#define KP_C3 21
#else
#define KP_R0 2
#define KP_R1 3
#define KP_R2 4
#define KP_R3 5
#define KP_C0 6
#define KP_C1 7
#define KP_C2 8
#define KP_C3 9
#endif

void setup() {
  Serial.begin(115200);
  AARUNO.begin();
  if (AARUNO.keypad4x4(KP_R0, KP_R1, KP_R2, KP_R3,
                       KP_C0, KP_C1, KP_C2, KP_C3) != AARUNO_OK) {
    Serial.println(AARUNO.errorString());
  }
}

void loop() {
  AARUNO.update();
  char k = AARUNO.readKey();  // 0 when no key pressed
  if (k) {
    Serial.print("Key: ");
    Serial.println(k);
    delay(300);  // crude key-repeat guard
  }
}
