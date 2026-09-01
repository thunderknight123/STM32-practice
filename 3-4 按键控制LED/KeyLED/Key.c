#include "stm32f10x.h"   		// Device header
#include "LED.h"

//同样是配置GPIO口（让按键Key处的Pin在Key未按下时是VDD（3.3V），按下后为GND（约0V））
void PAx_Key_Init(uint16_t x)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU; //配置为上拉输入模式（Input Pull-up）,悬空时Pin输出VDD（3.3V）
	GPIO_InitStruct.GPIO_Pin = x; //x是传的参数，如：GPIO_Pin_x（x 可以是 0~15）
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz; //事实上，输入模式下设置Speed没有用
	
	GPIO_Init(GPIOA , &GPIO_InitStruct);
}

//检查按键所在Pin的电平状态的函数
uint16_t PAx_Key_Action(uint16_t x)
{
	uint16_t Key_num = 0;
	if (GPIO_ReadInputDataBit(GPIOA , x) == (uint8_t)Bit_RESET) //Bit_RESET本来是枚举常量，是int型的，但前面函数的返回值是uint8_t，所以要强制类型转换
	{
		Key_num = 0; //0 -> 低电平
	}
	else
	{
		Key_num = 1;
	}
	
	return Key_num;
}

//最后一行要空出来（其实空出来的本质是以换行符结尾）
