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
#include "participants.h"
#include "sd.h"

#define ENABLE_UI
#ifdef ENABLE_UI

#define THIS_PARTICIPANT_ID 0
//#define ENCRYPTION

void setup() {
  Serial.begin(115200);
  //while (!Serial) delay(10);

  // Need to disable the lora chip before accessing SD because they
  // interfere with each other.
  set_lora_cs_high();
  setup_sd();

#ifdef ENCRYPTION
  uint8_t encryption_key[KEY_SIZE] = { 0 };
  if (read_bytes("RandomNumbers", encryption_key, KEY_SIZE) <= 0) {
    Serial.println("Could not read secret key, cannot send messages!");
  } else {
    Serial.print("Key: ");
    for (int i = 0; i < KEY_SIZE; i++) {
      Serial.printf("%02x ", encryption_key[i]);
    }
    Serial.println("");
  }

  ChaCha20Encryptor* encryptor = new ChaCha20Encryptor(encryption_key);
#else
  //PassthruEncryptor* encryptor = new PassthruEncryptor();
  ChaCha20Encryptor* encryptor = new TestChaCha20Encryptor();
#endif

  int8_t transmit_power = 5;
  setup_lora(transmit_power, encryptor);
  setup_ui(get_participant_name(THIS_PARTICIPANT_ID));
}

void loop(void) {
  ChatMessage msg = {"Unknown", { 0 }};

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

#else

#include <cstring>
#define THIS_PARTICIPANT_ID 1

const char* ME = get_participant_name(THIS_PARTICIPANT_ID);

void setup() {
  Serial.begin(115200);
  //while (!Serial) delay(10);

  int8_t transmit_power = 5;
  setup_lora(transmit_power, new TestChaCha20Encryptor());
}

void loop(void) {
  ChatMessage msg = {"Unknown", { 0 }};
  strncpy(msg.author, ME, sizeof(msg.author) - 1);

  int bytesRead = Serial.readBytesUntil('\n', msg.message, sizeof(msg.message) - 1);
  if (bytesRead > 0) {
    send_chat(&msg);
  }

  receive_chat(&msg);
}
#endif
