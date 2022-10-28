/*
 * Arduino2.c
 *
 * Created: 10/14/2022 3:37:43 PM
 * Author : itsom
 */ 

#include "sam.h"
#include "uart/printf-stdarg.h"
#include "uart/uart.h"
#include "can/can_controller.h"
#include "can/can_interrupt.h"

void init_LED()
{
  
  REG_PIOA_WPMR = 0x50494F00; // Write protect disable
  REG_PIOA_OER  = 1 << 19; //  Enables the output on the I/O line.
  REG_PIOA_ABSR = 1 << 19; // Assigns the I/O line to the Peripheral B function.
  REG_PIOA_PER  = 1 << 19; // Enable REgister
  
  REG_PIOA_SODR = 1 << 19; // Output data register
  
  
  REG_PIOA_OWER = 1 << 19;
  
  
}

int main(void)
{
    /* Initialize the SAM system */
    SystemInit();
	configure_uart();
	// Can Init
    can_init_def_tx_rx_mb((2 << CAN_BR_PHASE2_Pos) | (1 << CAN_BR_PHASE1_Pos) | (1 << CAN_BR_PROPAG_Pos) | (1 << CAN_BR_SJW_Pos) | (20 << CAN_BR_BRP_Pos) | (CAN_BR_SMP_ONCE));
	
	init_LED();

    while (1) 
    {
		REG_PIOA_ODSR = 1 << 19;
		
		//REG_PIOB_CODR = 1 << 27;
    }
}
