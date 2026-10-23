// Compile-only check: verifies examples reference real AARUNO APIs.
// This file is built by CI per-board; it is not an Arduino sketch.
#include "AARUNO.h"
void compileCheck() {
  AARUNO.begin();
  AARUNO.update();
  AARUNO.ultrasonic(7, 8);
  AARUNO.setUltrasonicTimeout(30000);
  float cm = AARUNO.readUltrasonic();
  (void)cm;
  AARUNO.dht11(2);
  AARUNO.readTemperature();
  AARUNO.dhtFresh();
  AARUNO.ldr(A0);
  AARUNO.readLDR();
  AARUNO.isValidAnalogPin(A0);
  AARUNO.isValidPwmPin(11);
  AARUNO.button(4);
  AARUNO.buttonPressed();
  AARUNO.buttonRose();
  AARUNO.buttonFell();
  AARUNO.led(11);
  AARUNO.setLED(true);
  AARUNO.setBrightness(128);
  AARUNO.buzzer(9);
  AARUNO.beep(100);
  AARUNO.relay(10, true);
  AARUNO.setRelay(false);
  AARUNO.motorBegin(5, 6, 11);
  AARUNO.motorSpeed(100);
  AARUNO.oled();
  AARUNO.displayText(0, "hi");
  AARUNO.wifiStart("s", "p");
  AARUNO.wifiState();
  ultrasonicesp32(5, 18);
  ultrasonicesp32();
}
