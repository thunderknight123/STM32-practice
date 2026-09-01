#include "stm32f10x.h"                  // Device header

//配置GPIO口的函数
void LED_Init_PAx(uint16_t x)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE); //开启（使能）时钟
	
	//然后创建结构体变量，给三个成员（Mode，Pin，Speed）赋值
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Pin = x; //这里的x就是传的参数，如：GPIO_Pin_0
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOA , &GPIO_InitStruct);
}

//点亮PAx处引脚的LED的函数（LED是低电平点亮）
void PAx_LED_ON(uint16_t x) //x传的是GPIO_Pin_x
{
	GPIO_ResetBits(GPIOA , x);
}

//熄灭PAx处引脚的LED的函数（LED是高电平熄灭）
void PAx_LED_OFF(uint16_t x)
{
	GPIO_SetBits(GPIOA , x);
}
