#include "stm32f10x.h"				//Device Header
#include "Delay.h"
#include <math.h>
#define M_PI 3.14159265358979323846

//初始化PWM引脚和定时器
void PWM_Init(void)
{
	//1. 将TIM2挂在APB1总线上
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , ENABLE);
	
	//2. 配置TIM2的时基
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_TimeBaseStructure.TIM_Period = 999;
	TIM_TimeBaseStructure.TIM_Prescaler = 71; //72MHz / （71 + 1） = 1 MHz = 1 us = 1e-6 s
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //写等于0也是一样的
		//TIM_ClockDivision是时钟分频因子，影响数字滤波器的采样频率，0代表采样频率=定时器时钟频率
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //设置成向上计数
		//Up模式下:从0数到999（Down则从999数到0）
	TIM_TimeBaseInit(TIM2 , &TIM_TimeBaseStructure);
	
	//3. 配置TIM2的PWM模式
	TIM_OCInitTypeDef TIM_OCInitstructure = {0}; //因为我们只部分初始化，所以先初始化所有成员为0，安全点
	TIM_OCInitstructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitstructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitstructure.TIM_Pulse = 0; //初始占空比为0（即熄灭状态）
	TIM_OCInitstructure.TIM_OCPolarity = TIM_OCPolarity_High; //高电平有效：当CNT < CCR时引脚输出高电平
		//CNT是计数器当前的值（因为前面不是规定了从0数到999 ， CNT就是这个）
	TIM_OC1Init(TIM2 , &TIM_OCInitstructure);
	
	//4. 开启TIM2的预装载寄存器
	TIM_OC1PreloadConfig(TIM2 , TIM_OCPreload_Enable);
	
	//5. 开启定时器
	TIM_Cmd(TIM2 , ENABLE);
}

 //volatile可以防止msTick被编译器优化（我把msTick放在"stm32f10x_it.c"中了）
extern volatile uint32_t msTick;

int main(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE); //用RCC使能GPIO的时钟
	
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP; //配置PA0为复用推挽输出（把Out改成AF）（呼吸灯时使用）
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0; //All就是同时弄他16个Pin
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOA , &GPIO_InitStruct); //初始化GPIO
	
	PWM_Init();
	
	SysTick_Config((uint32_t)(SystemCoreClock / 1000)); //STM32里有个“时钟”，每秒“滴答”72000000次（72MHz）
	                                         //那每“滴答”72000次是不是相当于经过了1ms
	                                       	 //所以参数给72000就是在告诉他“每1ms给我触发SysTick_Handler”
	
	while (1)
	{
		uint32_t current_time = msTick;
		
		float angle = (float)(current_time / 2000.0f) * (2.0f * M_PI); //angle应是弧度制的，因为sin传的参是弧度
			
		float x_times = (sin(angle) + 1.0f) / 2.0f;  //弄成0~1倍
			
		uint16_t new_CCR = (uint16_t)(x_times * 999.0f); //CCR -> Capture Compare1 Register value
		
		TIM_SetCompare1(TIM2 , new_CCR);
		
		for (volatile int i = 0; i < 1000; i++){;} //給CPU一个几us的短暂的延时，防止计算过快
	}
}
//Keil 5最后一点要空一行，否则会警告
