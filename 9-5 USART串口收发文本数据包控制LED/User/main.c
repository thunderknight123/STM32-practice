#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Serial.h"
#include "LED.h"
#include <stdio.h>
#include <string.h>

int main(void)
{
	OLED_Init();
	Serial_Init();
	LED_Init();

	OLED_ShowString(1, 1, "React:");
	OLED_ShowString(3, 1, "Command:");
	while(1)
	{
		/*下面是接收*/
		if (Serial_GetRxFlag() == 1)
		{
			if (strcmp(Serial_RxPacket, "LED_ON") == 0) //当两个字符串相等时返回0
			{
				LED_ON();
				OLED_ShowString(4, 1, "                ");
				OLED_ShowString(2, 1, "LED_ON_OK");
				Serial_SendString("LED_ON_OK\r\n");
			}
			else if (strcmp(Serial_RxPacket, "LED_OFF") == 0)
			{
				LED_OFF();
				OLED_ShowString(4, 1, "                ");
				OLED_ShowString(2, 1, "LED_OFF_OK");
				Serial_SendString("LED_OFF_OK\r\n");
			}
			else 
			{
				OLED_ShowString(4, 1, "                ");
				OLED_ShowString(2, 1, "Wrong_Command");
				Serial_SendString("Wrong_Command\r\n");
			}
			OLED_ShowString(4, 1, "                "); //填写空格相当于删除
			OLED_ShowString(4, 1, Serial_RxPacket);
		}
	}
}

