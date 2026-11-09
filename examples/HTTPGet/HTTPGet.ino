#include <AARUNO.h>

// ESP32/ESP8266 HTTP GET demo with non-blocking WiFi connect.
// On non-ESP boards returns AARUNO_UNSUPPORTED and stops.
const char *SSID = "your-ssid";
const char *PASS = "your-password";

void setup() {
  Serial.begin(115200);
  AARUNO.begin();
#if defined(ESP32) || defined(ESP8266) || defined(ARDUINO_ARCH_ESP32) || defined(ARDUINO_ARCH_ESP8266)
  // Non-blocking connect: start attempt, service in loop()
  if (AARUNO.wifiStart(SSID, PASS) != AARUNO_OK) {
    Serial.print("WiFi start failed: ");
    Serial.println(AARUNO.errorString());
    while (true) delay(1000);
  }
  // Configure HTTP timeouts and response size limit
  AARUNO.httpSetConnectTimeout(5000);
  AARUNO.httpSetReadTimeout(5000);
  AARUNO.httpSetMaxResponseSize(2048);  // cap at 2 KB to avoid heap exhaustion
#else
  Serial.println("This example needs an ESP board.");
  while (true) delay(1000);
#endif
}

void loop() {
  AARUNO.update();  // services WiFi connection attempt
#if defined(ESP32) || defined(ESP8266) || defined(ARDUINO_ARCH_ESP32) || defined(ARDUINO_ARCH_ESP8266)
  static bool done = false;
  if (!done && AARUNO.wifiState() == AARUNO_WIFI_CONNECTED) {
    done = true;
    Serial.print("Connected, RSSI: ");
    Serial.println(AARUNO.wifiRSSI());
    int code = AARUNO.httpGet("http://example.com/", Serial);
    Serial.print("HTTP code: ");
    Serial.println(code);
    Serial.print("Body bytes streamed: ");
    Serial.println(AARUNO.httpLastBodyBytes());
  } else if (!done && AARUNO.wifiState() == AARUNO_WIFI_FAILED) {
    Serial.println("WiFi failed (timeout).");
    while (true) delay(1000);
  }
#endif
  delay(250);
}
