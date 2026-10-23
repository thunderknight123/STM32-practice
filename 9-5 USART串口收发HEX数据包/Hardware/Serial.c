#include "stm32f10x.h"            // Device header
#include <math.h>
#include <stdio.h>
#include <stdarg.h>               //这里提供与可变参数相关的宏

uint8_t Serial_TxPacket[4]; //这4个数据就完全是载荷数据，不包含包头包尾（包头包尾直接在函数里面顺便发送）
uint8_t Serial_RxPacket[4];

uint8_t Serial_RxFlag;


void Serial_Init(void)
{
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
	
	//配置发送数据Tx的GPIO口，配置成复用推挽输出
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	//配置接收数据Rx的GPIO口，配置成上拉输入/浮空输入（因为不接收的常态下是高电平，所以不能配置成下拉输入，会打架）
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	//配置USART结构体
	USART_InitTypeDef USART_InitStructure;            /*要与 比特率 区分开，只是一般它们在数值上相等*/
	USART_InitStructure.USART_BaudRate = 9600; //”波特率“ -> BandRate == PCLKx / (16 * DIV)	，DIV在波特率寄存器里，波特率9600刚好，好像是默认112600
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //”硬件流控制“ -> 防止因收发双方处理速度不匹配而导致数据丢失（速度实在太大就启用）
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx; //Rx是receive， Tx是transmit
	USART_InitStructure.USART_Parity = USART_Parity_No; //Parity n. 奇偶校验；相等
	USART_InitStructure.USART_StopBits = USART_StopBits_1; //停止位长度
	USART_InitStructure.USART_WordLength = USART_WordLength_8b; //发送的数据位长度（无校验位选8bits就好）
	USART_Init(USART3,&USART_InitStructure);
	
	
	//用RXNE这个标志位申请中断
	  
	  //先打开中断通道
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
	
	  //给优先级分组（分bits）
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	  //结构体
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_Init(&NVIC_InitStructure);
	
	USART_Cmd(USART3, ENABLE);
}

void Serial_SendByte(uint8_t Byte)
{
	USART_SendData(USART3, Byte); //第二个形参实际类型是uint16_t（由于一次发送8bits，所以如果接收模式HEX，每一次发送会显示2位，因为每4bits对应一个16进制位）
	while(USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET); //当数据从 发送寄存器TDR -> 发送移位寄存器 时会置标志位TXE，所以这里是在等待这一步骤完成（当再次写入TDR时会自动清除标志位）
}

void Serial_SendNumArray(uint8_t* arr, uint8_t Length)
{
	for(uint16_t i = 0; i < Length; i++)
	{
		Serial_SendByte(arr[i]); //具体接受到什么由接受模式决定
	}
}

void Serial_SendString(char* String)
{
	for(uint8_t i = 0; String[i] != '\0'; i++)
	{
		Serial_SendByte(String[i]);
	}
}


void Serial_SendNum(uint16_t Num) //要显示数字的话接受模式要用：文本模式
{
	uint8_t Length = 1;
	uint16_t Temp = Num;
	while((Temp = Temp / 10)> 0)
	{
		Length++;
	}
	for(uint8_t i = 0; i < Length; i++) //想得到第x位，则Num/POW(10,x-1)%10
	{
		Serial_SendByte(Num/(uint16_t)pow(10, Length-i-1) % 10 + '0'); //得到的数还要加上字符'0'
	}
}

void Serial_SendChar(char ch)
{
	Serial_SendByte(ch); //变量就不需要加’‘或者”“了（老是忘）
}





/*======================================================*/
//发数据包（直接发就完事了）

void Serial_SendPacket(void)
{
	Serial_SendByte(0xFF);
	Serial_SendNumArray(Serial_TxPacket, 4);
	Serial_SendByte(0xFE);
}
/*======================================================*/
  



  //中断函数（接收固定长度数据包）
void USART3_IRQHandler(void)
{
	static uint8_t RxStatus = 0; //静态变量，寿命很长，只会被初始化一次，但是只能在该函数内使用
	static uint8_t pRx = 0;
	
	if (USART_GetFlagStatus(USART3, USART_FLAG_RXNE) == SET)
	{
		uint8_t RxData = USART_ReceiveData(USART3);
		if (RxStatus == 0) //为0时等待包头
		{
			if (RxData == 0xFF)
			{
				RxStatus = 1;
				pRx = 0;
			}
		}
		else if (RxStatus == 1) //为1时接收载荷数据（我固定载荷长度是4 bits）
		{
			Serial_RxPacket[pRx] = RxData;
			pRx++;
			if (pRx >= 4)
			{
				RxStatus = 2;
			}
		}
		else if (RxStatus == 2) //为2时等待包尾
		{
			if (RxData == 0xFE)
			{
				RxStatus = 0;
				Serial_RxFlag = 1;
			}
		}
	}
	USART_ClearFlag(USART3, USART_FLAG_RXNE); //如果读了DR就不用这一步
}


  //返回数据包是否完全接收
uint8_t Serial_GetRxFlag(void)
{
	if (Serial_RxFlag == 1)
	{
		Serial_RxFlag = 0;
		return 1;
	}
	return 0;
}

/*==================================================*/
/*                对printf进行重定向                */
/*==================================================*/

  //首先，printf的实现就是依托于fputc，所以要对fputc进行重定义
  //为什么可以重定义？————因为stdio.h中的fputc是[弱定义__weak]的，所以对于同名函数，编译器会优先选择[强定义]

int fputc(int ch, FILE* f) 
{
	(void)f; //这里的FILE*类型的指针实际上用不到了，所以直接强转为void，之所以形参不能变是因为就算内容用不上但签名必须保持一致，否则会链接错误
	Serial_SendByte(ch);
	return ch;
}


  //刚刚重定义fputc之后其实printf就重定向成功了，然后我们封装了一个“printf”，使它也能实现发送功能，以下内容涉及“可变参数”的内容，有点超纲，课后好好再学
void Serial_printf(char* format, ...)
{
	//以下va_list, va_start(arg, format), va_end等等都是<stdarg.h>里定义的宏
	char String[100];
	va_list arg;                    //va_list -> 定义指向arg的指针类型
	va_start(arg, format);          //让指针arg指向第一个可变参数（format实际是最后Lsat个固定参数）
	vsprintf(String, format, arg);  //作用：把格式化内容写入一个字符数组（不用sprintf是因为它只能接受直接写的参数，而对于这种封装的用vsprintf）
									//参数 1. 目标字符数组的地址 2. 格式化字符串 3. 参数表
	va_end(arg);                    //释放参数表（其实就是让这个指针“失效”， 而不是变成野指针）
	
	//最后不要忘了我们这个是个printf，是要发送数据的，然后printf就封装好了
	Serial_SendString(String);
}










