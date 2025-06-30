
#include "ui.h"
#include <string.h>

const char* participants[] = {
  "The Peddler",
  "The Other"
};

const unsigned short TOTAL_PARTICIPANTS = sizeof(participants) / sizeof(char*);

const char* get_participant_name(unsigned short id) {
  if (id > TOTAL_PARTICIPANTS) {
    return NULL;
  }

  return participants[id];
}

int get_participant_id(const char* name) {
  for (int i = 0; i < TOTAL_PARTICIPANTS; i++) {
    if (strcmp(name, participants[i]) == 0) {
      return i;
    }
  }

  return -1;
}