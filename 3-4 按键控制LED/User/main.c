#include "stm32f10x.h"   // Device header
#include "Delay.h"
#include "LED.h"
#include "Key.h"

int main(void)
{
	LED_Init_PAx(GPIO_Pin_0);
	LED_Init_PAx(GPIO_Pin_1);
	//一开始PA0与PA1处的LED都OFF
	PAx_LED_OFF(GPIO_Pin_0);
	PAx_LED_OFF(GPIO_Pin_1);
	
	PAx_Key_Init(GPIO_Pin_5);
	PAx_Key_Init(GPIO_Pin_7);
	
	uint16_t Key_num5 = 1 , Key_num7 = 1;
	
	while(1)
	{
		//Bit_RESET本来是枚举常量，是int型的，但前面函数的返回值是uint8_t，所以要强制类型转换
		
        Key_num5 = PAx_Key_Action(GPIO_Pin_5);
		
		if (Key_num5 == (uint16_t)0) //如果不把0强制类型转换的话，会把前面函数的返回值“类型提升”（不过在这里或者一般对结果没影响）
		{
			Delay_ms(20); //用代码“消抖”
			 GPIOA->ODR ^= (uint16_t)0x0001; //PA5控制PA0
			 while (PAx_Key_Action(GPIO_Pin_5) == (uint16_t)0) //等待按键松开，确保一次按压只触发一次
			 {
				 Delay_ms(10);
			 }
			Delay_ms(20); //用代码“消抖”
		}
		
		Key_num7 = PAx_Key_Action(GPIO_Pin_7);
		
		if (Key_num7 == (uint16_t)0) //如果不把0强制类型转换的话，会把前面函数的返回值“类型提升”（不过在这里或者一般对结果没影响）
		{
			Delay_ms(20); //用代码“消抖”
			 GPIOA->ODR ^= (uint16_t)0x0002;//PA6控制PA1
			 while (PAx_Key_Action(GPIO_Pin_7) == (uint16_t)0) //等待按键松开，确保一次按压只触发一次
			 {
				 Delay_ms(10);
			 }
			Delay_ms(20); //用代码“消抖”
		}
	
	}
}
