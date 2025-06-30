#ifndef __KEYBOARD_H__
#define __KEYBOARD_H__

#include <Adafruit_TSC2007.h>

#define KEY_WIDTH 30
#define KEY_COLOR HX8357_WHITE
#define KEY_BORDER_COLOR HX8357_MAGENTA
#define KEY_TEXT_SIZE 3

namespace Keyboard {

typedef struct {
  char key;
  int col;
  int row;

  // Width of this key in key units. 1u is the width of
  // a regular letter.
  int u;
} Key;

struct Keyboard {
  const Key* keys;
  const size_t total_keys;
};

#define KEYS(n) (KEY_WIDTH * n)

extern const Keyboard regular;
extern const Keyboard numeric;

int x_coord(int key_col);
int y_coord(int key_row);
TS_Point key_location(const Key* k);

// Given a point in the keyboard's local reference frame, return
// a pointer to the key at that point, or NULL.
const Key* check_keypress(TS_Point* p, const Keyboard* board);


}
#endif
