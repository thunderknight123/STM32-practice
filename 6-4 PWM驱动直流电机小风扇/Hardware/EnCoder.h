#ifndef __EN__CODER__H
#define __EN__CODER__H

void PB01_Init(void);
void AFIO_Config(void);
void EXTI_Config(void);
void NVIC_Config(void);
int16_t Count_Get(void);
void EXTI0_IRQHandler(void);
void EXTI1_IRQHandler(void);

#endif
