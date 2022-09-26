#ifndef __LIB_ADC_ADC_H__
#define __LIB_ADC_ADC_H__

typedef enum { OFFSET, MAP, NO_CORRECTION } offset_sel_t;

typedef struct {
  long AIN0;
  long AIN1;
  long AIN2;
  long AIN3;
} adc_t;

void ADC_init();
volatile adc_t ADC_get_data(offset_sel_t offset_correction);

#endif//__LIB_ADC_ADC_H__