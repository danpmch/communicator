#ifndef __UI_H__
#define __UI_H__

#define LINE_WIDTH 26
#define TEXT_BUFFER_LINES 2
#define TEXT_BUFFER_SIZE (LINE_WIDTH * TEXT_BUFFER_LINES + 1)

typedef struct {
  char author[LINE_WIDTH];
  char message[TEXT_BUFFER_SIZE];
} ChatMessage;
void copy_chat(ChatMessage* target, const ChatMessage* source);
void record_chat(ChatMessage* msg);

void setup_ui(const char* this_participant);

void update_chat_history();
bool update_ui(ChatMessage* out);

#endif
