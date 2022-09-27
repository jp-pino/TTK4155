/*
 * oled_init.c
 *
 * Created: 9/23/2022 12:15:17 PM
 *  Author: itsom
 */
#include "oled.h"

#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <stdint.h>

#include "fonts.h"

volatile uint8_t *ext_ram = (uint8_t *)0x1800;
cursor_t cursor = {0, 0};

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

  // Initialization for the apple
  TCCR1A |= (1 << COM1A0);
  TCCR1B |= (1 << WGM12) | (1 << CS10);
  OCR1AH = (uint8_t)(82519 >> 8) & 0xFF;
  OCR1AL = (uint8_t)(82519 & 0xFF);
  TIMSK |= (1 << OCIE1A);

  SCREEN_reset();
  OLED_reset();
}

ISR(TIMER1_COMPA_vect) {
  for (uint8_t i = 0; i < 8; i++) {
    OLED_goto_line(i);
    for (uint8_t j = 0; j < 128; j++) {
      OLED_write_data(ext_ram[i * 128 + j]);
    }
  }
}

void OLED_write_command(uint8_t comm) {
  volatile char *ptr = (char *)0x1000;
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
  volatile char *ptr = (char *)0x1200;
  *ptr = dt;
}

void OLED_print_char8(unsigned char c) {
  for (int i = 0; i < 8; i++) {
    uint8_t aux = pgm_read_word(&(font8[c - 32][i]));
    OLED_write_data(aux);
  }
}

void OLED_print_char5(unsigned char c) {
  for (int i = 0; i < 5; i++) {
    uint8_t aux = pgm_read_word(&(font5[c - 32][i]));
    OLED_write_data(aux);
  }
}

void OLED_print_char4(unsigned char c) {
  for (int i = 0; i < 4; i++) {
    uint8_t aux = pgm_read_word(&(font4[c - 32][i]));
    OLED_write_data(aux);
  }
}

void OLED_set_brightness(uint8_t level) {
  OLED_write_command(0x81);
  OLED_write_command(level);
}

void SCREEN_print_char8(unsigned char c) {
  for (int i = 0; i < 8; i++) {
    uint8_t aux = pgm_read_word(&(font8[c - 32][i]));
    SCREEN_write_data(aux);
  }
}

void SCREEN_print_char5(unsigned char c) {
  for (int i = 0; i < 5; i++) {
    uint8_t aux = pgm_read_word(&(font5[c - 32][i]));
    SCREEN_write_data(aux);
  }
}

void SCREEN_print_char4(unsigned char c) {
  for (int i = 0; i < 4; i++) {
    uint8_t aux = pgm_read_word(&(font4[c - 32][i]));
    SCREEN_write_data(aux);
  }
}

void SCREEN_reset() {
  for (cursor.row = 0; cursor.row < 8; cursor.row++) {
    for (cursor.col = 0; cursor.col < 128; cursor.col++) {
      ext_ram[cursor.row * 128 + cursor.col] = 0x00;
    }
  }
  cursor.row = 0;
  cursor.col = 0;
}

void SCREEN_goto_line(uint8_t line) {
  cursor.row = line;
  cursor.col = 0;
}

void SCREEN_pixel(uint8_t x, uint8_t y, bit_t val) {
  if (val == ZERO) {
    ext_ram[y + x / 8] &= ~(1 << (x % 8));
  } else {
    ext_ram[y + x / 8] |= (1 << (x % 8));
  }
}

void SCREEN_pos(uint8_t row, uint8_t col) {
  cursor.row = row;
  cursor.col = col;
}

void SCREEN_write_data(uint8_t dt) {
  ext_ram[cursor.row * 128 + cursor.col] = dt;
  if (++(cursor.col) >= 128) {
    cursor.col = 0;
  }
}

void SCREEN_print(char *data, void (*f)(unsigned char)) {
  while (*data != '\0') {
    f(*data++);
  }
}

void SCREEN_line(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1) {
  int dx, dy, p, x, y;
  dx = x1 - x0;
  dy = y1 - y0;
  x = x0;
  y = y0;
  p = 2 * dy - dx;
  while (x < x1) {
    SCREEN_pixel(x, y, ONE);
    if (p >= 0) {
      y = y + 1;
      p = p + 2 * dy - 2 * dx;
    } else {
      p = p + 2 * dy;
    }
    x = x + 1;
  }
}

void SCREEN_circle(uint8_t x_centre, uint8_t y_centre, uint8_t r) {
  int x = r, y = 0;

  // Printing the initial point on the axes
  // after translation
  SCREEN_pixel(x + x_centre, y + y_centre, ONE);

  // When radius is zero only a single
  // point will be printed
  if (r > 0) {
    SCREEN_pixel(x + x_centre, -y + y_centre, ONE);
    SCREEN_pixel(y + x_centre, x + y_centre, ONE);
    SCREEN_pixel(-y + x_centre, x + y_centre, ONE);
  }

  // Initialising the value of P
  int P = 1 - r;
  while (x > y) {
    y++;

    // Mid-point is inside or on the perimeter
    if (P <= 0) P = P + 2 * y + 1;
    // Mid-point is outside the perimeter
    else {
      x--;
      P = P + 2 * y - 2 * x + 1;
    }

    // All the perimeter points have already been printed
    if (x < y) break;

    // Printing the generated point and its reflection
    // in the other octants after translation
    SCREEN_pixel(x + x_centre, y + y_centre, ONE);
    SCREEN_pixel(-x + x_centre, y + y_centre, ONE);
    SCREEN_pixel(x + x_centre, -y + y_centre, ONE);
    SCREEN_pixel(-x + x_centre, -y + y_centre, ONE);

    // If the generated point is on the line x = y then
    // the perimeter points have already been printed
    if (x != y) {
      SCREEN_pixel(y + x_centre, x + y_centre, ONE);
      SCREEN_pixel(-y + x_centre, x + y_centre, ONE);
      SCREEN_pixel(y + x_centre, -x + y_centre, ONE);
      SCREEN_pixel(-y + x_centre, -x + y_centre, ONE);
    }
  }
}