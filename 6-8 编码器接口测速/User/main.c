#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "TIM.h"
#include "EncoderSpeed.h"

int main(void)
{
	OLED_Init();
	TIM2_Init();
	EncoderSpeed_Init();

	OLED_ShowString(1 , 1 , "Sped&Drec:");
	OLED_ShowString(2 , 1 , "Cout:");
	while(1)
	{
		OLED_ShowSignedNum(1, 12, Get_Temp(), 4);
		
	}
}

