#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "CountSensor.h"


int main(void)
{
	OLED_Init();
	PB5_Init();
	AFIO_Config();
	EXTI_Config();
	NVIC_Config();

	OLED_ShowString(1 , 1 , "Count:");
	while(1)
	{
		uint32_t COUNT = Count_Get();
		OLED_ShowNum(1 , 7 , COUNT , 4);
	}
}

