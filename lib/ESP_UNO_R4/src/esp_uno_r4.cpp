
#include "esp_uno_r4.h"

#include "USB.h"
#include "DAP.h"

//#define DEBUG_AT

#define SERIAL_USER            USBSerial
#define SERIAL_DEBUG           USBSerial
#define SERIAL_USER_INTERNAL   Serial

static void usbEventCallback(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data);


static uint32_t _baud = 0;

USBCDC USBSerial(0);

static TaskHandle_t cdcTask;
static void cdcLoop(void* param);

/* -------------------------------------------------------------------------- */
void esp_uno_r4_setup() {
/* -------------------------------------------------------------------------- */

  pinMode(GPIO_BOOT, OUTPUT);
  pinMode(GPIO_RST, OUTPUT);
  digitalWrite(GPIO_BOOT, HIGH);
  digitalWrite(GPIO_RST, HIGH);

#ifdef DEBUG_AT
  SERIAL_AT.begin(115200);
  while (!SERIAL_AT);
  SERIAL_AT.println("READY");
#else
  USB.VID(0x2341);
  USB.PID(0x1002);
  USB.manufacturerName("Arduino");
  USB.productName("UNO WiFi R4 CMSIS-DAP");
  //USB.enableDFU();
  DAP.begin();
  SERIAL_USER.onEvent(usbEventCallback);
  SERIAL_USER.enableReboot(false);
  SERIAL_USER.begin(115200);
  SERIAL_USER.setRxBufferSize(0);
  SERIAL_USER.setRxBufferSize(2048);
  SERIAL_USER_INTERNAL.setRxBufferSize(8192);
  SERIAL_USER_INTERNAL.setTxBufferSize(8192);
  SERIAL_USER_INTERNAL.begin(115200, SERIAL_8N1, 44, 43);
  SERIAL_AT.setRxBufferSize(4096);
  SERIAL_AT.setTxBufferSize(4096);
  SERIAL_AT.begin(115200, SERIAL_8N1, 6, 5);
  USB.begin();
#endif

  xTaskCreatePinnedToCore(
      cdcLoop, /* Function to implement the task */
      "CDCBridge", /* Name of the task */
      2048,  /* Stack size in words */
      NULL,  /* Task input parameter */
      3,  /* Priority of the task */
      &cdcTask,  /* Task handle. */
      1); /* Core where the task should run */
}

/*
   arduino-builder -compile -fqbn=espressif:esp32:esp32s3:JTAGAdapter=default,PSRAM=disabled,FlashMode=qio,FlashSize=4M,LoopCore=1,EventsCore=1,USBMode=default,CDCOnBoot=cdc,MSCOnBoot=default,DFUOnBoot=default,UploadMode=default,PartitionScheme=huge_app,CPUFreq=240,UploadSpeed=921600,DebugLevel=none,EraseFlash=none -vid-pid=303A_1001 -ide-version=10820 CompostaUSBBridge.ino
*/

static uint8_t buf[2048];

/* -------------------------------------------------------------------------- */
void cdcLoop(void* param) { while(1) {
/* -------------------------------------------------------------------------- */

  if (SERIAL_USER.baudRate() != _baud) {
    _baud = SERIAL_USER.baudRate();
  }

  int i = 0;

  if (SERIAL_USER.available()) {
    i = min((unsigned int)SERIAL_USER.available(), sizeof(buf));
    SERIAL_USER.readBytes(buf, i);
  }

  if (i > 0) {
    SERIAL_USER_INTERNAL.write(buf, i);
  }

  i = 0;
  if (SERIAL_USER_INTERNAL.available()) {
    i = min((unsigned int)SERIAL_USER_INTERNAL.available(), sizeof(buf));
    SERIAL_USER_INTERNAL.readBytes(buf, i);
  }
  if (i > 0) {
    SERIAL_USER.write(buf, i);
  }

  delay(1);
  //yield();
}}

void usbEventCallback(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data) {
  if (event_base == ARDUINO_USB_CDC_EVENTS) {
    arduino_usb_cdc_event_data_t * data = (arduino_usb_cdc_event_data_t*)event_data;
    switch (event_id) {
      case ARDUINO_USB_CDC_LINE_CODING_EVENT:
        auto baud = data->line_coding.bit_rate;
        if (baud == 1200) {
          digitalWrite(GPIO_BOOT, HIGH);
          digitalWrite(GPIO_RST, LOW);
          delay(100);
          digitalWrite(GPIO_RST, HIGH);
          delay(100);
          digitalWrite(GPIO_RST, LOW);
          delay(100);
          digitalWrite(GPIO_RST, HIGH);
        } else if (baud == 2400) {
          digitalWrite(GPIO_BOOT, LOW);
          digitalWrite(GPIO_RST, HIGH);
          delay(100);
          digitalWrite(GPIO_RST, LOW);
          delay(100);
          digitalWrite(GPIO_RST, HIGH);
        } else {
          SERIAL_USER_INTERNAL.updateBaudRate(baud);
        }
        while (SERIAL_USER_INTERNAL.available()) {
          SERIAL_USER_INTERNAL.read();
        }
        break;
    }
  }
}

