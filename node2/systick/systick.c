#include "systick.h"

#include "sam.h"

volatile uint64_t clock;

void SysTick_Handler() {
  clock++;
}

void SysTick_Init() {
  // Enable SysTick
  // 84MHz / 8 = 10.5MHz
  SysTick->CTRL |= (1 << SysTick_CTRL_ENABLE_Pos) | (1 << SysTick_CTRL_CLKSOURCE_Pos) | (1 << SysTick_CTRL_TICKINT_Pos);
  SysTick->LOAD |= 84;
}

void SysTick_Delay(uint32_t d) {
  uint64_t current = clock;
  while (clock < current + d); 
}