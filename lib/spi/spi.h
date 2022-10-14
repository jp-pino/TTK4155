#ifndef __LIB_SPI_SPI_H__
#define __LIB_SPI_SPI_H__

#include <stdint.h>

typedef enum {
  CPOL_LOW, // LOW WHEN IDLE
  CPOL_HIGH  // HIGH WHEN IDLE
} cpol_t;

typedef enum {
  CPHA_LEADING, 
  CPHA_TRAILING
} cpha_t;

typedef enum {
  DORD_MSB, 
  DORD_LSB
} dord_t;

void SPI_init(cpol_t cpol, cpha_t cpha, dord_t dord);
uint8_t SPI_send(uint8_t* byte);
uint8_t SPI_send_length(uint8_t* byte, uint8_t length);

#endif  //__LIB_SPI_SPI_H__