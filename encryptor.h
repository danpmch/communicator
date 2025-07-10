#ifndef __ENCRYPTOR_H__
#define __ENCRYPTOR_H__

#include <cstdint>
#include <cstddef>

class Encryptor {
  public:
    virtual void encrypt(uint8_t* packet, const uint8_t* msg, uint8_t msg_len);
    virtual void decrypt(uint8_t* msg, const uint8_t* packet, uint8_t packet_len);
    virtual size_t header_size();
};

#define KEY_SIZE 32

class ChaCha20Encryptor : public Encryptor {
  public:
    ChaCha20Encryptor(const uint8_t* key);
    void encrypt(uint8_t* packet, const uint8_t* msg, uint8_t msg_len);
    void decrypt(uint8_t* msg, const uint8_t* packet, uint8_t packet_len);
    size_t header_size();

  private:
    uint8_t key[KEY_SIZE];
};

class TestChaCha20Encryptor : public ChaCha20Encryptor {
  public:
    TestChaCha20Encryptor();

  private:
    static const uint8_t default_key[KEY_SIZE];

};

class PassthruEncryptor: public Encryptor {
  public:
    void encrypt(uint8_t* packet, const uint8_t* msg, uint8_t msg_len);
    void decrypt(uint8_t* msg, const uint8_t* packet, uint8_t packet_len);
    size_t header_size();
};

#endif