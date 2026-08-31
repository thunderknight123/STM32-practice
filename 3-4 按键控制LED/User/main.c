#include "stm32f10x.h"   // Device header
#include "Delay.h"
#include "LED.h"

int main(void)
{
	LED_Init_PAx(GPIO_Pin_0);
	LED_Init_PAx(GPIO_Pin_1);
	
	while(1)
	{
		PAx_LED_ON(GPIO_Pin_0);
		PAx_LED_OFF(GPIO_Pin_1);
		Delay_ms(500);
		PAx_LED_OFF(GPIO_Pin_0);
		PAx_LED_ON(GPIO_Pin_1);
		Delay_ms(500);
	}
}
