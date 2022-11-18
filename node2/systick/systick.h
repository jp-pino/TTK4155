#ifndef __SYSTICK_SYSTICK_H__
#define __SYSTICK_SYSTICK_H__

#include <stdint.h>


void SysTick_Init();
void SysTick_Delay(uint32_t d);
uint64_t SysTick_GetTime();

#endif//__SYSTICK_SYSTICK_H__