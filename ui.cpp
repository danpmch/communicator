
#include "ui.h"
#include "keyboard.h"
#include "geometry.h"

#include <Adafruit_GFX.h> // Core graphics library
#include <Adafruit_HX8357.h>
#include <Adafruit_TSC2007.h>

#define TFT_RST -1
#ifdef ESP8266
#define STMPE_CS 16
#define TFT_CS 0
#define TFT_DC 15
#define SD_CS 2
#elif defined(ARDUINO_ADAFRUIT_FEATHER_ESP32C6)
#define STMPE_CS 6
#define TFT_CS 7
#define TFT_DC 8
#define SD_CS 5
#elif defined(ESP32) && !defined(ARDUINO_ADAFRUIT_FEATHER_ESP32S2) && !defined(ARDUINO_ADAFRUIT_FEATHER_ESP32S3)
#define STMPE_CS 32
#define TFT_CS 15
#define TFT_DC 33
#define SD_CS 14
#elif defined(TEENSYDUINO)
#define TFT_DC 10
#define TFT_CS 4
#define STMPE_CS 3
#define SD_CS 8
#elif defined(ARDUINO_STM32_FEATHER)
#define TFT_DC PB4
#define TFT_CS PA15
#define STMPE_CS PC7
#define SD_CS PC5
#elif defined(ARDUINO_NRF52832_FEATHER) /* BSP 0.6.5 and higher! */
#define TFT_DC 11
#define TFT_CS 31
#define STMPE_CS 30
#define SD_CS 27
#elif defined(ARDUINO_MAX32620FTHR) || defined(ARDUINO_MAX32630FTHR)
#define TFT_DC P5_4
#define TFT_CS P5_3
#define STMPE_CS P3_3
#define SD_CS P3_2
#else
// Anything else, defaults!
#define STMPE_CS 6
#define TFT_CS 9
#define TFT_DC 10
#define SD_CS 5
#endif

// Init screen on hardware SPI, HX8357D type:
Adafruit_HX8357 tft = Adafruit_HX8357(TFT_CS, TFT_DC, TFT_RST);

#if defined(_ADAFRUIT_STMPE610H_)
Adafruit_STMPE610 ts = Adafruit_STMPE610(STMPE_CS);
#elif defined(_ADAFRUIT_TSC2007_H)
// If you're using the TSC2007 there is no CS pin needed, so instead its an IRQ!
#define TSC_IRQ STMPE_CS
Adafruit_TSC2007 ts = Adafruit_TSC2007();  // newer rev 2 touch contoller
#else
#error("You must have either STMPE or TSC2007 headers included!")
#endif

// This is calibration data for the raw touch data to the screen coordinates
// For STMPE811/STMPE610
#define STMPE_TS_MINX 3800
#define STMPE_TS_MAXX 100
#define STMPE_TS_MINY 100
#define STMPE_TS_MAXY 3750

// For TSC2007
#define TSC_TS_MINX 300
#define TSC_TS_MAXX 3800
#define TSC_TS_MINY 185
#define TSC_TS_MAXY 3700
#define TSC_IRQ STMPE_CS

// Size of the color selection boxes and the paintbrush size
#define PENRADIUS 3

#define TEXT_COLOR HX8357_RED

typedef struct {
  Box_2D obj;
  uint16_t background_color;
  Translation_2D location;
} UI_Element;

typedef struct {
  const Keyboard::Keyboard* board;
  Translation_2D location;
} UI_Keyboard;

UI_Element send_button;
UI_Element text_area;
UI_Element history_area;

UI_Keyboard regular = {
  .board = &Keyboard::regular,
  .location = { .x = 10, .y = KEY_WIDTH * 12 }
};

UI_Keyboard numeric = {
  .board = &Keyboard::numeric,
  .location = { .x = 10, .y = KEY_WIDTH * 12 }
};

UI_Keyboard* current_keyboard = &regular;

// At textSize(2) the screen fits 26 characters per line
uint16_t text_2_width;
uint16_t text_2_height;

ChatMessage current_msg = { "Me", { 0 } };
int text_cursor = 0;

void clear_area(Adafruit_HX8357* tft, UI_Element* area) {
  tft->fillRect(area->location.x, area->location.y, area->obj.w, area->obj.h, area->background_color);
}

void draw_send(Adafruit_HX8357* tft, UI_Element* send) {
  tft->fillRect(send->location.x, send->location.y, send->obj.w, send->obj.h, send->background_color);
  tft->drawRect(send->location.x, send->location.y, send->obj.w, send->obj.h, KEY_BORDER_COLOR);
  tft->setTextSize(KEY_TEXT_SIZE);
  tft->setCursor(send->location.x + send->obj.w / 3, send->location.y + send->obj.h / 8);
  tft->print("SEND");
}

void draw_text_area_cursor(Adafruit_HX8357* tft, int cursor, uint color) {
  int row = cursor / LINE_WIDTH;
  int col = cursor - row * LINE_WIDTH;
  tft->drawFastVLine(
    text_2_width * col,
    text_area.location.y + text_2_height * row,
    text_2_height,
    color
  );
}

// paints over the given index with the background color
void delete_text_buffer_index(Adafruit_HX8357* tft, int index) {
  int16_t dirty_row = index / LINE_WIDTH;
  int16_t dirty_col = index - dirty_row * LINE_WIDTH;
  Serial.printf("Computed bounds for %d (%d, %d): (%d, %d, %d, %d)\n", index, dirty_col, dirty_row, dirty_col * text_2_width, dirty_row * text_2_height, text_2_width, text_2_height);
  tft->fillRect(
    dirty_col * text_2_width,
    text_area.location.y + dirty_row * text_2_height,
    text_2_width,
    text_2_height,
    text_area.background_color
  );
}

void draw_keyboard(Adafruit_HX8357* tft, UI_Keyboard* board) {
  Serial.printf("Drawing %d keys:\n", board->board->total_keys);
  tft->setTextColor(TEXT_COLOR);
  tft->setTextSize(KEY_TEXT_SIZE);
  for (int i = 0; i < board->board->total_keys; i++) {
    Keyboard::Key k = board->board->keys[i];
    Serial.printf("-> %c (%d, %d)\n", k.key, k.col, k.row);
    TS_Point local_key_p = key_location(&k);
    TS_Point key_p = translate(&local_key_p, &board->location);
    tft->fillRect(key_p.x, key_p.y, KEY_WIDTH * k.u, KEY_WIDTH, KEY_COLOR);
    tft->drawRect(key_p.x, key_p.y, KEY_WIDTH * k.u, KEY_WIDTH, KEY_BORDER_COLOR);
    tft->setCursor(key_p.x + (KEY_WIDTH * k.u / 4), key_p.y + (KEY_WIDTH / 8));
    tft->printf("%c", k.key);
  }
  Serial.println("Key drawing complete!");
}

#define CHAT_HISTORY_SIZE 6
ChatMessage history[CHAT_HISTORY_SIZE] = { 0 };
uint used_history_entries = 0;
uint next_history = 0;

void copy_chat(ChatMessage* target, const ChatMessage* source) {
  memcpy(target, source, sizeof(ChatMessage));
}

void record_chat(ChatMessage* msg) {
  copy_chat(history + next_history, msg);
  next_history = (next_history + 1) % CHAT_HISTORY_SIZE;
  used_history_entries = min(CHAT_HISTORY_SIZE, used_history_entries + 1);
}

// we will assign the calibration values on init
int16_t min_x, max_x, min_y, max_y;

void setup_ui(const char* this_participant) {
  Serial.println("Initializing UI");

  strncpy(current_msg.author, this_participant, LINE_WIDTH - 1);

#if defined(_ADAFRUIT_STMPE610H_)
  if (!ts.begin()) {
    Serial.println("Couldn't start STMPE touchscreen controller");
    while (1) delay(100);
  }
  min_x = STMPE_TS_MINX;
  max_x = STMPE_TS_MAXX;
  min_y = STMPE_TS_MINY;
  max_y = STMPE_TS_MAXY;
#else
  if (!ts.begin(0x48, &Wire)) {
    Serial.println("Couldn't start TSC2007 touchscreen controller");
    while (1) delay(100);
  }
  min_x = TSC_TS_MINX;
  max_x = TSC_TS_MAXX;
  min_y = TSC_TS_MINY;
  max_y = TSC_TS_MAXY;
  pinMode(TSC_IRQ, INPUT);
#endif

  Serial.println("Touchscreen started");

  tft.begin();
  tft.fillScreen(HX8357_BLACK);

  // find single character bounding box
  int16_t x;
  int16_t y;
  tft.setTextSize(2);
  tft.getTextBounds("r", 0, 0, &x, &y, &text_2_width, &text_2_height);
  Serial.printf("Found character bounding box: (%d, %d)\n", text_2_width, text_2_height);

  // landscape with USB port at bottom
  tft.setRotation(2);

  text_area = {
    .obj = {.x = 0, .y = 0, .w = tft.width(), .h = TEXT_BUFFER_LINES * text_2_height },
    .background_color = HX8357_BLUE,
    .location = {.x = 0, .y = current_keyboard->location.y - TEXT_BUFFER_LINES * text_2_height }
  };

  send_button = {
    .obj = {.x = 0, .y = 0, .w = tft.width(), .h = KEY_WIDTH},
    .background_color = KEY_COLOR,
    .location = {.x = 0, .y = text_area.location.y - KEY_WIDTH}
  };

  history_area = {
    .obj = {.x = 0, .y = 0, .w = tft.width(), .h = send_button.location.y},
    .background_color = HX8357_GREEN,
    .location = {.x = 0, .y = 0}
  };

  clear_area(&tft, &text_area);
  draw_keyboard(&tft, current_keyboard);
  draw_send(&tft, &send_button);
  clear_area(&tft, &history_area);
}

#define DEBOUNCE_DELAY_MS 200
unsigned long last_keypress_ms = 0;

void update_chat_history() {
    clear_area(&tft, &history_area);
    int display_entry = CHAT_HISTORY_SIZE - 1;
    for (int msg = 0; msg < used_history_entries; msg++) {
      int msg_offset = next_history - 1 - msg;
      int msg_index = 0 <= msg_offset ? msg_offset : CHAT_HISTORY_SIZE + msg_offset;
      Serial.printf("Printing msg %d, index %d\n", msg, msg_index);

      tft.setCursor(0, text_2_height * display_entry * 3);
      tft.println(history[msg_index].author);
      tft.println(history[msg_index].message);
      display_entry--;
    }
}

bool update_ui(ChatMessage* out) {
#if defined(TSC_IRQ)
  if (digitalRead(TSC_IRQ)) {
    // IRQ pin is high, nothing to read!
    return false;
  }
#endif

  TS_Point p = ts.getPoint();

  Serial.print("X = ");
  Serial.print(p.x);
  Serial.print("\tY = ");
  Serial.print(p.y);
  Serial.print("\tPressure = ");
  Serial.print(p.z);
  if (((p.x == 0) && (p.y == 0)) || (p.z < 10)) {
    Serial.printf("Rejected point (%d, %d, %d)\n", p.x, p.y, p.z);
    return false;  // no pressure, no touch
  }

  /* When the user presses the screen, it generates a series of presses
   * at varying pressure levels. Ideally we need to reduce these to one
   * discrete screen press for each logical press.
   */
  unsigned long current_time = millis();
  if (current_time < last_keypress_ms + DEBOUNCE_DELAY_MS) {
    // reject this keypress as a duplicate
    Serial.println("Rejecting duplicate keypress");
    return false;
  } else {
    // accept this keypress, record keypress time
    last_keypress_ms = current_time;
  }

  // Scale from ~0->4000 to tft.width using the calibration #'s
  // These values get confusing because .height() and .width() account for the
  // screen rotation but .getPoint() does not.
  int scaled_x = map(p.x, max_x, min_x, 0, tft.width());
  int scaled_y = map(p.y, max_y, min_y, 0, tft.height());
  p.x = scaled_x;
  p.y = scaled_y;
  Serial.print(" -> ");
  Serial.print(p.x);
  Serial.print(", ");
  Serial.println(p.y);

  TS_Point keyboard_point = inverse_translate(&p, &current_keyboard->location);
  const Keyboard::Key* k = Keyboard::check_keypress(&keyboard_point, current_keyboard->board);

  int dirty = -1;
  if (k != NULL) {
    Serial.printf("Pushed key %c\n", k->key);
    if (k->key == '<') {
      // backspace
      text_cursor = max(0, text_cursor - 1);
      dirty = text_cursor;
    } else if (k->key == '#' && current_keyboard == &regular) {
      current_keyboard = &numeric;
      draw_keyboard(&tft, current_keyboard);
    } else if (k->key == 'A' && current_keyboard == &numeric) {
      current_keyboard = &regular;
      draw_keyboard(&tft, current_keyboard);
    } else {
      current_msg.message[text_cursor] = k->key;
      text_cursor = min(TEXT_BUFFER_SIZE - 1, text_cursor + 1);
    }
  }

  TS_Point translated_p = inverse_translate(&p, &send_button.location);
  bool send_pushed = box_intersect(&translated_p, &send_button.obj);
  bool should_send_msg = send_pushed && text_cursor > 0;
  if (should_send_msg) {
    current_msg.message[text_cursor] = 0;
    record_chat(&current_msg);
    copy_chat(out, &current_msg);
    text_cursor = 0;
  }

  if (((p.y - PENRADIUS) > 0) && ((p.y + PENRADIUS) < tft.height())) {
    tft.fillCircle(p.x, p.y, PENRADIUS, HX8357_BLUE);
  }

  current_msg.message[text_cursor] = 0;
  if (dirty != -1) {
    delete_text_buffer_index(&tft, dirty);
    draw_text_area_cursor(&tft, dirty + 1, text_area.background_color);
  }
  if (should_send_msg) {
    clear_area(&tft, &text_area);
  }
  tft.setTextSize(2);
  tft.setCursor(0, text_area.location.y);
  tft.print(current_msg.message);

  // clear old cursor and draw new one
  draw_text_area_cursor(&tft, max(0, text_cursor - 1), text_area.background_color);
  draw_text_area_cursor(&tft, text_cursor, TEXT_COLOR);

  if (should_send_msg) {
    update_chat_history();
  }

  return should_send_msg;
}
