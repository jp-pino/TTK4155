#include <stdio.h>

#include "lib/usart/usart.h"

#define FOSC 4951200 // Clock Speed
#define BAUD 9600
#define MYUBRR FOSC/16/BAUD-1

int main() {
  USART_Initialize(MYUBRR);
  while (1) {
    printf("Juan Pablo\r\nIa Tsomalia\r\nSimen Bjerkestrand\n");
  }
  return 0;
}