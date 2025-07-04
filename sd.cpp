
#include "SdFat.h"

#define SD_FAT_TYPE 1

#ifdef ESP8266
   #define STMPE_CS 16
   #define TFT_CS   0
   #define TFT_DC   15
   #define SD_CS    2
#elif defined(ARDUINO_ADAFRUIT_FEATHER_RP2040)
   #define STMPE_CS 8
   #define TFT_CS   9
   #define TFT_DC   10
   #define SD_CS    7
#elif defined(ESP32) && !defined(ARDUINO_ADAFRUIT_FEATHER_ESP32S2) && !defined(ARDUINO_ADAFRUIT_FEATHER_ESP32S3)
   #define STMPE_CS 32
   #define TFT_CS   15
   #define TFT_DC   33
   #define SD_CS    14
#elif defined(TEENSYDUINO)
   #define TFT_DC   10
   #define TFT_CS   4
   #define STMPE_CS 3
   #define SD_CS    8
#elif defined(ARDUINO_STM32_FEATHER)
   #define TFT_DC   PB4
   #define TFT_CS   PA15
   #define STMPE_CS PC7
   #define SD_CS    PC5
#elif defined(ARDUINO_NRF52832_FEATHER)  /* BSP 0.6.5 and higher! */
   #define TFT_DC   11
   #define TFT_CS   31
   #define STMPE_CS 30
   #define SD_CS    27
#elif defined(ARDUINO_MAX32620FTHR) || defined(ARDUINO_MAX32630FTHR)
   #define TFT_DC   P5_4
   #define TFT_CS   P5_3
   #define STMPE_CS P3_3
   #define SD_CS    P3_2
#else
    // Anything else, defaults!
   #define STMPE_CS 6
   #define TFT_CS   9
   #define TFT_DC   10
   #define SD_CS    5
#endif

#define SPI_CLOCK SD_SCK_MHZ(8)

// Try to select the best SD card configuration.
#if defined(HAS_TEENSY_SDIO)
#define SD_CONFIG SdioConfig(FIFO_SDIO)
#elif defined(RP_CLK_GPIO) && defined(RP_CMD_GPIO) && defined(RP_DAT0_GPIO)
// See the Rp2040SdioSetup example for RP2040/RP2350 boards.
#define SD_CONFIG SdioConfig(RP_CLK_GPIO, RP_CMD_GPIO, RP_DAT0_GPIO)
#elif ENABLE_DEDICATED_SPI
#define SD_CONFIG SdSpiConfig(SD_CS, DEDICATED_SPI, SPI_CLOCK)
#else  // HAS_TEENSY_SDIO
#define SD_CONFIG SdSpiConfig(SD_CS, SHARED_SPI, SPI_CLOCK)
#endif  // HAS_TEENSY_SDIO

SdFat32 sd;
File32 file;

void setup_sd() {
  Serial.print("Initializing SD card...");
  // SD card is pretty straightforward, a single call...
    // Initialize the SD.
  if (!sd.begin(SD_CONFIG)) {
    sd.initErrorHalt(&Serial);
    return;
  }
  /*
  if(!sd.begin(SD_CS, SD_SCK_MHZ(8))) { // ESP32 requires 25 MHz limit
    Serial.println("SD begin() failed");
    while(1) delay(10);
  }
  */
  Serial.println("OK!");
}

int read_bytes(const char * path, uint8_t* buffer, uint max_bytes) {
  if (!file.open(path, FILE_READ)) {
    Serial.printf("Error opening file: %s", path);
    return 0;
  }
  return file.read(buffer, max_bytes);
}