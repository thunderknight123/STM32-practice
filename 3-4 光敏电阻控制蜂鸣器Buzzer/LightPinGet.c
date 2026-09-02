#include "stm32f10x.h"                  // Device header

//初始化GPIO口,而且我把他们插在GPIOB上了（光敏电阻，蜂鸣器的I/O引脚接在哪个Pin就传哪个Pin）
void Buzzer_Init_GPIOB(uint16_t Pin)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Pin = Pin;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOB , &GPIO_InitStruct);
}

//初始化GPIO口，光敏电阻的，由于BuzLzer与Photoresisor的GPIO_Mode不一样，所以写了两个函数来分别初始化
void LightSensor_Init_GPIOB(uint16_t Pin)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU; //给光敏电阻的DO引脚连接的GPIO_Pin配置为上拉输入（Input Pull-Up）
	GPIO_InitStruct.GPIO_Pin = Pin;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz; //输入模式下这个其实没用
	
	GPIO_Init(GPIOB , &GPIO_InitStruct);
}

//打开蜂鸣器（低电平打开）
void Buzzer_ON(uint16_t Pin)
{
	GPIO_ResetBits(GPIOB , Pin);
}

//关闭蜂鸣器
void Buzzer_OFF(uint16_t Pin)
{
	GPIO_SetBits(GPIOB , Pin);
}

//改变蜂鸣器引脚的状态（3V3 <-> 0V）
void turn_Buzzer(uint16_t Pin)
{
	if (GPIO_ReadOutputDataBit(GPIOB , Pin) == (uint8_t)Bit_SET) //检测到Pin输出高电平，就将其转化为低电平
	{
		Buzzer_ON(Pin);
	}
	else //Pin输出低电平，就转为高电平
	{
		Buzzer_OFF(Pin);
	}
}
	
//获取光敏传感器处的引脚输入电平情况
uint8_t Get_LightSensor(uint16_t Pin)
{
	return GPIO_ReadInputDataBit(GPIOB , Pin);
}
