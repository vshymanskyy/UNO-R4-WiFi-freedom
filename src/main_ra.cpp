#include <Arduino.h>

void setup()
{
  Serial.begin(115200);

  SERIAL_AT.begin(115200);
}

void loop()
{
  delay(10);

  // Read messages from ESP32
  if (SERIAL_AT.available()) {
    Serial.print("ESP32 says: ");
    while (SERIAL_AT.available()) {
      Serial.write(SERIAL_AT.read());
    }
  }
}
