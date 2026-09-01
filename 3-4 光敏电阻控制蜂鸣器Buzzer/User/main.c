#include "stm32f10x.h"                  // Device header
#include "LightPinGet.h"
#include "Delay.h"

//蜂鸣器是“低电平”响，
int main(void)
{
	Buzzer_Init_GPIOB(GPIO_Pin_12);
	
	while(1)
	{
		Buzzer_ON(GPIO_Pin_12);
		Delay_ms(500);
		Buzzer_OFF(GPIO_Pin_12);
		Delay_ms(500);
	}
}
