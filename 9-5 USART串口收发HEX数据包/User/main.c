#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Serial.h"
#include <stdio.h>


int main(void)
{
	OLED_Init();
	Serial_Init();
	//printf("kskbl %s\r\n", "zdjd");
	//Serial_printf("Num=%d\r\n", 666);
	
//	/*发送*/
//	Serial_TxPacket[0] = 0x11;
//	Serial_TxPacket[1] = 0x22;
//	Serial_TxPacket[2] = 0x33;
//	Serial_TxPacket[3] = 0x44;
//	Serial_SendPacket();
	while(1)
	{
		
		/*下面是接收*/
		if (Serial_GetRxFlag() == 1)
		{
			OLED_ShowHexNum(1, 1, Serial_RxPacket[0], 2);
			OLED_ShowHexNum(1, 4, Serial_RxPacket[1], 2);
			OLED_ShowHexNum(1, 7, Serial_RxPacket[2], 2);
			OLED_ShowHexNum(1, 10, Serial_RxPacket[3], 2);
		}
	}
}

