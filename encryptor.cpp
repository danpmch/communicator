
#include "encryptor.h"

// TODO: figure out if better random numbers are needed
#include <ChaCha.h>
#include "WMath.h"
#include <cstring>

#define IV_SIZE 8

void new_iv(uint8_t* buf) {
  for(int i = 0; i < IV_SIZE; i++) {
    buf[i] = random(256);
  }
}

ChaCha20Encryptor::ChaCha20Encryptor(const uint8_t* key) {
  memcpy(this->key, key, KEY_SIZE);
}

// creates an encrypted packet by generating a new nonce, encrypting the message,
// and storing the nonce followed by the encrypted message in the output packet.
void ChaCha20Encryptor::encrypt(uint8_t* packet, const uint8_t* msg, uint8_t msg_len) {
  new_iv(packet);
  ChaCha cipher = ChaCha(20);
  cipher.setKey(this->key, KEY_SIZE);
  cipher.setIV(packet, IV_SIZE);
  cipher.encrypt(packet + IV_SIZE, msg, msg_len);
}

// decrypt the message from the packet and store in msg, assuming the first
// IV_SIZE bytes of the packet are the nonce
void ChaCha20Encryptor::decrypt(uint8_t* msg, const uint8_t* packet, uint8_t packet_len) {
  ChaCha cipher = ChaCha(20);
  cipher.setKey(this->key, KEY_SIZE);
  cipher.setIV(packet, IV_SIZE);
  cipher.decrypt(msg, packet + IV_SIZE, packet_len - IV_SIZE);
}

size_t ChaCha20Encryptor::header_size() { return IV_SIZE; }


const uint8_t TestChaCha20Encryptor::default_key[KEY_SIZE] = { 0 };
TestChaCha20Encryptor::TestChaCha20Encryptor() : ChaCha20Encryptor(default_key) {}

void PassthruEncryptor::encrypt(uint8_t* packet, const uint8_t* msg, uint8_t msg_len) {
  memcpy(packet, msg, msg_len);
}

void PassthruEncryptor::decrypt(uint8_t* msg, const uint8_t* packet, uint8_t packet_len) {
  memcpy(msg, packet, packet_len);
}

size_t PassthruEncryptor::header_size() { return 0; }
