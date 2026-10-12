#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "MyDMA.h"
#include "AD.h"


int main(void)
{
	OLED_Init();
	MyDMA_Init();
	AD_Init();
	
	OLED_ShowString(1 , 1 , "AD 0:");
	OLED_ShowString(2 , 1 , "AD 1:");
	OLED_ShowString(3 , 1 , "AD 2:");
	OLED_ShowString(4 , 1 , "AD 3:");

		
	while(1)
	{
		ConvStart(ADC1);
		OLED_ShowNum(1, 7, AD_Value[0], 4);
		OLED_ShowNum(2, 7, AD_Value[1], 4);
		OLED_ShowNum(3, 7, AD_Value[2], 4);
		OLED_ShowNum(4, 7, AD_Value[3], 4);
	}
}

