#include "stm32f10x.h"                  // Device header


void IC_Init(void)
{
	/*======================*/
	/* 先由PA0输出一个PWM波 */
	/*======================*/
	//1.
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	//2.
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	
	//1000Hz
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseStructInit(&TIM_TimeBaseInitStructure); //先默认初始化一遍结构体，防止出错
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = 100 - 1; //ARR - 1（最大就是65536）
	TIM_TimeBaseInitStructure.TIM_Prescaler = 720 - 1; //PSC - 1
	
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);
	
	//3.
	TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_OCStructInit(&TIM_OCInitStructure);
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //有效电平为高电平
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 50; //CCR
	
	TIM_OC1Init(TIM2, &TIM_OCInitStructure);
	
	TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable); //开启CCR1的预装载（影子）寄存器，动态改变CCR必备
	
	TIM_Cmd(TIM2, ENABLE); //最后开启定时器TIM2
	
	
	/*======================*/
	/* 以下是输入捕获的关键 */
	/*======================*/
	
	//1.
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	//2.
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //要用TIM3就先开启TIM3的时钟
	TIM_InternalClockConfig(TIM3);
	
	//3.
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = 65536 - 1; //ARR - 1（最大就是65536）
	TIM_TimeBaseInitStructure.TIM_Prescaler = 72 - 1; //PSC - 1
	
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure);
	
	//4.
	TIM_ICInitTypeDef TIM_ICInitStructure;
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1; //PA1输出的PWM波在PA6被接收，而PA6是TIM3_CH1,所以选择TIM_Channel_1
	TIM_ICInitStructure.TIM_ICFilter = 0xF; //一般滤波器（Filter）的采样频率都远高于信号频率，因此滤波器不会改变信号频率；这里0xF配置的是参数，详见手册
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising; //上升沿触发
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1; //不分频
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;	//直接输入来自TIMx_CH1（CH1还是CH2取决于你上面的选了什么）的信号TI1
	TIM_PWMIConfig(TIM3, &TIM_ICInitStructure);
	
	//TIM_ICInit(TIM3, &TIM_ICInitStructure);  	有了TIM_PWMIConfig(TIM3, &TIM_ICInitStructure);就不需要TIM_ICInit了
	
	//5.
	TIM_SelectInputTrigger(TIM3, TIM_TS_TI1FP1); //从触发源选择TI1FP1
	
	//6.
	TIM_SelectSlaveMode(TIM3, TIM_SlaveMode_Reset); //从模式选择
	
	//7.
	TIM_Cmd(TIM3, ENABLE); //最后开启定时器TIM3
}

uint32_t IC_GetFreq(void)
{
	return 1000000 / TIM_GetCapture1(TIM3);
}

uint16_t IC_GetDuty(void)
{
	return (TIM_GetCapture2(TIM3) * 100) / TIM_GetCapture1(TIM3);
}
	