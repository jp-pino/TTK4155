#include "game.h"

#include "../mcp2515/mcp2515.h"
#include "../oled/oled.h"

#define RST_SCORE_FLAG   0
#define RST_ENCODER_FLAG 1

uint8_t status = 0;

void GAME_Enable() {
  status = 1;
  MCP2515_write((message_t){0x06, { (1 << RST_SCORE_FLAG) }, 1, DATA_FRAME});
  MCP2515_rts();
}

void GAME_Disable() {
  status = 0;
  MCP2515_write((message_t){0x06, { (1 << RST_SCORE_FLAG) }, 1, DATA_FRAME});
  MCP2515_rts();
  SCREEN_reset();
  MENU_PrintCurrent();
}

uint8_t GAME_Status() {
  return status;
}

void GAME_ResetEncoder() {
  MCP2515_write((message_t){0x06, { (1 << RST_ENCODER_FLAG) }, 1, DATA_FRAME});
  MCP2515_rts();
}