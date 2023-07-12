
#include <Arduino.h>
#include "USBCDC.h"

#define SERIAL_AT   Serial1
#define GPIO_BOOT   9
#define GPIO_RST    4

extern "C" void esp_uno_r4_setup();

extern USBCDC USBSerial;
