#include "menu.h"

#include <avr/interrupt.h>
#include <avr/io.h>

#include "../joystick/joystick.h"
#include "../oled/oled.h"


menu_option_t* options[MENU_MAX_OPTIONS];
int current_option;

void MENU_init() {
  current_option = 0;
  for (int i = 0; i < MENU_MAX_OPTIONS; i++) {
    options[i] = 0;
  }

  // Initialization for the timer
  cli();
  TCCR0 |= (1 << WGM01) | (1 << CS02) | (1 << CS00);
  OCR0 = (uint8_t)(0xFF);
  TIMSK |= (1 << OCIE0);
  sei();
}

ISR(TIMER0_COMP_vect) {
  joy_t joystick = JOYSTICK_get_data(MAP);
  
  if (joystick.direction == DOWN && current_option > 0) {
    current_option--;
  } else if (joystick.direction == UP && current_option < MENU_MAX_OPTIONS) {
    current_option++;
  }

  SCREEN_reset();
  SCREEN_goto_line(2);
  SCREEN_print(options[current_option]->title, SCREEN_print_char8);
}

void MENU_add_option(menu_option_t *option) {
  int i = 0;
  while(options[i] == 0 && i < MENU_MAX_OPTIONS - 1) i++;
  options[i] = option;
}