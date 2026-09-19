#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "EnCoder.h"


int main(void)
{
	OLED_Init();
	PB01_Init();
	AFIO_Config();
	EXTI_Config();
	NVIC_Config();
	
	int16_t COUNT = 0;
	OLED_ShowString(1 , 1 , "Count:");
	while(1)
	{
		OLED_ShowSignedNum(1 , 7 , COUNT += Count_Get(), 4);
	}
}

