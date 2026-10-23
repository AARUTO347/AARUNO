#include <AARUNO.h>

// MPU6050 IMU over I2C — real driver, no external library needed.
// Default address 0x68 (AD0 low). I2C: Uno A4/A5, ESP32 21/22, ESP8266 4/5.
void setup() {
  Serial.begin(115200);
  AARUNO.begin();
  if (AARUNO.mpu6050(0x68) != AARUNO_OK) {
    Serial.print("MPU6050 init failed: ");
    Serial.println(AARUNO.errorString());
  }
}

void loop() {
  AARUNO.update();
  float ax, ay, az;
  if (AARUNO.readAccel(ax, ay, az)) {
    Serial.print("a=");
    Serial.print(ax);
    Serial.print(",");
    Serial.print(ay);
    Serial.print(",");
    Serial.println(az);
  } else {
    Serial.println(AARUNO.errorString());
  }
  Serial.print("t=");
  Serial.println(AARUNO.readIMUTemp());
  delay(500);
}
