#ifndef __LIB_GAME_GAME_H__
#define __LIB_GAME_GAME_H__

#include <stdint.h>

void GAME_Enable();
void GAME_Disable();
uint8_t GAME_Status();
void GAME_ResetEncoder();

#endif//__LIB_GAME_GAME_H__