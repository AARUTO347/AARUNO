# Pin Reference

Never assume GPIO numbers transfer between boards.

## Arduino Uno R3 / R4 (Minima & WiFi)

- Digital: 0–13 (0/1 = Serial, avoid unless needed).
- Analog as digital: 14–19 (A0–A5).
- Analog in: A0–A5.
- PWM: 3, 5, 6, 9, 10, 11 (R4 has more; these are safe everywhere).
- I2C: A4 (SDA), A5 (SCL).
- Built-in LED: 13.
- Safe starter: ultrasonic (7, 8), DHT (2), button (4), LED (13), buzzer (9), relay (10).

## ESP32 (DevKit, 30-pin)

- Safe GPIO: 4, 5, 13, 14, 15, 16, 17, 18, 19, 21, 22, 23, 25, 26, 27, 32, 33.
- Input-only (no output/PWM): 34, 35, 36, 39 — good for LDR/PIR, bad for LED/relay/trig.
- Forbidden: 6–11 (flash).
- Strapping (boot-sensitive, usable with care): 0, 2, 5, 12, 15.
- ADC: prefer ADC1 (32–39); ADC2 conflicts with WiFi on some cores.
- I2C default: SDA 21, SCL 22.
- Safe starter: ultrasonic (5, 18), DHT (4), LDR (34), button (0), LED (2), buzzer (15), relay (16).

## ESP8266 (NodeMCU / Wemos D1 mini)

- Usable: 0, 1, 2, 3, 4, 5, 12, 13, 14, 15, 16 (labels D0–D8 differ by board; use GPIO numbers).
- Boot-sensitive: 0, 2, 15 (keep HIGH/LOW as required at boot), 16 (no PWM/interrupt, HIGH at boot).
- Analog: A0 only.
- I2C default: SDA 4, SCL 5.
- Safe starter: ultrasonic trig 5 (D1) echo 4 (D2), DHT 2 (D4), LED 2, buzzer 14 (D5), relay 12 (D6).
