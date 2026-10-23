#include <AARUNO.h>

// ESP32 WiFi demo with non-blocking connect (loop keeps polling sensors).
// wifiBegin() is blocking; wifiStart() + update() is preferred.
const char *SSID = "your-ssid";
const char *PASS = "your-password";

void setup() {
  Serial.begin(115200);
  AARUNO.begin();
#if defined(ESP32) || defined(ARDUINO_ARCH_ESP32)
  if (AARUNO.wifiStart(SSID, PASS) != AARUNO_OK) {
    Serial.print("WiFi start failed: ");
    Serial.println(AARUNO.errorString());
  }
#else
  Serial.println("This example needs an ESP32 board.");
#endif
}

void loop() {
  AARUNO.update();  // services the wifiStart() attempt
#if defined(ESP32) || defined(ARDUINO_ARCH_ESP32)
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
