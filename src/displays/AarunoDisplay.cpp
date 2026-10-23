#include "AarunoDisplay.h"
#include "../AARUNOConfig.h"

#if defined(ARDUINO) && AARUNO_ENABLE_DISPLAY
#ifdef __has_include
#if __has_include(<Adafruit_SSD1306.h>)
#include <Adafruit_SSD1306.h>
#define AARUNO_HAS_SSD1306 1
#endif
#if __has_include(<LiquidCrystal_I2C.h>)
#include <LiquidCrystal_I2C.h>
#define AARUNO_HAS_LCD 1
#endif
#endif
#if defined(AARUNO_HAS_SSD1306) || defined(AARUNO_HAS_LCD)
#include <Wire.h>
#endif
#endif

AarunoDisplay::AarunoDisplay()
    : _ready(false), _isOled(true), _cols(0), _rows(0),
      _err(AARUNO_NOT_INITIALIZED), _handle(nullptr) {}

AarunoDisplay::~AarunoDisplay() { end(); }

void AarunoDisplay::end() {
#if defined(ARDUINO) && defined(AARUNO_HAS_SSD1306)
  if (_handle && _isOled) { delete (Adafruit_SSD1306 *)_handle; _handle = nullptr; }
#endif
#if defined(ARDUINO) && defined(AARUNO_HAS_LCD)
  if (_handle && !_isOled) { delete (LiquidCrystal_I2C *)_handle; _handle = nullptr; }
#endif
  _handle = nullptr;
  _ready = false;
}

AarunoError AarunoDisplay::oled(uint8_t addr, uint8_t w, uint8_t h) {
#if defined(AARUNO_HAS_SSD1306) && defined(ARDUINO)
  if (w == 0 || h == 0) { _err = AARUNO_INVALID_CONFIG; _ready = false; return _err; }
  if (addr < 0x08 || addr > 0x77) { _err = AARUNO_INVALID_CONFIG; _ready = false; return _err; }
  end();
  Wire.begin();
  Adafruit_SSD1306 *d = new Adafruit_SSD1306(w, h, &Wire, -1);
  if (!d) { _err = AARUNO_INIT_FAIL; _ready = false; return _err; }
  if (!d->begin(SSD1306_SWITCHCAPVCC, addr)) {
    delete d;
    _err = AARUNO_INIT_FAIL;  // hardware NACK / wrong address
    _ready = false;
    return _err;
  }
  d->clearDisplay();
  d->setTextSize(1);
  d->setTextColor(SSD1306_WHITE);
  d->setCursor(0, 0);
  d->display();
  _handle = d;
  _isOled = true;
  _cols = (uint8_t)(w / 6);   // 6 px per column at text size 1
  _rows = (uint8_t)(h / 8);   // 8 px per row (one text line) at size 1
  _ready = true;
  _err = AARUNO_OK;
  return AARUNO_OK;
#else
  (void)w; (void)h;
  if (addr < 0x08 || addr > 0x77) { _err = AARUNO_INVALID_CONFIG; _ready = false; return _err; }
  _err = AARUNO_MISSING_DEP;
  _ready = false;
  return AARUNO_MISSING_DEP;
#endif
}

AarunoError AarunoDisplay::lcd(uint8_t addr, uint8_t cols, uint8_t rows) {
#if defined(AARUNO_HAS_LCD) && defined(ARDUINO)
  if (cols == 0 || rows == 0) { _err = AARUNO_INVALID_CONFIG; _ready = false; return _err; }
  if (addr < 0x08 || addr > 0x77) { _err = AARUNO_INVALID_CONFIG; _ready = false; return _err; }
  end();
  Wire.begin();
  // Probe first: if the backpack doesn't ACK, fail before init() (some
  // cores block inside init() when no device is present).
  Wire.beginTransmission(addr);
  if (Wire.endTransmission() != 0) {
    _err = AARUNO_INIT_FAIL;
    _ready = false;
    return _err;
  }
  LiquidCrystal_I2C *d = new LiquidCrystal_I2C(addr, cols, rows);
  if (!d) { _err = AARUNO_INIT_FAIL; _ready = false; return _err; }
  d->init();
  d->backlight();
  d->clear();
  d->setCursor(0, 0);
  _handle = d;
  _isOled = false;
  _cols = cols;
  _rows = rows;
  _ready = true;
  _err = AARUNO_OK;
  return AARUNO_OK;
#else
  (void)addr; (void)cols; (void)rows;
  _err = AARUNO_MISSING_DEP;
  _ready = false;
  return AARUNO_MISSING_DEP;
#endif
}

AarunoError AarunoDisplay::clear() {
  if (!_ready || !_handle) { _err = AARUNO_NOT_INITIALIZED; return _err; }
#if defined(ARDUINO) && defined(AARUNO_HAS_SSD1306)
  if (_isOled) {
    Adafruit_SSD1306 *d = (Adafruit_SSD1306 *)_handle;
    d->clearDisplay();
    d->setCursor(0, 0);
    d->display();
    _err = AARUNO_OK;
    return _err;
  }
#endif
#if defined(ARDUINO) && defined(AARUNO_HAS_LCD)
  if (!_isOled) {
    LiquidCrystal_I2C *d = (LiquidCrystal_I2C *)_handle;
    d->clear();
    d->setCursor(0, 0);
    _err = AARUNO_OK;
    return _err;
  }
#endif
  _err = AARUNO_UNSUPPORTED;
  return _err;
}

AarunoError AarunoDisplay::text(const char *line) { return text(0, line); }

AarunoError AarunoDisplay::text(int row, const char *line) {
  if (!_ready || !_handle) { _err = AARUNO_NOT_INITIALIZED; return _err; }
  if (!line) { _err = AARUNO_INVALID_CONFIG; return _err; }
  if (row < 0 || row >= (int)_rows) { _err = AARUNO_INVALID_CONFIG; return _err; }
#if defined(ARDUINO) && defined(AARUNO_HAS_SSD1306)
  if (_isOled) {
    Adafruit_SSD1306 *d = (Adafruit_SSD1306 *)_handle;
    d->setCursor(0, row * 8);
    d->print(line);
    d->display();
    _err = AARUNO_OK;
    return _err;
  }
#endif
#if defined(ARDUINO) && defined(AARUNO_HAS_LCD)
  if (!_isOled) {
    LiquidCrystal_I2C *d = (LiquidCrystal_I2C *)_handle;
    d->setCursor(0, (uint8_t)row);
    d->print(line);
    _err = AARUNO_OK;
    return _err;
  }
#endif
  _err = AARUNO_UNSUPPORTED;
  return _err;
}

AarunoError AarunoDisplay::textAt(int row, int col, const char *line) {
  if (!line) { _err = AARUNO_INVALID_CONFIG; return _err; }
  if (col < 0 || col >= (int)_cols) { _err = AARUNO_INVALID_CONFIG; return _err; }
  if (row < 0 || row >= (int)_rows) { _err = AARUNO_INVALID_CONFIG; return _err; }
#if defined(ARDUINO) && defined(AARUNO_HAS_SSD1306)
  if (_isOled) {
    Adafruit_SSD1306 *d = (Adafruit_SSD1306 *)_handle;
    d->setCursor(col * 6, row * 8);
    d->print(line);
    d->display();
    _err = AARUNO_OK;
    return _err;
  }
#endif
#if defined(ARDUINO) && defined(AARUNO_HAS_LCD)
  if (!_isOled) {
    LiquidCrystal_I2C *d = (LiquidCrystal_I2C *)_handle;
    d->setCursor((uint8_t)col, (uint8_t)row);
    d->print(line);
    _err = AARUNO_OK;
    return _err;
  }
#endif
  _err = AARUNO_UNSUPPORTED;
  return _err;
}

AarunoError AarunoDisplay::backlight(bool on) {
  if (!_ready || !_handle) { _err = AARUNO_NOT_INITIALIZED; return _err; }
#if defined(ARDUINO) && defined(AARUNO_HAS_LCD)
  if (!_isOled) {
    LiquidCrystal_I2C *d = (LiquidCrystal_I2C *)_handle;
    if (on) d->backlight();
    else d->noBacklight();
    _err = AARUNO_OK;
    return _err;
  }
#else
  (void)on;
#endif
  // OLED SSD1306 has no backlight API in this adapter.
  _err = AARUNO_OK;
  return _err;
}
