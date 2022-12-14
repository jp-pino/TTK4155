#include "usart.h"

#include <stdlib.h>
#include <stdio.h>
#include <avr/io.h>


void USART_Transmit(unsigned char data) {
  /* Wait for empty transmit buffer */ 
  while ( !( UCSR0A & (1<<UDRE0)) );
  /* Put data into buffer, sends the data */ 
  UDR0 = data;
}

unsigned char USART_Receive() {
  /* Wait for data to be received */ 
  while ( !(UCSR0A & (1<<RXC0)));
  /* Get and return received data from buffer */ 
  return UDR0;
}

void USART_Initialize(unsigned int baud) {
  // Set baudrate
  UBRR0H = (unsigned char) (baud >> 8);
  UBRR0L = (unsigned char) (baud);
  // Enable receiver and transmitter
  UCSR0B = (1 << RXEN0) | (1 << TXEN0);
  // Set frame format: 8 data, 2 stop bit
  UCSR0C = (1 << URSEL0) | (1 << USBS0) | (3 << UCSZ00);

  // Connect to stdio
  fdevopen(&USART_Transmit, &USART_Receive);
}

void USART_Flush() {
  unsigned char dummy;
  while ( UCSR0A & (1<<RXC0) ) dummy = UDR0; 
}