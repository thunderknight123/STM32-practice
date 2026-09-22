#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "TIM.h"

int main(void)
{
	OLED_Init();
	PA0_Init();
	TIM_Init();

	OLED_ShowString(1 , 1 , "time:");
	OLED_ShowString(2 , 1 , "Cout:");
	while(1)
	{
		OLED_ShowNum(1, 6, Get_Num(), 4);
		OLED_ShowNum(2, 6, TIM_GetCounter(TIM2), 4);
	}
}

