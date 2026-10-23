#include <AARUNO.h>

// Requires: DHT sensor library + Adafruit Unified Sensor (Library Manager).
// Uno: pin 2. ESP32: pin 4. ESP8266: pin 2.
#if defined(ESP32) || defined(ARDUINO_ARCH_ESP32)
#define DHT_PIN 4
#else
#define DHT_PIN 2
#endif

void setup() {
  Serial.begin(115200);
  AARUNO.begin();
  AarunoError e = AARUNO.dht11(DHT_PIN);
  if (e == AARUNO_MISSING_DEP) {
    Serial.println("Install 'DHT sensor library' + 'Adafruit Unified Sensor'.");
  } else if (e != AARUNO_OK) {
    Serial.println(AARUNO.errorString());
  }
}

void loop() {
  AARUNO.update();
  float t, h;
  if (AARUNO.readDHT(t, h)) {
    Serial.print("Temp: ");
    Serial.print(t);
    Serial.print(" C  Hum: ");
    Serial.print(h);
    Serial.println(" %");
  } else {
    Serial.print("DHT error: ");
    Serial.println(AARUNO.errorString());
  }
  delay(2000);  // DHT11 needs >=2 s between reads
}
