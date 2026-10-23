#include <AARUNO.h>

// ESP8266 WiFi demo with non-blocking connect.
const char *SSID = "your-ssid";
const char *PASS = "your-password";

void setup() {
  Serial.begin(115200);
  AARUNO.begin();
#if defined(ESP8266) || defined(ARDUINO_ARCH_ESP8266)
  if (AARUNO.wifiStart(SSID, PASS) != AARUNO_OK) {
    Serial.print("WiFi start failed: ");
    Serial.println(AARUNO.errorString());
  }
#else
  Serial.println("This example needs an ESP8266 board.");
#endif
}

void loop() {
  AARUNO.update();
#if defined(ESP8266) || defined(ARDUINO_ARCH_ESP8266)
  static bool reported = false;
  if (AARUNO.wifiState() == AARUNO_WIFI_CONNECTED && !reported) {
    reported = true;
    Serial.print("Connected RSSI: ");
    Serial.println(AARUNO.wifiRSSI());
  } else if (AARUNO.wifiState() == AARUNO_WIFI_FAILED && !reported) {
    reported = true;
    Serial.println("WiFi failed (timeout).");
  }
#endif
  delay(250);
}
