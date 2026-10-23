#include <AARUNO.h>

// Optional: requires Adafruit SSD1306 + Adafruit GFX libraries.
// Without them, oled() returns AARUNO_MISSING_DEP and sketch still compiles.
// If hardware NACKs (wrong address/wiring), oled() returns AARUNO_INIT_FAIL.
void setup() {
  Serial.begin(115200);
  AARUNO.begin();
  AarunoError e = AARUNO.oled(0x3C);
  if (e == AARUNO_MISSING_DEP) {
    Serial.println("Install Adafruit SSD1306 + GFX for OLED support.");
  } else if (e == AARUNO_INIT_FAIL) {
    Serial.println("OLED not found: check wiring/address.");
  } else if (e == AARUNO_OK) {
    AARUNO.displayText(0, "AARUNO ready");
    AARUNO.displayText(1, "v1.0.2");
  }
}

void loop() {
  AARUNO.update();
  delay(1000);
}
