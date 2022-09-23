/*
 * oled_init.c
 *
 * Created: 9/23/2022 12:15:17 PM
 *  Author: itsom
 */ 
#include <avr/pgmspace.h>

void init_program()
{
	write_c(0xae); // display off
	write_c(0xa1); //segment remap
	write_c(0xda); //common pads hardware: alternative
	write_c(0x12);
	write_c(0xc8); //common output scan direction:com63~com0
	write_c(0xa8); //multiplex ration mode:63
	write_c(0x3f);
	write_c(0xd5); //display divide ratio/osc. freq. mode
	write_c(0x80);
	write_c(0x81); //contrast control
	write_c(0x50);
	write_c(0xd9); //set pre-charge period
	write_c(0x21);
	write_c(0x20); //Set Memory Addressing Mode
	write_c(0x02);
	write_c(0xdb); //VCOM deselect level mode
	write_c(0x30);
	write_c(0xad); //master configuration
	write_c(0x00);
	write_c(0xa4); //out follows RAM content
	write_c(0xa6); //set normal display
	write_c(0xaf); // display on
}
void write_c(unsigned char comm)
{
   // D/C -> 0 R/W always 0
  volatile char *ptr = 0x1000;
  *ptr = comm;
}
void write_d(unsigned char dt)
{
	// D/C -> 1 R/W always 0
	volatile char *ptr = 0x1200;
	*ptr = dt;
}
