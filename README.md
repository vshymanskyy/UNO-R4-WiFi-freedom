# Freedom for ESP32 on Arduino UNO R4 WiFi!


If you (like me) are trying to use ESP32 on the UNO R4 WiFi for your own sketches, you'll soon learn that by re-programming the ESP32 the board loses several powerful features:
- The ability to see the `Serial` output in the serial monitor
- Renesas chip debugging

> __Note__: This project re-packages parts of the [official Arduino firmware](https://github.com/arduino/uno-r4-wifi-usb-bridge) using PlatformIO

## Getting started

1. Disconnect the UNO R4 WiFi from your PC
2. Short the pins highlighted in the image using a jumper wire:

![ESP32-S3 mode selection jumper](https://github.com/pennam/UnoR4WiFiUpdate/assets/20436476/b271759e-5d7b-44f5-954e-15bc0f7feae9)

3. Connect the UNO R4 WiFi to your PC
4. **Upload ESP32 firmware:**

```sh
pio run -e esp32 -t upload
```

5. Disconnect the UNO R4 WiFi from your PC
6. **Unplug the jumper wire**
7. Connect the board again
8. **Upload the Renesas firmware:**

```sh
pio run -e unoR4 -t upload -t monitor
```

## Expected Serial Monitor output:

```log
ESP32 says: Freedom for ESP32-S3! 🌟🌟🌟
ESP32 says: Freedom for ESP32-S3! 🇺🇦🛡️🇺🇦🛡️🇺🇦
ESP32 says: Freedom for ESP32-S3! 😎👌🔥
ESP32 says: Freedom for ESP32-S3! 🚀🌘
```
