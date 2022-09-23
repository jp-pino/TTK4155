/*
 * oled.h
 *
 * Created: 9/23/2022 12:17:38 PM
 *  Author: itsom
 */ 

#ifndef __LIB_OLED_OLED_H__
#define __LIB_OLED_OLED_H__

#include <stdint.h>

void OLED_write_command(uint8_t comm);

void OLED_init();
void OLED_reset();
void OLED_goto_line(uint8_t line);
void OLED_clear_line(uint8_t line);
void OLED_pos(uint8_t row, uint8_t col);
void OLED_write_data(uint8_t dt);
void OLED_print(unsigned char *data);
void OLED_set_brightness(uint8_t level);




#endif//__LIB_OLED_OLED_H__