#include <AARUNO.h>

// Rotary encoder (KY-040): CLK, DT, SW. Count updates in AARUNO.update().
// Uno: clk=2 dt=3 sw=4. ESP32: clk=32 dt=33 sw=25. ESP8266: clk=14 dt=12 sw=13.
#if defined(ESP32) || defined(ARDUINO_ARCH_ESP32)
#define ENC_CLK 32
#define ENC_DT 33
#define ENC_SW 25
#elif defined(ESP8266) || defined(ARDUINO_ARCH_ESP8266)
#define ENC_CLK 14
#define ENC_DT 12
#define ENC_SW 13
#else
#define ENC_CLK 2
#define ENC_DT 3
#define ENC_SW 4
#endif

void setup() {
  Serial.begin(115200);
  AARUNO.begin();
  AARUNO.rotaryEncoder(ENC_CLK, ENC_DT, ENC_SW);
}

void loop() {
  AARUNO.update();  // polls the encoder
  static long lastPos = 0;
  long pos = AARUNO.encoderCount();
  if (pos != lastPos) {
    lastPos = pos;
    Serial.println(pos);
  }
  if (AARUNO.encoderButton()) {
    AARUNO.encoderReset();
    Serial.println("Reset");
    delay(300);
  }
}
