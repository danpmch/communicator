#ifndef __LORA_H__
#define __LORA_H__

#include <cstdint>
#include "ui.h"

#define KEY_SIZE 32

void set_lora_cs_high();

// allowed transmit power values are 5-23 dBm
void setup_lora(int transmit_power, const uint8_t* encryption_key);

void send_chat(ChatMessage* msg);
bool receive_chat(ChatMessage* msg);

#endif
