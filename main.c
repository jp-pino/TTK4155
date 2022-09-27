#include <stdio.h>
#include <stdlib.h>

#include <avr/io.h>

#include "lib/usart/usart.h"
#include "lib/oled/oled.h"
#include "lib/adc/adc.h"
#include "lib/joystick/joystick.h"
#include "lib/menu/menu.h"

#define FOSC 4951200  // Clock Speed
#define BAUD 9600
#define MYUBRR FOSC / 16 / BAUD - 1

void SRAM_test() {
  volatile char *ext_ram = (char *)0x1800;  // Start address for the SRAM
  uint16_t ext_ram_size = 0x800;
  uint16_t write_errors = 0;
  uint16_t retrieval_errors = 0;

  printf("Starting SRAM test...\n");
  // rand() stores some internal state, so calling this function in a loop will
  // yield different seeds each time (unless srand() is called before this
  // function)
  uint16_t seed = rand();
  // Write phase: Immediately check that the correct value was stored
  srand(seed);
  for (uint16_t i = 0; i < ext_ram_size; i++) {
    uint8_t some_value = rand();
    ext_ram[i] = some_value;
    uint8_t retreived_value = ext_ram[i];
    if (retreived_value != some_value) {
      printf("Write phase error: ext_ram[%4d] = %02X (should be %02X)\n", i,
             retreived_value, some_value);
      write_errors++;
    }
  }

  // Retrieval phase: Check that no values were changed during or after the
  // write phase
  srand(seed);
  // reset the PRNG to the state it had before the write phase
  for (uint16_t i = 0; i < ext_ram_size; i++) {
    uint8_t some_value = rand();
    uint8_t retreived_value = ext_ram[i];
    if (retreived_value != some_value) {
      printf("Retrieval phase error: ext_ram[%4d] = %02X (should be %02X)\n", i,
             retreived_value, some_value);
      retrieval_errors++;
    }
  }
  printf(
      "SRAM test completed with \n%4d errors in write phase and \n%4d errors "
      "in retrieval phase\n\n",
      write_errors, retrieval_errors);
}

void option1_fn() {
  printf("SELECTED OPTION 1!!! <<<<<<<<<<<<<<<<<<");
}

void option2_fn() {
  printf("SELECTED OPTION 2!!! <<<<<<<<<<<<<<<<<<");
}

menu_option_t option1 = {"OPTION 1", option1_fn};
menu_option_t option2 = {"OPTION 2", option2_fn};

int main() {
  USART_Initialize(MYUBRR);

  MCUCR |= (1 << SRE);
  SFIOR |= (1 << XMM2);

  ADC_init();

  // SRAM_test();

	//// OLED Init
  OLED_init();
  SCREEN_reset();
  // SCREEN_goto_line(4);
  // OLED_goto_line(1);
  
  // SCREEN_print("Ia Tsomaia", SCREEN_print_char8);
  // OLED_set_brightness(64);
  // SCREEN_goto_line(1);
  // SCREEN_print("Juanpi", SCREEN_print_char4);

  // SCREEN_line(10, 10, 60, 60);
// 
  // SCREEN_circle(50, 50, 10);

  // MENU_init();
  // MENU_add_option(&option1);
  // MENU_add_option(&option2);
  
  
  while (1) {
    joy_t joystick = JOYSTICK_get_data(OFFSET);
    adc_t adc = ADC_get_data(NO_CORRECTION);
    if (joystick.direction == NEUTRAL) {
      printf("SLIDER1: %ld \t SLIDER2: %ld \t BUTTON 1: %d \t  BUTTON 2: %d \n\r", adc.AIN0, adc.AIN1, (PINB & 0x01), ((PINB & (1 << 1)) >> 1));
    } else {
      printf("DATA[X]: %ld \t DATA[Y]: %ld \t %s \r\n", joystick.x, joystick.y, JOY_DIRECTION_STRINGS[joystick.direction]);
    }
  }

  return 0;
}
