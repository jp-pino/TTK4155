#include "joystick.h"

#include <stdio.h>

#include <avr/io.h>

#include "../adc/adc.h"

volatile joy_t joystick;

const char* JOY_DIRECTION_STRINGS[] = { "LEFT", "RIGHT", "UP", "DOWN", "NEUTRAL" };

void JOYSTICK_init() {
  PORTB |= (1 << 2);
}

volatile joy_t JOYSTICK_get_data(offset_sel_t offset_correction) {
  
  adc_t data = ADC_get_data(offset_correction);

  joystick.button = ((PINB & (1 << 2)) >> 2); //PB2

  joystick.x = data.AIN2;
  joystick.y = data.AIN3;

  joystick.direction = NEUTRAL;
  if (joystick.x < 10 && joystick.x > -10) {
    if (joystick.y > 10) {
      joystick.direction = UP;
    } else if (joystick.y < -10) {
      joystick.direction = DOWN;
    }
  } else {
    if (joystick.x > 10) {
      joystick.direction = RIGHT;
    } else if (joystick.x < -10) {
      joystick.direction = LEFT;
    }
  }

  return joystick;
}