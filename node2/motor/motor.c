#include "motor.h"

#include "sam.h"

#include "systick/systick.h"

#define DIR_PIN 10
#define EN_PIN  9
#define RST_PIN 1
#define SEL_PIN 2
#define OE_PIN 0

void Motor_Init() {
  // Motor    DIR = PD10 | EN = PD9 
  // Encoder  RST = PD1 | SEL = PD2 | OE
  // Data     PC1-8
  // PIOD->PIO_WPMR = 0x50494F00; // Write protect disable
  PIOD->PIO_OER |= (1 << DIR_PIN) | (1 << EN_PIN) | (1 << RST_PIN) | (1 << SEL_PIN) | (1 << OE_PIN); //  Enables the output on the I/O line.
  PIOD->PIO_ABSR |= (1 << DIR_PIN) | (1 << EN_PIN) | (1 << RST_PIN) | (1 << SEL_PIN) | (1 << OE_PIN); // Assigns the I/O line to the Peripheral B function.
  PIOD->PIO_PER |= (1 << DIR_PIN) | (1 << EN_PIN) | (1 << RST_PIN) | (1 << SEL_PIN) | (1 << OE_PIN); // Enable Register
  PIOD->PIO_SODR |= (1 << DIR_PIN) | (1 << EN_PIN) | (1 << RST_PIN) | (1 << SEL_PIN) | (1 << OE_PIN); // Output data register
  PIOD->PIO_OWER |= (1 << DIR_PIN) | (1 << EN_PIN) | (1 << RST_PIN) | (1 << SEL_PIN) | (1 << OE_PIN); // Enable output
  PIOD->PIO_ODSR |= (1 << EN_PIN) | (1 << RST_PIN);
  PIOD->PIO_ODSR &= ~(1 << RST_PIN);
  SysTick_Delay(10);
  PIOD->PIO_ODSR |= (1 << RST_PIN);

  PIOC->PIO_PER |= (0xFF << 1);
  PIOC->PIO_ABSR |= (0xFF << 1); // Assigns the I/O line to the Peripheral B function.
  PIOC->PIO_PUDR |= (0xFF << 1);
  PIOC->PIO_ODR |= 0xFFFF;


  // DACC
  DACC->DACC_MR |= DACC_MR_REFRESH(1) | DACC_MR_USER_SEL_CHANNEL1;
  DACC->DACC_CHER |= DACC_CHER_CH1;

  // Enable clock for DACC
  PMC->PMC_PCR |= PMC_PCR_EN | (ID_DACC << PMC_PCR_PID_Pos);
  PMC->PMC_PCER1 |= 1 << (ID_DACC - 32);
  
  // Enable clock
  PMC->PMC_PCR |= PMC_PCR_EN | (ID_PIOC << PMC_PCR_PID_Pos);
  PMC->PMC_PCER0 |= 1 << (ID_PIOC);
}


void Motor_setSpeed(int32_t speed) {
  if (speed < 0) {
    PIOD->PIO_ODSR |= (1 << DIR_PIN);
    speed *= -1;
  } else {
    PIOD->PIO_ODSR &= ~(1 << DIR_PIN);
  }

  DACC->DACC_CDR = DACC_CDR_DATA((uint32_t)speed);
}

uint16_t Motor_GetCount() {
  uint16_t data = 0;
  // Set OE low to sample and hold encoder value
  PIOD->PIO_ODSR &= ~(1 << OE_PIN);
  // Set SEL low (high byte)
  PIOD->PIO_ODSR &= ~(1 << SEL_PIN);
  SysTick_Delay(20);
  // Read high byte
  // printf("High Byte: %x ", PIOC->PIO_PDSR & 0xFF);
  data = (((PIOC->PIO_PDSR >> 1) & 0xFF) << 8);
  // Set SEL high (low byte)
  PIOD->PIO_ODSR |= (1 << SEL_PIN);
  SysTick_Delay(20);
  // Read low byte
  // printf("Low Byte: %x\n\r", PIOC->PIO_PDSR & 0xFF);
  data |= ((PIOC->PIO_PDSR >> 1) & 0xFF);
  // Set OE high to sample and hold encoder value
  PIOD->PIO_ODSR |= (1 << OE_PIN);

  return data;
}