/*
 * oled.h
 *
 * Created: 9/23/2022 12:17:38 PM
 *  Author: itsom
 */ 

#ifndef __LIB_OLED_OLED_H__
#define __LIB_OLED_OLED_H__

#include <stdint.h>

#define SCREEN_W 128
#define SCREEN_H 64

typedef struct {
  uint8_t col;
  uint8_t row;
} cursor_t;

typedef enum {
  ZERO,
  ONE
} bit_t;

void OLED_write_command(uint8_t comm);

void OLED_init();
void OLED_reset();
void OLED_goto_line(uint8_t line);
void OLED_clear_line(uint8_t line);
void OLED_pos(uint8_t row, uint8_t col);
void OLED_write_data(uint8_t dt);
void OLED_set_brightness(uint8_t level);

void OLED_print_char8(unsigned char c);
void OLED_print_char5(unsigned char c);
void OLED_print_char4(unsigned char c);

void SCREEN_print_char8(unsigned char c);
void SCREEN_print_char5(unsigned char c);
void SCREEN_print_char4(unsigned char c);

void SCREEN_reset();
void SCREEN_goto_line(uint8_t line);
void SCREEN_pixel(uint8_t x, uint8_t y, bit_t val);
void SCREEN_pos(uint8_t row, uint8_t col);
void SCREEN_write_data(uint8_t dt);
void SCREEN_print(char *data, void (*f)(unsigned char));
void SCREEN_line(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1);
void SCREEN_circle(uint8_t x_centre, uint8_t y_centre, uint8_t r);


#endif//__LIB_OLED_OLED_H__