#include "adc.h"

#include "sam.h"

void ADC_Init() {
    // ADC->ADC_WPMR |= 0x414443;

    // ADC_CR Control Register Force Reset
    // ADC->ADC_CR |= ADC_CR_SWRST;

    // ADC_MR Mode Register
    ADC->ADC_MR |= ADC_MR_LOWRES_BITS_12 | ADC_MR_TRGEN_DIS 
        | ADC_MR_SLEEP_NORMAL | ADC_MR_FWUP_OFF | ADC_MR_FREERUN_ON | ADC_MR_PRESCAL(0) 
        | ADC_MR_STARTUP_SUT0 | ADC_MR_SETTLING_AST17 | ADC_MR_ANACH_NONE | ADC_MR_TRACKTIM(2) 
        | ADC_MR_TRANSFER(2) | ADC_MR_USEQ_NUM_ORDER;
    // ADC->ADC_MR = ADC_MR_FREERUN_ON;


    // Disable PIO
    // PIOA->PIO_PDR |= PIO_PDR_P16;

    // ADC_CHER Channel Enable Register
    ADC->ADC_CHER |= ADC_CHER_CH7;

    // Enable clock
    PMC->PMC_PCR |= PMC_PCR_EN | (ID_ADC << PMC_PCR_PID_Pos);
    PMC->PMC_PCER1 |= 1 << (ID_ADC - 32);

    // ADC_CR Start conversion?
    ADC->ADC_CR |= ADC_CR_START;
}


uint16_t ADC_GetData() {
    // ADC_CR Start conversion?
    // ADC->ADC_CR |= ADC_CR_START;
    // ADC_CDR[0] Channel 0 Data register
    // printf("CH: %d ", (ADC->ADC_LCDR && ADC_LCDR_CHNB_Msk) >>  ADC_LCDR_CHNB_Pos);
    // return ADC->ADC_LCDR && ADC_LCDR_LDATA_Msk;
    return ADC->ADC_CDR[7];
}