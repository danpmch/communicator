/***************************************************
  This is our library for the Adafruit HX8357D Featherwing
  ----> http://www.adafruit.com/products/2050

  Check out the links above for our tutorials and wiring diagrams
  These displays use SPI to communicate, 4 or 5 pins are required to
  interface (RST is optional)
  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.
  MIT license, all text above must be included in any redistribution
 ****************************************************/

#include "ui.h"
#include "lora.h"

void setup() {
  Serial.begin(115200);
  while (!Serial) delay(10);

  int8_t transmit_power = 5;
  setup_lora(transmit_power);
  setup_ui();
}

void loop(void) {
  ChatMessage msg = {"The Other", { 0 }};
  bool send = update_ui(&msg);

  if (send) {
    send_chat(&msg);
  }

  bool received = receive_chat(&msg);
  if (received) {
    record_chat(&msg);
    update_chat_history();
  }
}
