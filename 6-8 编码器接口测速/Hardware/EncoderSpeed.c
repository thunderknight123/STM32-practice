#include "stm32f10x.h"                  // Device header

void EncoderSpeed_Init(void)
{
	//一开始把要开启的时钟先都给开了
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //上拉还是下拉看外部默认低电平还是高电平，与外部保持一致即可，不要打架
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	//配置时基单元
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = 65536 - 1; //ARR
	TIM_TimeBaseInitStructure.TIM_Prescaler = 1 - 1; //PSC
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0x00;
	
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure);
	
	//部分配置IC成员
	TIM_ICInitTypeDef TIM_ICInitStructure;
	TIM_ICStructInit(&TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1; //这个Channl不能跟GPIO_Pin一样用 '|' 一起选
	TIM_ICInitStructure.TIM_ICFilter = 0xF;
	TIM_ICInit(TIM3, &TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;
	TIM_ICInitStructure.TIM_ICFilter = 0xF;
	TIM_ICInit(TIM3, &TIM_ICInitStructure);
	
	//配置编码器接口的函数（这里面帮你配置了IC成员的TIM_ICPolarity）
	TIM_EncoderInterfaceConfig(TIM3, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
	
	//最后不要忘记
	TIM_Cmd(TIM3, ENABLE);
	
}


