#include "stm32f10x.h"                  // Device header


/*==============================================================================*/
//此处创建配置中断的函数
/*==============================================================================*/

/*==============================================================================*/
/*配置外部中断的步骤：

	1.激活引脚时钟（EXTI，NVIC的时钟默认开启）
	2.配置AFIO外设，AFIO选择中断引脚（用GPIO_EXTILineConfig）
	3.用EXTI的库函数配置EXTI并EXTI_Init（步骤类似于GPIO_Init）
	4.用NVIC的库函数设置优先级分组（这个在整个工程里只用设置一次）（pre == 先占优先级 == 抢占优先级）（sub == 从占优先级 == 响应优先级）
	  同样的，设置分组后，NVIC也有NVIC_Init
	  NVIC_InitStructure中有一个成员可能ctrl + f搜不到，需要扩大搜索范围从当前文件到当前工程，搜完之后，在里面找到MD的即可（因为我们的芯片是中等密度的）
	  每一个成员有啥用自己去看定义
	5.搞完之后整个外部中断的配置也就结束了
	  
*/
/*==============================================================================*/

volatile uint32_t count = (uint32_t)0;

void PB5_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE); //开启GPIOB的时钟
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //配置为上拉输入
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOB , &GPIO_InitStructure);
}

void AFIO_Config(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO , ENABLE); // !!!!!除了EXTI,NVIC外的外设要使用都要首先开启时钟！！！！！
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB , GPIO_PinSource5); //选择PB5为中断引脚
}

void EXTI_Config(void)
{
	EXTI_InitTypeDef EXTI_Initstructure;
	EXTI_Initstructure.EXTI_Line = EXTI_Line5; //这里要PB5所在的第五个线路，所以选EXTI_Line5
	EXTI_Initstructure.EXTI_LineCmd = ENABLE;  //这个指的是指定选择的中断线的新状态ENABLE/DISABLE
	EXTI_Initstructure.EXTI_Mode = EXTI_Mode_Interrupt;  //这个就是选择出发响应方式：中断响应/事件响应
	EXTI_Initstructure.EXTI_Trigger = EXTI_Trigger_Falling;  //trigger的意思是触发，代表触发方式（1.上升沿 2.下降沿 3.双边沿）
	
	EXTI_Init(&EXTI_Initstructure);
}

void NVIC_Config(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //优先级分组
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn; //选择中断通道（stm32的EXTI10~EXTI15都合并在了这个中断通道里）（？？？但是不懂为什么要用这个？？？）
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; //由于我前面设置了NVIC_PriorityGroup_2，因此Pre与Sub的取值都是0~3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	
	NVIC_Init(&NVIC_InitStructure); // !!!!!给结构体赋之后一定要 Init 才能真正写入寄存器上生效!!!!!
}

uint32_t Count_Get(void)
{
	return count;
}

void EXTI9_5_IRQHandler(void)
{
	if (EXTI_GetITStatus(EXTI_Line5) == SET)  //看一看EXTI_Line5的中断标志位是不是为1，确保是EXTI_Line5才进中断
	{
		count++;
		EXTI_ClearITPendingBit(EXTI_Line5);  //中断程序结束后一定要调用一下清除中断标志位的函数，否则程序会一直卡在中断里出不去
	}
}


