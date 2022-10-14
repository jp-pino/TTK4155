#include "mcp2515.h"

#include <stdint.h>

#include "../spi/spi.h"

message_t temp = {0, 0, 0, 0};
char buffer[8];

message_t MCP2515_read() {
  SPI_send_length("\x90\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00", 15);
  temp.id = SPI_getData()[1] << 3 | ((SPI_getData()[2] >> 5) & 0x07);
  temp.length = 8;//SPI_getData()[5] & 0x07; //MCP2515_read_reg(MCP_RXB0DLC) & 0x07;
  // SPI_send_length("\x92\x00\x00\x00\x00\x00\x00\x00\x00\x00", 10);
  for (uint8_t i = 0; i < temp.length; i++) {
    buffer[i] = SPI_getData()[i + 6];
  }
  temp.data = buffer;
  temp.remote = DATA_FRAME;

  return temp;
}

uint8_t MCP2515_read_byte() {
  return SPI_send_length("\x92\x00", 2);
}

void MCP2515_write_reg(uint8_t reg, uint8_t data) {
  char buffer[4];
  sprintf(buffer, "%c%c%c", MCP_WRITE, reg, data);
  SPI_send_length(buffer, 3);
}

uint8_t MCP2515_read_reg(uint8_t reg) {
  char buffer[4];
  sprintf(buffer, "%c%c\x00", MCP_WRITE, reg);
  return SPI_send_length(buffer, 3);
}

void MCP2515_write(message_t message) {
  // char buffer[128];
  MCP2515_write_reg(MCP_TXB0SIDH, (uint8_t)((message.id >> 3) & 0xFF));
  MCP2515_write_reg(MCP_TXB0SIDL, (uint8_t)(message.id << 5) & 0xFF);
  MCP2515_write_reg(MCP_TXB0DLC, (uint8_t)((message.length & 0x0F) | (message.remote << 6)));
  for (int i = 0; i < message.length; i++) {
    // sprintf(buffer, "\x41%c", message.data[i]);
    // SPI_send_length(buffer, 2);
    MCP2515_write_reg(0x36 + i, message.data[i]);
  }
}

void MCP2515_rts() {
  SPI_send_length("\x81", 1);
}

void MCP2515_bit_modify(uint8_t address, uint8_t mask, uint8_t data) {
  char buffer[5];
  sprintf(buffer, "\x05%c%c%c", address, mask, data);
  SPI_send_length(buffer, 4);
}

void MCP2515_reset() {
  SPI_send_length("\xc0", 1);
}

uint8_t MCP2515_read_status() {
  return SPI_send_length("\xA0\x00\x00", 3);
}
