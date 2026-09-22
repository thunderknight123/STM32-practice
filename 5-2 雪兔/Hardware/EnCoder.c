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

volatile int16_t count = (int16_t)0;

void PB01_Init(void) //我把旋转编码器的A口接到了PB1，B口接到了PB0
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE); //开启GPIOB的时钟
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //配置为上拉输入
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOB , &GPIO_InitStructure);
}

void AFIO_Config(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO , ENABLE); // !!!!!除了EXTI,NVIC外的外设要使用都要首先开启时钟！！！！！
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB , GPIO_PinSource0);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB , GPIO_PinSource1);//选择PB0,PB1为中断引脚
}

void EXTI_Config(void)
{
	EXTI_InitTypeDef EXTI_Initstructure;
	EXTI_Initstructure.EXTI_Line = EXTI_Line0 | EXTI_Line1; //这里要PB5所在的第五个线路，所以选EXTI_Line5
	EXTI_Initstructure.EXTI_LineCmd = ENABLE;  //这个指的是指定选择的中断线的新状态ENABLE/DISABLE
	EXTI_Initstructure.EXTI_Mode = EXTI_Mode_Interrupt;  //这个就是选择出发响应方式：中断响应/事件响应
	EXTI_Initstructure.EXTI_Trigger = EXTI_Trigger_Rising;  //trigger的意思是触发，代表触发方式（1.上升沿 2.下降沿 3.双边沿）
	
	EXTI_Init(&EXTI_Initstructure);
}

void NVIC_Config(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //优先级分组
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn; //选择中断通道 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2; //由于我前面设置了NVIC_PriorityGroup_2，因此Pre与Sub的取值都是0~3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2; //值越小优先级越高
	
	NVIC_Init(&NVIC_InitStructure); // !!!!!给结构体赋之后一定要 Init 才能真正写入寄存器上生效!!!!!
	
	/*===========================================================================================================*/
	//刚刚配置了EXTI0中断通道，由于我们0，1都会用，所以还要配置EXTI1，同时由于有了两个中断通道，优先级要谨慎一点了
	/*===========================================================================================================*/
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn; 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	
	NVIC_Init(&NVIC_InitStructure);// !!!!!别忘!!!!!
	
}

int16_t Count_Get(void) //我这里返回变化量，这样可以直接+=操作一个变量，再把这个变量显示到OLED上即可
{
	int16_t Temp = count; //用一个临时变量保存count的值（count原本为0），再把count置0，返回的Temp就是count的变化量
	count = 0;
	return Temp;
}

void EXTI0_IRQHandler(void) //EXTI0的中断函数（B上升沿,A低电平时触发）（反转）
{
	if (EXTI_GetITStatus(EXTI_Line0) == SET && (GPIO_ReadInputDataBit(GPIOB , GPIO_Pin_1) == (uint8_t)Bit_RESET))  //看一看EXTI_Line0的中断标志位是不是为1，确保是EXTI_Line0才进中断
	{
		count--;
	}
	EXTI_ClearITPendingBit(EXTI_Line0);  //中断程序结束后一定要调用一下清除中断标志位的函数，否则程序会一直卡在中断里出不去（这个清除中断标志位的库函数一点要放在if外面，否则if判断为非时同样会卡住）
}

void EXTI1_IRQHandler(void) //EXTI1的中断函数（A上升沿，B低电平时触发）（正转）
{
	if (EXTI_GetITStatus(EXTI_Line1) == SET && (GPIO_ReadInputDataBit(GPIOB , GPIO_Pin_0) == (uint8_t)Bit_RESET))
	{
		count++;
	}
	EXTI_ClearITPendingBit(EXTI_Line1);
}



