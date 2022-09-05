#ifndef __USART_USART_H__
#define __USART_USART_H__

void USART_Initialize(unsigned int baud);

void USART_Transmit(unsigned char data);
unsigned char USART_Receive();
void USART_Flush();

#endif//__USART_USART_H__