#ifndef __GEOMETRY_H__
#define __GEOMETRY_H__

#include <Adafruit_TSC2007.h>

typedef struct {
  int x;
  int y;
} Translation_2D;

TS_Point translate(TS_Point* p, Translation_2D* t);
TS_Point inverse_translate(TS_Point* p, Translation_2D* t);

typedef struct {
  int x;
  int y;
  int w;
  int h;
} Box_2D;

bool box_intersect(TS_Point* p, Box_2D* box);

#endif