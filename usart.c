#include <avr/io.h>
#include "usart.h"


void USART_Initialize(unsigned int baud) {
  // Set baudrate
  UBRR0H = (unsigned char) (baud >> 8);
  UBRR0L = (unsigned char) (baud);
  // Enable receiver and transmitter
  UCSR0B = (1 << RXEN0) | (1 << TXEN0);
  // Set frame format: 8 data, 2 stop bit
  UCSR0C = (1 << URSEL0) | (1 << USBS0) | (3 << UCSZ00);
}