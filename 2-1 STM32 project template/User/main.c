#include "stm32f10x.h"                  // Device header

int main(void)
{
	//用库函数间接配置寄存器点灯（本来要你自己查手册，来自己配置寄存器的，现在有库函数帮你了，就省事了）
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC , ENABLE); //Peripheral 外围的 ENABLE 使能
	
	//创建结构体变量GPIO_InitStructure
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOC , &GPIO_InitStructure); //GPIOx, x = 13, 13 = c;
	
	//设置高电平（同样的参数，GPIO_ReSetBits可以将PC13号口置为低电平）
	//高电平PC13号口灯灭，低则亮
	GPIO_SetBits(GPIOC , GPIO_Pin_13);
	
	while (1) //while(1)死循环里放置你的的程序
	{
		
	}
	//不用return 0是因为while（1）永远死循环，根本执行不到return
	
	//而且C语言中main是唯一可以允许不写return非void函数，因为系统会自动加上return 0
	
	//就算return了也没有用，在有操作系统的环境下，return的数字是给操作系统看的,0表示正常结束，非0表示异常
	//而单片机上又没有操作系统，你return给谁看呢
}
