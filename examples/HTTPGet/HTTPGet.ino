#include <AARUNO.h>

// ESP32/ESP8266 HTTP GET demo. On Uno it prints UNSUPPORTED and stops.
const char *SSID = "your-ssid";
const char *PASS = "your-password";

void setup() {
  Serial.begin(115200);
  AARUNO.begin();
#if defined(ESP32) || defined(ESP8266) || defined(ARDUINO_ARCH_ESP32) || defined(ARDUINO_ARCH_ESP8266)
  if (AARUNO.wifiBegin(SSID, PASS) != AARUNO_OK) {
    Serial.println("WiFi failed");
    while (true) delay(1000);
  }
  int code = AARUNO.httpGet("http://example.com/", Serial);
  Serial.print("HTTP code: ");
  Serial.println(code);
#else
  Serial.println("This example needs an ESP board.");
  while (true) delay(1000);
#endif
}

void loop() {
  AARUNO.update();
  delay(5000);
}
