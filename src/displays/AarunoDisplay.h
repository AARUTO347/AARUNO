#ifndef AARUNO_DISPLAY_H
#define AARUNO_DISPLAY_H

#include <stdint.h>
#include "../AARUNOTypes.h"

// Optional SSD1306 OLED + HD44780 I2C LCD adapters.
// Both require third-party libs; without them every call returns
// AARUNO_MISSING_DEP instead of failing silently.
// When libs are present, oled()/lcd() perform real hardware init and
// clear()/text() actually render; INIT_FAIL is returned if hardware NACKs.
// Units: "row" = text line index (OLED: 0..h/8-1, LCD: 0..rows-1).
class AarunoDisplay {
 public:
  AarunoDisplay();
  ~AarunoDisplay();
  // SSD1306 default 128x64 @ 0x3C. Requires Adafruit_SSD1306 + Adafruit_GFX.
  AarunoError oled(uint8_t addr = 0x3C, uint8_t w = 128, uint8_t h = 64);
  // HD44780 I2C LCD. Requires LiquidCrystal_I2C.
  AarunoError lcd(uint8_t addr, uint8_t cols, uint8_t rows);
  void end();
  AarunoError clear();
  AarunoError text(const char *line);
  AarunoError text(int row, const char *line);
  AarunoError textAt(int row, int col, const char *line);
  AarunoError backlight(bool on);
  uint8_t rows() const { return _rows; }
  uint8_t cols() const { return _cols; }
  bool ready() const { return _ready; }
  AarunoError lastError() const { return _err; }

 private:
  bool _ready;
  bool _isOled;
  uint8_t _cols;
  uint8_t _rows;
  AarunoError _err;
  void *_handle;  // Adafruit_SSD1306* or LiquidCrystal_I2C*, else nullptr
};

#endif
