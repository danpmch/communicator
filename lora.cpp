// Feather9x_TX
// -*- mode: C++ -*-
// Example sketch showing how to create a simple messaging client (transmitter)
// with the RH_RF95 class. RH_RF95 class does not provide for addressing or
// reliability, so you should only use RH_RF95 if you do not need the higher
// level messaging abilities.
// It is designed to work with the other example Feather9x_RX

#include "lora.h"
#include "participants.h"
#include <SPI.h>
#include <RH_RF95.h>

// TODO: figure out if better random numbers are needed
#include <ChaCha.h>

// First 3 here are boards w/radio BUILT-IN. Boards using FeatherWing follow.
#if defined (__AVR_ATmega32U4__)  // Feather 32u4 w/Radio
  #define RFM95_CS    8
  #define RFM95_INT   7
  #define RFM95_RST   4

#elif defined(ADAFRUIT_FEATHER_M0) || defined(ADAFRUIT_FEATHER_M0_EXPRESS) || defined(ARDUINO_SAMD_FEATHER_M0)  // Feather M0 w/Radio
  #define RFM95_CS    8
  #define RFM95_INT   3
  #define RFM95_RST   4

#elif defined(ARDUINO_ADAFRUIT_FEATHER_RP2040_RFM)  // Feather RP2040 w/Radio
  #define RFM95_CS   16
  #define RFM95_INT  21
  #define RFM95_RST  17

#elif defined (__AVR_ATmega328P__)  // Feather 328P w/wing
  #define RFM95_CS    4  //
  #define RFM95_INT   3  //
  #define RFM95_RST   2  // "A"

#elif defined(ESP8266)  // ESP8266 feather w/wing
  #define RFM95_CS    2  // "E"
  #define RFM95_INT  15  // "B"
  #define RFM95_RST  16  // "D"

#elif defined(ARDUINO_ADAFRUIT_FEATHER_ESP32S2) || defined(ARDUINO_NRF52840_FEATHER) || defined(ARDUINO_NRF52840_FEATHER_SENSE)
  #define RFM95_CS   10  // "B"
  #define RFM95_INT   9  // "A"
  #define RFM95_RST  11  // "C"

#elif defined(ESP32)  // ESP32 feather w/wing
  #define RFM95_CS   33  // "B"
  #define RFM95_INT  27  // "A"
  #define RFM95_RST  13

#elif defined(ARDUINO_NRF52832_FEATHER)  // nRF52832 feather w/wing
  #define RFM95_CS   11  // "B"
  #define RFM95_INT  31  // "C"
  #define RFM95_RST   7  // "A"

#endif

/* Some other possible setups include:

// Feather 32u4:
#define RFM95_CS   8
#define RFM95_RST  4
#define RFM95_INT  7

// Feather M0:
#define RFM95_CS   8
#define RFM95_RST  4
#define RFM95_INT  3

// Arduino shield:
#define RFM95_CS  10
#define RFM95_RST  9
#define RFM95_INT  7

// Feather 32u4 w/wing:
#define RFM95_RST 11  // "A"
#define RFM95_CS  10  // "B"
#define RFM95_INT  2  // "SDA" (only SDA/SCL/RX/TX have IRQ!)

// Feather m0 w/wing:
#define RFM95_RST 11  // "A"
#define RFM95_CS  10  // "B"
#define RFM95_INT  6  // "D"
*/

// Change to 434.0 or other frequency, must match RX's freq!
#define RF95_FREQ 915.0

// Singleton instance of the radio driver
RH_RF95 rf95(RFM95_CS, RFM95_INT);

#define KEY_SIZE 32
uint8_t key[KEY_SIZE] = {
  0xa1, 0xc4, 0x63, 0xe1, 0xfe, 0xff, 0x26, 0x2d,
  0x09, 0x13, 0xb0, 0x65, 0x30, 0x59, 0xb9, 0xae, 
  0x5a, 0x86, 0xda, 0xa1, 0xfe, 0xf4, 0x83, 0x3a, 
  0x20, 0x2f, 0xf0, 0x43, 0xb8, 0x6c, 0x29, 0x0c
};

#define IV_SIZE 8
#define AUTHOR_SIZE 1
#define PAYLOAD_SIZE (AUTHOR_SIZE + TEXT_BUFFER_SIZE)
#define MY_PACKET_SIZE (IV_SIZE + PAYLOAD_SIZE)

void new_iv(uint8_t* buf) {
  for(int i = 0; i < IV_SIZE; i++) {
    buf[i] = random(256);
  }
}

// creates an encrypted packet by generating a new nonce, encrypting the message,
// and storing the nonce followed by the encrypted message in the output packet.
void encrypt(uint8_t* packet, uint8_t* msg, uint8_t msg_len) {
  new_iv(packet);
  ChaCha cipher = ChaCha(20);
  cipher.setKey(key, KEY_SIZE);
  cipher.setIV(packet, IV_SIZE);
  cipher.encrypt(packet + IV_SIZE, msg, msg_len);
}

// decrypt the message from the packet and store in msg, assuming the first
// IV_SIZE bytes of the packet are the nonce
void decrypt(uint8_t* msg, uint8_t* packet, uint8_t packet_len) {
  ChaCha cipher = ChaCha(20);
  cipher.setKey(key, KEY_SIZE);
  cipher.setIV(packet, IV_SIZE);
  cipher.decrypt(msg, packet + IV_SIZE, packet_len - IV_SIZE);
}

// The default transmitter power is 13dBm, using PA_BOOST.
// If you are using RFM95/96/97/98 modules which uses the PA_BOOST transmitter pin, then
// you can set transmitter powers from 5 to 23 dBm:
void setup_lora(int transmit_power) {
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(RFM95_RST, OUTPUT);
  digitalWrite(RFM95_RST, HIGH);

  // analog read from unconnected analog input to get pretty random
  // number to seed PRNG with
  pinMode(PIN_A0, INPUT);
  randomSeed(analogRead(PIN_A0));

  delay(100);

  Serial.println("Feather LoRa TX Test!");

  // manual reset
  digitalWrite(RFM95_RST, LOW);
  delay(10);
  digitalWrite(RFM95_RST, HIGH);
  delay(10);

  while (!rf95.init()) {
    Serial.println("LoRa radio init failed");
    Serial.println("Uncomment '#define SERIAL_DEBUG' in RH_RF95.cpp for detailed debug info");
    while (1);
  }
  Serial.println("LoRa radio init OK!");

  // Defaults after init are 434.0MHz, modulation GFSK_Rb250Fd250, +13dbM
  if (!rf95.setFrequency(RF95_FREQ)) {
    Serial.println("setFrequency failed");
    while (1);
  }
  Serial.print("Set Freq to: "); Serial.println(RF95_FREQ);

  // Defaults after init are 434.0MHz, 13dBm, Bw = 125 kHz, Cr = 4/5, Sf = 128chips/symbol, CRC on

  Serial.printf("Set TX power to: %d\n", transmit_power);
  rf95.setTxPower(transmit_power, false);
}

void send(uint8_t* radiopacket, uint8_t packet_size) {
  //Serial.printf("Sending: '%s' ...", radiopacket);
  delay(10);
  digitalWrite(LED_BUILTIN, HIGH);
  rf95.send(radiopacket, packet_size);

  //Serial.println("Waiting for packet to complete...");
  delay(10);
  rf95.waitPacketSent();
  digitalWrite(LED_BUILTIN, LOW);
  //Serial.println("Done");
}

uint8_t receive(uint8_t* msg, uint8_t len) {
  //Serial.println("Waiting for reply...");
  if (rf95.waitAvailableTimeout(100)) {
    // Should be a reply message for us now
    if (rf95.recv(msg, &len)) {
      Serial.printf("Received msg (RSSI: %d): '%s'\n", rf95.lastRssi(), (char*) msg);
      return len;
    } else {
      Serial.println("Receive failed");
    }
  } else {
    //Serial.println("No reply, is there a listener around?");
  }

  return 0;
}

void send_chat(ChatMessage* msg) {
    size_t msg_len = strlen(msg->message);

    int author_id = get_participant_id(msg->author);
    uint8_t payload[PAYLOAD_SIZE] = { 0 };
    payload[0] = author_id;
    strcpy((char*) payload + AUTHOR_SIZE, msg->message);

    uint8_t encrypted[MY_PACKET_SIZE] = { 0 };
    encrypt(encrypted, payload, msg_len + AUTHOR_SIZE);
    Serial.printf("Sending: %s: '%s' '%s' ...", msg->author, msg->message, encrypted);
    send(encrypted, IV_SIZE + AUTHOR_SIZE + msg_len + 1);
    Serial.println("Done");
}

bool receive_chat(ChatMessage* msg) {
  uint8_t encrypted[RH_RF95_MAX_MESSAGE_LEN];
  uint8_t msg_len = receive(encrypted, sizeof(encrypted));

  if (msg_len == 0) {
    return false;
  }

  uint8_t decrypted[RH_RF95_MAX_MESSAGE_LEN] = { 0 };
  decrypt(decrypted, encrypted, msg_len - 1);

  uint8_t author_id = decrypted[0];
  const char* author = get_participant_name(author_id);

  if (author == NULL) {
    Serial.printf("Received message from unknown author_id %d, ignoring\n", author_id);
    return false;
  }

  char* message = (char*) decrypted + 1;

  Serial.printf("> %s: '%s'\n", author, (char*) message);
  strncpy(msg->author, author, sizeof(msg->author) - 1);
  strncpy(msg->message, message, sizeof(msg->message) - 1);
  return true;
}

void lora_loop() {
  delay(1000); // Wait 1 second between transmits, could also 'sleep' here!

  //Serial.println("Reading message from serial...");
  char radiopacket[PAYLOAD_SIZE] = { 0 };
  int bytesRead = Serial.readBytesUntil('\n', radiopacket, PAYLOAD_SIZE - 1);

  if (bytesRead > 0) {
    uint8_t encrypted[MY_PACKET_SIZE] = { 0 };
    encrypt(encrypted, (uint8_t*) radiopacket, bytesRead);

    Serial.printf("Sending: '%s' '%s' ...", radiopacket, encrypted);
    send(encrypted, bytesRead + IV_SIZE + 1);
    Serial.println("Done");
  }
  
    // Now wait for a reply
  uint8_t encrypted[RH_RF95_MAX_MESSAGE_LEN];
  uint8_t msg_len = receive(encrypted, sizeof(encrypted));
  if (msg_len > 0) {
    uint8_t decrypted[RH_RF95_MAX_MESSAGE_LEN] = { 0 };
    decrypt(decrypted, encrypted, msg_len - 1);
    Serial.printf("> '%s'\n", (char*) decrypted);
  }

}
