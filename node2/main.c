/*
 * Arduino2.c
 *
 * Created: 10/14/2022 3:37:43 PM
 * Author : itsom
 */ 

#include "sam.h"
#include "uart_and_printf/printf-stdarg.h"
#include "uart_and_printf/uart.h"
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

    PMC->PMC_PCR |= PMC_PCR_EN | (ID_PWM << PMC_PCR_PID_Pos);
    PMC->PMC_PCER1 = 1 << (ID_PWM - 32);


    REG_PIOC_WPMR = 0x50494F00; // Write protect disable
    PIOC->PIO_PDR |= PIO_PDR_P18;
    PIOC->PIO_ABSR |= PIO_ABSR_P18;

    PWM->PWM_CLK |= PWM_CLK_DIVA(1) | PWM_CLK_PREA(0b0101);
    PWM->PWM_CH_NUM[6].PWM_CMR |= (PWM_CMR_CPRE_CLKA | PWM_CMR_CPOL);
    PWM->PWM_CH_NUM[6].PWM_CPRD |= PWM_CPRD_CPRD(0xCD14);
    // Min 0x93B (2363) Max 0x1589 (5513) 0.9ms on to 2.1ms
    PWM->PWM_CH_NUM[6].PWM_CDTY |= PWM_CDTY_CDTY(0x1589);
    PWM->PWM_ENA |= PWM_ENA_CHID6;





    // init_LED();

    while (1) 
    {
		  REG_PIOA_ODSR = 1 << 19;
		
		//REG_PIOB_CODR = 1 << 27;
    }
}
