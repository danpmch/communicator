#ifndef __LORA_H__
#define __LORA_H__

#include "ui.h"

// allowed transmit power values are 5-23 dBm
void setup_lora(int transmit_power);

void send_chat(ChatMessage* msg);
bool receive_chat(ChatMessage* msg);

#endif
