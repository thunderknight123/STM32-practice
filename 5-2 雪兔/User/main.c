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
	char arr[] = " sonwball is g?? ";
	while(1)
	{
		if (COUNT >= 0 && COUNT < 16)
		{
			COUNT += Count_Get();
			OLED_ShowChar(1 , COUNT , arr[COUNT]);
		}
	}
}

