#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "IC.h"

int main(void)
{
	OLED_Init();
	IC_Init();

	OLED_ShowString(1 , 1 , "Freq:00000Hz");
	OLED_ShowString(2 , 1 , "Duty:000%");
	
	TIM_SetCompare1(TIM2, 50);
	TIM_PrescalerConfig(TIM2, 720 - 1, TIM_PSCReloadMode_Update);
	while(1)
	{
		OLED_ShowNum(1, 6, IC_GetFreq(), 5);
		OLED_ShowNum(2, 6, IC_GetDuty(), 3);
	}
}

