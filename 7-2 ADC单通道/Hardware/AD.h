#ifndef __AD_H
#define __AD_H

void AD_Init(void);
uint16_t Get_ConvResult(ADC_TypeDef* ADCx);
float Get_Voltage(ADC_TypeDef* ADCx);

#endif
