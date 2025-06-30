
#include "keyboard.h"
#include "geometry.h"

const Keyboard::Key regular_keys[] = {
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
  { .key = '#', .col = 0, .row = 3, .u = 1 },
  { .key = ' ', .col = 1, .row = 3, .u = 7 },
  { .key = '<', .col = 8, .row = 3, .u = 2 }
};
const size_t TOTAL_REGULAR_KEYS = sizeof(regular_keys) / sizeof(Keyboard::Key);

const Keyboard::Keyboard Keyboard::regular = {
  .keys = regular_keys,
  .total_keys = TOTAL_REGULAR_KEYS
};

const Keyboard::Key numeric_keys[] = {
  // row 0
  { .key = '!', .col = 0, .row = 0, .u = 1 },
  { .key = '@', .col = 1, .row = 0, .u = 1 },
  { .key = '#', .col = 2, .row = 0, .u = 1 },
  { .key = '$', .col = 3, .row = 0, .u = 1 },
  { .key = '%', .col = 4, .row = 0, .u = 1 },
  { .key = '^', .col = 5, .row = 0, .u = 1 },
  { .key = '&', .col = 6, .row = 0, .u = 1 },
  { .key = '*', .col = 7, .row = 0, .u = 1 },
  { .key = '(', .col = 8, .row = 0, .u = 1 },
  { .key = ')', .col = 9, .row = 0, .u = 1 },

  // row 1
  { .key = '1', .col = 0, .row = 1, .u = 1 },
  { .key = '2', .col = 1, .row = 1, .u = 1 },
  { .key = '3', .col = 2, .row = 1, .u = 1 },
  { .key = '4', .col = 3, .row = 1, .u = 1 },
  { .key = '5', .col = 4, .row = 1, .u = 1 },
  { .key = '6', .col = 5, .row = 1, .u = 1 },
  { .key = '7', .col = 6, .row = 1, .u = 1 },
  { .key = '8', .col = 7, .row = 1, .u = 1 },
  { .key = '9', .col = 8, .row = 1, .u = 1 },
  { .key = '0', .col = 9, .row = 1, .u = 1 },

  // row 2
  { .key = '~', .col = 0, .row = 2, .u = 1 },
  { .key = '`', .col = 1, .row = 2, .u = 1 },
  { .key = '_', .col = 2, .row = 2, .u = 1 },
  { .key = '-', .col = 3, .row = 2, .u = 1 },
  { .key = '+', .col = 4, .row = 2, .u = 1 },
  { .key = '=', .col = 5, .row = 2, .u = 1 },
  { .key = '{', .col = 6, .row = 2, .u = 1 },
  { .key = '}', .col = 7, .row = 2, .u = 1 },
  { .key = '[', .col = 8, .row = 2, .u = 1 },
  { .key = ']', .col = 9, .row = 2, .u = 1 },

  // row 3
  { .key = 'A', .col = 0, .row = 3, .u = 1 },
  { .key = ' ', .col = 1, .row = 3, .u = 7 },
  { .key = '<', .col = 8, .row = 3, .u = 2 }
};
const size_t TOTAL_NUMERIC_KEYS = sizeof(numeric_keys) / sizeof(Keyboard::Key);

const Keyboard::Keyboard Keyboard::numeric = {
  .keys = numeric_keys,
  .total_keys = TOTAL_NUMERIC_KEYS
};

int Keyboard::x_coord(int key_col) {
  return KEY_WIDTH * key_col;
}

int Keyboard::y_coord(int key_row) {
  return KEYS(key_row);
}

TS_Point Keyboard::key_location(const Key* k) {
  return TS_Point(x_coord(k->col), y_coord(k->row), 0);
}

const Keyboard::Key* Keyboard::check_keypress(TS_Point* p, const Keyboard* board) {
  for (int i = 0; i < board->total_keys; i++) {
    const Key* k = board->keys + i;
    TS_Point key_p = key_location(k);
    Box_2D key_box = {
      .x = key_p.x,
      .y = key_p.y,
      .w = KEY_WIDTH * k->u,
      .h = KEY_WIDTH
    };
    if (box_intersect(p, &key_box)) {
      return k;
    }
  }

  return NULL;
}


