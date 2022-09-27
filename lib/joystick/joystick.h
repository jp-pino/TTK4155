#ifndef __LIB_JOYSTICK_JOYSTICK_H__
#define __LIB_JOYSTICK_JOYSTICK_H__

#include "../adc/adc.h"

typedef enum { LEFT, RIGHT, UP, DOWN, NEUTRAL } joy_dir_t;
const char* JOY_DIRECTION_STRINGS[NEUTRAL + 1];

typedef struct {
  long x;
  long y;
  joy_dir_t direction;
} joy_t;

volatile joy_t JOYSTICK_get_data(offset_sel_t offset_correction);

#endif//__LIB_JOYSTICK_JOYSTICK_H__