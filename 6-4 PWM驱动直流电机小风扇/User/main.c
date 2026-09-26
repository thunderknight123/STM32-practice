#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "EnCoder.h"
#include "Motor.h"


int main(void)
{
	OLED_Init();
	PB01_Init();
	AFIO_Config();
	EXTI_Config();
	NVIC_Config();
	
	Motor_Init();
	
	int16_t SPEED = 0;
	OLED_ShowString(1 , 1 , "Speed:");
	while(1)
	{
		/*==========================================*/
		if (SPEED > -11 && SPEED < 11)
		{
			OLED_ShowSignedNum(1 , 7 , SPEED += Count_Get(), 4);
			if (SPEED > 0 && SPEED < 10)
			{
				OLED_ShowString(2, 7, "Right    ");
			}
			else if(SPEED < 0 && SPEED > -10)
			{
				OLED_ShowString(2, 7, "Left     ");
			}
			else if(SPEED == 0)
			{
				OLED_ShowString(2, 7, "STOP     ");
			}
		}
		else if (SPEED <= -10)
		{
			SPEED = -10;
			OLED_ShowString(2, 7, "TOO FAST!");
		}
		else if (SPEED >= 10)
		{
			SPEED = 10;
			OLED_ShowString(2, 7, "TOO FAST!");
		}
	    /*==========================================*/
		
		RotationDirection(SPEED);
	}
}
