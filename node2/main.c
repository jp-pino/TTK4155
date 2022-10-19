#include <stdio.h>
#include <stdarg.h>
#include "uart_and_printf/uart.h"
#include "uart_and_printf/printf-stdarg.h"

#include "can/can_controller.h"
#include "can/can_interrupt.h"

#include "sam.h"


int main()
{
    SystemInit();

    WDT->WDT_MR = WDT_MR_WDDIS; //Disable Watchdog Timer

    configure_uart();
    printf("Hello World\n\r");

    // REG_PORT_DIRSET1 = PORT_PB27;
    PIOB->PIO_OER = PIO_PB27;
    while (1)
    {
        printf("Hello World\n\r");
        PIOB->PIO_PDR ^= 1 << 27;
    }
    
}