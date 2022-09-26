#include "joystick.h"

#include "../adc/adc.h"

volatile joy_t joystick;

volatile joy_t JOYSTICK_get_data(offset_sel_t offset_correction) {
  static volatile joy_t result;
  adc_t data = ADC_get_data(offset_correction);

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

  return result;
}