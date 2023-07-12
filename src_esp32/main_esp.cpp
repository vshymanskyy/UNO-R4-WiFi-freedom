
#include <esp_uno_r4.h>

const uint32_t message_interval = 5000;

const char* emojis[8] = {
  "🌟🌟🌟", "😎👌🔥", "✅", "⚔️🇺🇦🛡️🇺🇦🛡️🇺🇦⚔️",
  "🤓", "¯\\_(ツ)_/¯", "🚀🌘", "🤯"
};

void setup()
{
  esp_uno_r4_setup();
}

void loop()
{
  delay(10);

  const uint32_t now = millis();
  static uint32_t last_message = 0;
  if (now - last_message > message_interval) {
    last_message += message_interval;

    // Send a message to Renesas chip
    SERIAL_AT.printf("Freedom for ESP32-S3! %s\r\n", emojis[random(8)]);
  }
}
