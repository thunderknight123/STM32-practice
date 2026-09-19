#ifndef __COUNT__SENSOR__H
#define __COUNT__SENSOR__H

void PB5_Init(void);
void AFIO_Config(void);
void EXTI_Config(void);
void NVIC_Config(void);
uint32_t Count_Get(void);
void EXTI9_5_IRQHandler(void);

#endif
