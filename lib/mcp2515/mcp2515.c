#include "mcp2515.h"

#include <stdint.h>

uint8_t MCP2515_read() {
  return SPI_send("\x90\x00", 2);
}

void MCP2515_write(uint8_t data) {
  char buffer[3];
  sprintf(buffer, "\x40%c", data);
  SPI_send(buffer, 2);
}

void MCP2515_rts() {
  SPI_send("\x80", 1);
}

void MCP2515_bit_modify(uint8_t address, uint8_t mask, uint8_t data) {
  char buffer[5];
  sprintf(buffer, "\x05%c%c%c", address, mask, data);
  SPI_send(buffer, 4);
}


void MCP2515_reset() {
  SPI_send("\xc0", 1);
}

uint8_t MCP2515_read_status() {
  return SPI_send("\xA0\x00\x00", 3);
}
