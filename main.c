#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>

#include "lib/usart/usart.h"
#include "lib/oled/oled.h"

#define FOSC 4951200  // Clock Speed
#define BAUD 9600
#define MYUBRR FOSC / 16 / BAUD - 1

typedef enum { LEFT, RIGHT, UP, DOWN, NEUTRAL } ADC_DIR;

typedef struct {
  long AIN0;
  long AIN1;
  long AIN2;
  long AIN3;
} ADC_T;

typedef struct {
  long x;
  long y;
  ADC_DIR direction;
} JOY_T;

ADC_T data;
JOY_T joystick;
ADC_T offset;

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

int main() {
  volatile char *adc = (char *)0x1400;
  USART_Initialize(MYUBRR);

  MCUCR |= (1 << SRE);
  SFIOR |= (1 << XMM2);

  // Initialization for the clock 2 MHz
  DDRD |= (1 << 4);
  TCCR3A |= (1 << COM3A0);
  TCCR3B |= (1 << WGM32) | (1 << CS30);
  OCR3AH = 0x00;
  OCR3AL = 0x00;

  //// Initialization for PE0 - INT0

  // // Disable global interrupts
  cli();
  // // Interrupt on rising edge PE0
  EMCUCR |= (1 << ISC2);
  // Enable interrupt on PE0
  GICR |= (1 << INT2);
  // Enable global interrupts
  sei();

  adc[0] = 0x00;

  // SRAM_test();

	//// OLED Init
  OLED_init();
  OLED_reset();
  OLED_goto_line(4);
  // OLED_goto_line(1);
  
  
  
  // OLED_write_data(0b00000000);
  // OLED_write_data(0b01000001);
  // OLED_write_data(0b01111111);
  // OLED_write_data(0b01111111);
  // OLED_write_data(0b01000001);
  // OLED_write_data(0b00000000);
  // OLED_write_data(0b00000000);
  // OLED_write_data(0b00000000);
  // OLED_print_char8('I');
  OLED_print("Ia Tsomaia", OLED_print_char8);
  OLED_set_brightness(64);
  OLED_goto_line(1);
  OLED_print("Juanpi", OLED_print_char4);
  

  // while (1) {
  //   switch (joystick.direction) {
  //     case UP:
  //       printf("DATA[X]: %ld \t DATA[Y]: %ld \t UP \r\n", joystick.x, joystick.y);
  //       break;
  //     case DOWN:
  //       printf("DATA[X]: %ld \t DATA[Y]: %ld \t DOWN \r\n", joystick.x, joystick.y);
  //       break;
  //     case RIGHT:
  //       printf("DATA[X]: %ld \t DATA[Y]: %ld \t RIGHT \r\n", joystick.x, joystick.y);
  //       break;
  //     case LEFT:
  //       printf("DATA[X]: %ld \t DATA[Y]: %ld \t LEFT \r\n", joystick.x, joystick.y);
  //       break;
  //     default: 
  //       printf("SLIDER1: %ld \t SLIDER2: %ld \t BUTTON 1: %d \t  BUTTON 2: %d    -     %d \n\r", data.AIN0, data.AIN1, (PINB & 0x01), ((PINB & (1 << 1)) >> 1));
  //   }
  // }

  return 0;
}

char current = 0;
int first = 0;

ISR(INT2_vect) {
  volatile unsigned char *adc = (unsigned char *)0x1400;   

  if (first == 0) {
    offset.AIN0 = (((long)(adc[0]) - 128) * 200 / 255);
    offset.AIN1 = (((long)(adc[0]) - 128) * 200 / 255);
    offset.AIN2 = (((long)(adc[0]) - 128) * 200 / 255);
    offset.AIN3 = (((long)(adc[0]) - 128) * 200 / 255);
    first = 1;

    adc[0] = 0x00;
    return;
  }

  data.AIN0 = (((long)(adc[0]) - 128) * 200 / 255);
  data.AIN1 = (((long)(adc[0]) - 128) * 200 / 255);
  data.AIN2 = (((long)(adc[0]) - 128) * 200 / 255) - offset.AIN2;
  data.AIN3 = (((long)(adc[0]) - 128) * 200 / 255) - offset.AIN3;

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

  adc[0] = 0x00;
}