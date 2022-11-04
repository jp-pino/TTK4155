#include "adc.h"

#include <avr/interrupt.h>
#include <avr/io.h>

volatile adc_t data;
volatile adc_t offset;
volatile adc_t result;

volatile char current = 0;
volatile int first = 0;

volatile char *adc = (char *)0x1400;

long map(long x, long in_min, long in_max, long out_min, long out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void ADC_init() {
  // Initialization for the clock 2 MHz
  DDRD |= (1 << 4);
  TCCR3A |= (1 << COM3A0);
  TCCR3B |= (1 << WGM32) | (1 << CS30);
  OCR3AH = 0x00;
  OCR3AL = 0x00;

  //// Initialization for PE0 - INT0

  // // Disable global interrupts
  cli();
  // // Interrupt on rising edge PE0
  EMCUCR |= (1 << ISC2);
  // Enable interrupt on PE0
  GICR |= (1 << INT2);
  // Enable global interrupts
  sei();

  adc[0] = 0x00;
}

volatile adc_t ADC_get_data(offset_sel_t offset_correction) {
  switch (offset_correction) {
    case OFFSET:
      result.AIN0 = data.AIN0 - offset.AIN0;
      result.AIN1 = data.AIN1 - offset.AIN1;
      result.AIN2 = data.AIN2 - offset.AIN2;
      result.AIN3 = data.AIN3 - offset.AIN3;

      if (result.AIN0 < -128) result.AIN0 = -128;
      if (result.AIN1 < -128) result.AIN1 = -128;
      if (result.AIN2 < -128) result.AIN2 = -128;
      if (result.AIN3 < -128) result.AIN3 = -128;
      break;
    case MAP:
      if (result.AIN0 < offset.AIN0) {
        result.AIN0 = map(data.AIN0, -100, 0, -100, offset.AIN0);
      } else {
        result.AIN0 = map(data.AIN0, 0, 100, offset.AIN0, 100);
      }
      
      if (result.AIN1 < offset.AIN1) {
        result.AIN1 = map(data.AIN1, -100, 0, -100, offset.AIN1);
      } else {
        result.AIN1 = map(data.AIN1, 0, 100, offset.AIN1, 100);
      }
      
      if (result.AIN2 < offset.AIN2) {
        result.AIN2 = map(data.AIN2, -100, 0, -100, offset.AIN2);
      } else {
        result.AIN2 = map(data.AIN2, 0, 100, offset.AIN2, 100);
      }
      
      if (result.AIN3 < offset.AIN3) {
        result.AIN3 = map(data.AIN3, -100, 0, -100, offset.AIN3);
      } else {
        result.AIN3 = map(data.AIN3, 0, 100, offset.AIN3, 100);
      }
      break;
    case NO_CORRECTION:
    default:
      result.AIN0 = data.AIN0;
      result.AIN1 = data.AIN1;
      result.AIN2 = data.AIN2;
      result.AIN3 = data.AIN3;
      break;
  }

  return result;
}



long i = 0;
ISR(INT2_vect) {
  volatile unsigned char *adc = (unsigned char *)0x1400;

  if (first == 0) {
    if(i++ < 100) { 
      adc[0] = 0x00;
      return;
    }
    offset.AIN0 = (long)(adc[0]);
    offset.AIN1 = (long)(adc[0]);
    offset.AIN2 = (long)(adc[0]);
    offset.AIN3 = (long)(adc[0]);
    first = 1;

    adc[0] = 0x00;
    return;
  }

  data.AIN0 = (long)(adc[0]);
  data.AIN1 = (long)(adc[0]);
  data.AIN2 = (long)(adc[0]);
  data.AIN3 = (long)(adc[0]);

  adc[0] = 0x00;
}