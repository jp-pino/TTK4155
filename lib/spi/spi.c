#include "spi.h"

#include <avr/io.h>

void SPI_init(cpol_t cpol, cpha_t cpha, dord_t dord) {
  /* Set MOSI and SCK output, all others input */
  DDRB |= (1 << 4) | (1 << 5) | (1 << 7);
  DDRB &= ~(1 << 6);
  /* Enable SPI, Master, set clock rate fck/16 */
  SPCR |= (1 << SPE) | (1 << MSTR) | (1 << SPR0) | (1 << SPR1);

  if (cpol == CPOL_LOW) {
    SPCR &= ~(1 << CPOL);
  } else {
    SPCR |= 1 << CPOL;
  }

  if (cpha == CPHA_RISING) {
    SPCR &= ~(1 << CPHA);
  } else {
    SPCR |= 1 << CPHA;
  }

  if (dord == DORD_MSB) {
    SPCR &= ~(1 << DORD);
  } else {
    SPCR |= 1 << DORD;
  }


  // Slave deselect
  PORTB |= (1 << 4);
}

uint8_t SPI_send(uint8_t* byte, uint8_t length) {
  uint8_t data = 0x00;

  // Slave select
  PORTB &= ~(1 << 4);

  for (uint8_t i = 0; i < length; i++) {
    /* Start transmission */
    SPDR = *(byte++);

    /* Wait for transmission complete */ 
    while(!(SPSR & (1<<SPIF)));

    data = SPDR;
  }

  // Slave deselect
  PORTB |= (1 << 4);

  return data;
}
