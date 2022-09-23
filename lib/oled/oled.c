/*
 * oled_init.c
 *
 * Created: 9/23/2022 12:15:17 PM
 *  Author: itsom
 */
#include "oled.h"

#include <avr/pgmspace.h>

#include "fonts.h"

void OLED_init() {
  OLED_write_command(0xae);  // display off
  OLED_write_command(0xa1);  // segment remap
  OLED_write_command(0xda);  // common pads hardware: alternative
  OLED_write_command(0x12);
  OLED_write_command(0xc8);  // common output scan direction:com63~com0
  OLED_write_command(0xa8);  // multiplex ration mode:63
  OLED_write_command(0x3f);
  OLED_write_command(0xd5);  // display divide ratio/osc. freq. mode
  OLED_write_command(0x80);
  OLED_write_command(0x81);  // contrast control
  OLED_write_command(0x50);
  OLED_write_command(0xd9);  // set pre-charge period
  OLED_write_command(0x21);
  OLED_write_command(0x20);  // Set Memory Addressing Mode
  OLED_write_command(0x02);
  OLED_write_command(0xdb);  // VCOM deselect level mode
  OLED_write_command(0x30);
  OLED_write_command(0xad);  // master configuration
  OLED_write_command(0x00);
  OLED_write_command(0xa4);  // out follows RAM content
  OLED_write_command(0xa6);  // set normal display
  OLED_write_command(0xaf);  // display on
}

void OLED_write_command(uint8_t comm) {
  volatile char *ptr = (char *) 0x1000;
  *ptr = comm;
}

void OLED_goto_line(uint8_t line) {
	OLED_write_command(0xB0 | line);
	OLED_write_command(0x00);
	OLED_write_command(0x10);
}

void OLED_clear_line(uint8_t line) {
	OLED_goto_line(line);
	for (uint8_t j = 0; j < 128; j++) {
      OLED_write_data(0);
    }
}

void OLED_reset() {
  for (uint8_t i = 0; i < 8; i++) {
    OLED_clear_line(i);
  }
}

void OLED_pos(uint8_t row, uint8_t col) {
	OLED_goto_line(row);

	OLED_write_command(0x00 | (col & 0x0F));
	OLED_write_command(0x10 | ((col & 0xF0) >> 4));
}

void OLED_write_data(uint8_t dt) {
  volatile char *ptr = (char *) 0x1200;
  *ptr = dt;
}

void OLED_print(unsigned char *data) {
	while (*data != '\0') {
    for (uint8_t i = 0; i < 8; i++) {
      OLED_write_data(font8[*(data)][i]);
    }
		data++;
	}
}

void OLED_set_brightness(uint8_t level) {
	OLED_write_command(0x81);
	OLED_write_command(level);
}