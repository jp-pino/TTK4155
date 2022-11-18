/*
 * interrupt.c
 *
 * Author: Gustav O. Often and Eivind H. J�lsgard
 *
 * For use in TTK4155 Embedded and Industrial Computer Systems Design
 * NTNU - Norwegian University of Science and Technology
 *
 */ 

#include "can_interrupt.h"

#include <stdio.h>
#include "sam.h"

#include "../uart_and_printf/printf-stdarg.h"
#include "../motor/motor.h"
#include "../systick/systick.h"

#include "can_controller.h"

#define DEBUG_INTERRUPT 0

#define MAX_ENCODER 1600

long map(long x, long in_min, long in_max, long out_min, long out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

/**
 * \brief CAN0 Interrupt handler for RX, TX and bus error interrupts
 *
 * \param void
 *
 * \retval 
 */

void CAN0_Handler( void )
{
	//if(DEBUG_INTERRUPT)printf("CAN0 interrupt\n\r");
	char can_sr = CAN0->CAN_SR; 
	
	//RX interrupt
	if(can_sr & (CAN_SR_MB1 | CAN_SR_MB2) )//Only mailbox 1 and 2 specified for receiving
	{
		CAN_MESSAGE message;
		if(can_sr & CAN_SR_MB1)  //Mailbox 1 event
		{
			can_receive(&message, 1);

		}
		else if(can_sr & CAN_SR_MB2) //Mailbox 2 event
		
		{
			can_receive(&message, 2);
		}
		else
		{
			printf("CAN0 message arrived in non-used mailbox\n\r");
		}

		if (message.id == 0x05) {
			// long pwm = map((int8_t)message.data[0], -128, 100, 2363, 5513);
			long pwm = map((uint8_t)message.data[2], 256, 0, 2363, 5513);
			// printf("PWM: %d - %d | ", message.data[2], pwm);
			PWM->PWM_CH_NUM[6].PWM_CDTYUPD |= PWM_CDTY_CDTY(pwm); 

			// long speed = map((int8_t)message.data[0], -128, 110, 8000, -8000);
			// long speed = map((uint8_t)message.data[1], 0, 256, 8000, -8000);
			long position = map((uint8_t)message.data[1], 0, 256, 0, MAX_ENCODER);
			// Motor_SetSpeed(speed);
			Motor_SetPosition(position);

			if (message.data[3]) {
      	REG_PIOA_ODSR &= ~(1 << 19);
				// SysTick_Delay(1000);
				// int i = 0;
				// while(i++ < 199999);
				// REG_PIOA_ODSR = 1 << 19;
				// i=0;
				// while(i++ < 199999);
			} else {
				REG_PIOA_ODSR = 1 << 19;
			}
      
		}
		
		

		if(DEBUG_INTERRUPT)printf("message id: %d ", message.id);
		if(DEBUG_INTERRUPT)printf("message data %x %x %x %x - length: %d\n\r", message.data[3], message.data[2], message.data[1], message.data[0], message.data_length);

    

		//if(DEBUG_INTERRUPT)printf("\n\r");
	}
	
	if(can_sr & CAN_SR_MB0)
	{
		//if(DEBUG_INTERRUPT) printf("CAN0 MB0 ready to send \n\r");
		
	//Disable interrupt
		CAN0->CAN_IDR = CAN_IER_MB0;

	}

	if(can_sr & CAN_SR_ERRP)
	{
		if(DEBUG_INTERRUPT)printf("CAN0 ERRP error\n\r");

	}
	if(can_sr & CAN_SR_TOVF)
	{
		if(DEBUG_INTERRUPT)printf("CAN0 timer overflow\n\r");

	}
	
	NVIC_ClearPendingIRQ(ID_CAN0);
	//sei();*/
}
