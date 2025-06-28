#ifndef __KEYBOARD_H__
#define __KEYBOARD_H__

#define KEY_WIDTH 30
#define KEY_COLOR HX8357_WHITE
#define KEY_BORDER_COLOR HX8357_MAGENTA
#define KEY_TEXT_SIZE 3

typedef struct {
  char key;
  int col;
  int row;

  // Width of this key in key units. 1u is the width of
  // a regular letter.
  int u;
} Key;

#define TOTAL_KEYS 32

#define KEYBOARD_BASE (KEY_WIDTH * 12)
#define KEYBOARD_X_START 10
#define KEYS(n) (KEY_WIDTH * n)

extern const Key keys[];

int x_coord(int key_col);
int y_coord(int key_row);

#endif
