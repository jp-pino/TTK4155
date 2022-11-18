#include "menu.h"

#include <avr/interrupt.h>
#include <avr/io.h>

#include "../joystick/joystick.h"
#include "../oled/oled.h"
#include "../mcp2515/mcp2515.h"

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
  OCR0 = (uint8_t)(0x80);
  TIMSK |= (1 << OCIE0);
  sei();
}

volatile int counter = 0;

ISR(TIMER0_COMP_vect) {
  static volatile joy_dir_t last_direction = NEUTRAL;
  static volatile joy_dir_t last_pressed = NEUTRAL;
  joy_t joystick = JOYSTICK_get_data(OFFSET);
  adc_t adc = ADC_get_data(NO_CORRECTION);


  // printf("Data: %d <-> %d <-> %d\n", (int8_t)adc.AIN1, adc.AIN1, (int32_t)((int8_t)adc.AIN1));

  MCP2515_write((message_t){0x05, { (int8_t)joystick.x, (int8_t)adc.AIN0, (int8_t)adc.AIN1, ((PINB & (1 << 1)) >> 1) & 0xFF}, 4, DATA_FRAME});
  MCP2515_rts();


  SCREEN_line(0, 30, 128, 30, ZERO);
  SCREEN_line(0, 35, 128, 35, ZERO);
  SCREEN_line(0, 30, adc.AIN0/4, 30, ONE);
  SCREEN_line(0, 35, adc.AIN1/4, 35, ONE);
  
  

  if (joystick.direction != last_direction) {
    // printf("CURRENT: %d - %s!!\n", current_option, JOY_DIRECTION_STRINGS[joystick.direction]);
    if (joystick.direction == DOWN && current_option > 0 && options[current_option - 1] != 0) {
      current_option--;
    } else if (joystick.direction == UP && current_option < MENU_MAX_OPTIONS && options[current_option + 1] != 0) {
      current_option++;
    }

    if (joystick.direction == DOWN || joystick.direction == UP) {
      // SCREEN_reset();
      SCREEN_goto_line(2);
      SCREEN_print(options[current_option]->title, SCREEN_print_char8);
    }
  }

  if (joystick.button == 0 && joystick.button != last_pressed) {
    options[current_option]->f();
  }

  last_pressed = joystick.button;
  last_direction = joystick.direction;
}

void MENU_add_option(menu_option_t* option) {
  int i = 0;
  while (options[i] != 0 && i < MENU_MAX_OPTIONS - 1) i++;
  options[i] = option;
}