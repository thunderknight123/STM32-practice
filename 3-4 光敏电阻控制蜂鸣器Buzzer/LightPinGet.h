#ifndef __LightPinGet_H
#define __LightPinGet_H

void Buzzer_Init_GPIOB(uint16_t Pin);
void LightSensor_Init_GPIOB(uint16_t Pin);
void Buzzer_ON(uint16_t Pin);
void Buzzer_OFF(uint16_t Pin);
void turn_Buzzer(uint16_t Pin); //改变Buzzer的引脚电平的，不过main.c里面没用到
uint8_t Get_LightSensor(uint16_t Pin);

#endif
