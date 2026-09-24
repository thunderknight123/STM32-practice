#include "stm32f10x.h"                  // Device header

void Motor_Init(void)
{
	/*=============================================================*/
	//先搞定PWM波形的输出（注意：我把电机驱动模块的PWMA接到了PA2）
	/*=============================================================*/
	
	  //a. 打开PA2，PA4，PA5的时钟，并调为复用推挽输出（4，5分别接到了AIN1，AIN2）
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	  //b. 配置时基单元（让分辨率为10%，频率为500Hz）
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //第一步肯定是先开启外设时钟
	
	TIM_InternalClockConfig(TIM2); //这一步可以省略，因为默认时钟源就是内部时钟
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = 10 - 1; //ARR自动重装值（实际计算的是ARR + 1）
	TIM_TimeBaseInitStructure.TIM_Prescaler = 7200*2 - 1; //PSC预分频系数（实际计算的是PSC + 1）
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0x00; //这是高级定时器的成员，现在不用管，但先给初始化了
	
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);
	
	  //c. 配置输出比较单元 OC3 （因为我把PWMA接到了PA2，而PA2是TIM_CH3的复用引脚,即OC3，因此配置OC3）
	TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_OCStructInit(&TIM_OCInitStructure); //有一些成员是用不到的，但不初始化它们怕出意外，所以在赋值前先用这个库函数给统一初始化了
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //High：有效电平为高电平
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 0; //即CCR（Capture Compare Register）
	
	TIM_OC3Init(TIM2, &TIM_OCInitStructure);
	
	TIM_OC3PreloadConfig(TIM2,TIM_OCPreload_Enable); //开启CCR3的预装载寄存器
	
	TIM_Cmd(TIM2, ENABLE); //最后开启定时器TIM2
}

void RotationDirection(int16_t SPEED) //这里SPEED可以为负数，因此用有符号的int16_t
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	/*=============================================================*/
	//让电机驱动模块能根据旋转编码器的计次情况COUNT控制电机的正反转
	/*=============================================================*/
	
	  //a. “停转”：PA4 RESET，PA5 RESET（或者都SET也是停转）
	if (SPEED == 0)
	{
		GPIO_ResetBits(GPIOA, GPIO_Pin_4);
		GPIO_ResetBits(GPIOA, GPIO_Pin_5);
	}
	  //b. “正转”：PA4 SET，PA5 RESET
	else if (SPEED < 0) 
	{
		GPIO_SetBits(GPIOA, GPIO_Pin_4);
		GPIO_ResetBits(GPIOA, GPIO_Pin_5);
		TIM_SetCompare3(TIM2, -SPEED); //这里必须填>=0的数，因此是-SPEED
	}
	  //c. “反转”：PA4 RESET，PA5 SET
	else if (SPEED > 0)
	{
		GPIO_ResetBits(GPIOA, GPIO_Pin_4);
		GPIO_SetBits(GPIOA, GPIO_Pin_5);
		TIM_SetCompare3(TIM2, SPEED);
	}

}
	
	

