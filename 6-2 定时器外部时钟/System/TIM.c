#include "stm32f10x.h"                  // Device header

uint32_t Num = 0;

void TIM_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //干啥首先要开启外设时钟
	
	TIM_ETRClockMode2Config(TIM2, TIM_ExtTRGPSC_OFF, TIM_ExtTRGPolarity_NonInverted, 0x00); //这里不用内部时钟的转而用外部时钟
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1; //这个分频是作用于输入时钟的，因为时钟进入预分频器之前需要先交由一个装置检查波形是否稳定，检查方式为：在几个不同时间点检查电平，如果电平相同说明稳定；这几个时间点的间隔是默认的，但是可以通过这个成员来改变（分频）
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up; //这个设置的是计数模式：1.边沿对齐模式（计数器只朝一个方向数，上：0 ~ MAX，到达MAX之后计数器置0/下 MAX ~ 0，到0之后计数器回到MAX0）2.中心对齐模式
	TIM_TimeBaseInitStruct.TIM_Period = 10 - 1; //设置ARR的自动重装值
	TIM_TimeBaseInitStruct.TIM_Prescaler = 1 - 1; //设置分频系数（不过实际分频系数是PSC + 1）
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0x00;//高级定时器（TIM1）才有，这里不要它就把他初始化成0
	
	//中断时间间隔/周期 == 1 / (72MHz / PSC + 1 / ARR + 1) , 通过这个公式可以看出我以上的设置会使中断周期为1s
	
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStruct); //Init之后才算是写入寄存器生效了
	
	/*!!!!!Init之后会自动触发一个更新事件，导致中断标志位SET，使得中断一开始就触发，因此下一步要Claer中断标志位!!!!!*/
	
	TIM_ClearFlag(TIM2, TIM_FLAG_Update);
	
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE); //这个是用来使能/禁用一个定时器（TIM）的中断源的
	
	/*========================================*/
	/*          然后就开始对接NVIC            */
	/*========================================*/
	
	//1.首先对优先级分组
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	//2.创建并初始化结构体变量
	NVIC_InitTypeDef NVIC_InitStruct;
	NVIC_InitStruct.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 2;
	
	NVIC_Init(&NVIC_InitStruct); //!!!!!要Init了才真正写入寄存器
	
	/*========================================*/
	/*           最后启动定时器               */
	/*========================================*/
	
	TIM_Cmd(TIM2, ENABLE); //最后启动，防止没配置完中途启动发生错误
	
}

uint32_t Get_Num(void)
{
	return Num;
}

void TIM2_IRQHandler(void)
{
	/*!!!!!中断函数最后一定要清除中断标志位，否则会不断重复进入中断!!!!!*/
	TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	Num++;
}


void PA0_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz; //输入模式下Speed实际上没用
	
	GPIO_Init(GPIOA, &GPIO_InitStruct); //!!!!!要记得这一步才算是真正写入寄存器生效
}
