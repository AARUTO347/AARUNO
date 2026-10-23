#include <AARUNO.h>

// I2C LCD (HD44780 backpack). Requires "LiquidCrystal I2C" library.
// Common addresses: 0x27 or 0x38. Use AARUNO.i2cScan() to find yours.
#define LCD_ADDR 0x27
#define LCD_COLS 16
#define LCD_ROWS 2

void setup() {
  Serial.begin(115200);
  AARUNO.begin();
  // Fastest path: auto-detect the LCD address, then print.
  if (AARUNO.lcdAuto() == AARUNO_OK) {
    AARUNO.displayText(0, "MADE BY AARUTO");
    AARUNO.displayTextAt(1, 0, "AARUNO v1.1.1");
  } else {
    Serial.println("No LCD found. Install 'LiquidCrystal I2C' + check wiring.");
  }
}

void loop() {
  AARUNO.update();
  delay(1000);
}
