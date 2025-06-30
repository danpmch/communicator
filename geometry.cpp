
#include "geometry.h"

TS_Point translate(TS_Point* p, Translation_2D* t) {
  TS_Point new_p(p->x + t->x, p->y + t->y, p->z);
  return new_p;
}

TS_Point inverse_translate(TS_Point* p, Translation_2D* t) {
  TS_Point new_p(p->x - t->x, p->y - t->y, p->z);
  return new_p;
}

bool box_intersect(TS_Point* p, Box_2D* box) {
  bool x_in_range = box->x <= p->x && p->x < box->x + box->w;
  bool y_in_range = box->y <= p->y && p->y < box->y + box->h;
  return x_in_range && y_in_range;
}