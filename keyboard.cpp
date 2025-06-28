
#include "keyboard.h"

const Key keys[] = {
  // row 0
  { .key = 'q', .col = 0, .row = 0, .u = 1 },
  { .key = 'w', .col = 1, .row = 0, .u = 1 },
  { .key = 'e', .col = 2, .row = 0, .u = 1 },
  { .key = 'r', .col = 3, .row = 0, .u = 1 },
  { .key = 't', .col = 4, .row = 0, .u = 1 },
  { .key = 'y', .col = 5, .row = 0, .u = 1 },
  { .key = 'u', .col = 6, .row = 0, .u = 1 },
  { .key = 'i', .col = 7, .row = 0, .u = 1 },
  { .key = 'o', .col = 8, .row = 0, .u = 1 },
  { .key = 'p', .col = 9, .row = 0, .u = 1 },

  // row 1
  { .key = 'a', .col = 0, .row = 1, .u = 1 },
  { .key = 's', .col = 1, .row = 1, .u = 1 },
  { .key = 'd', .col = 2, .row = 1, .u = 1 },
  { .key = 'f', .col = 3, .row = 1, .u = 1 },
  { .key = 'g', .col = 4, .row = 1, .u = 1 },
  { .key = 'h', .col = 5, .row = 1, .u = 1 },
  { .key = 'j', .col = 6, .row = 1, .u = 1 },
  { .key = 'k', .col = 7, .row = 1, .u = 1 },
  { .key = 'l', .col = 8, .row = 1, .u = 1 },
  { .key = '!', .col = 9, .row = 1, .u = 1 },

  // row 2
  { .key = 'z', .col = 0, .row = 2, .u = 1 },
  { .key = 'x', .col = 1, .row = 2, .u = 1 },
  { .key = 'c', .col = 2, .row = 2, .u = 1 },
  { .key = 'v', .col = 3, .row = 2, .u = 1 },
  { .key = 'b', .col = 4, .row = 2, .u = 1 },
  { .key = 'n', .col = 5, .row = 2, .u = 1 },
  { .key = 'm', .col = 6, .row = 2, .u = 1 },
  { .key = ',', .col = 7, .row = 2, .u = 1 },
  { .key = '.', .col = 8, .row = 2, .u = 1 },
  { .key = '?', .col = 9, .row = 2, .u = 1 },

  // row 3
  { .key = ' ', .col = 0, .row = 3, .u = 8 },
  { .key = '<', .col = 8, .row = 3, .u = 2 }
};

int x_coord(int key_col) {
  return KEYBOARD_X_START + KEY_WIDTH * key_col;
}

int y_coord(int key_row) {
  return KEYBOARD_BASE + KEYS(key_row);
}

