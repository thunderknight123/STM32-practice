#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Serial.h"
#include <stdio.h>

//需要在魔术棒C/C++里的Msc Controls里输入--no-multibyte-chars，才可以发送中文
char String[] = "abcdefg";

uint8_t RxData = 0; //接收到的一个字节数据放在Rxdata里面了

int main(void)
{
	OLED_Init();
	Serial_Init();
	//printf("kskbl %s\r\n", "zdjd");
	//Serial_printf("Num=%d\r\n", 666);
	
	
	while(1)
	{
	/*串口接收的方式一：查询*/
		
		//if(USART_GetFlagStatus(USART3, USART_FLAG_RXNE) == SET)
		//{
		//	RxData = USART_ReceiveData(USART3);                   //读完DR就不用清除标志位了，会自动清除
		//  OLED_ShowHexNum(1, 1, RxData, 2);
		//}
		
		
	/*串口接收的方式二：中断*/
		
		if (Serial_GetRxFlag() == 1)
		{
			RxData = Serial_GetRxData();
			OLED_ShowHexNum(1, 1, RxData, 2);
			Serial_printf("Num=%d\r\n", RxData);
		}
	}
}

