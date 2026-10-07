#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "MyDMA.h"

uint8_t DataA[] = {1, 2, 3, 4};
uint8_t DataB[4] = {0};

int main(void)
{
	OLED_Init();
	
	OLED_ShowString(1 , 1 , "DataA");
	OLED_ShowString(2 , 1 , "DataB");
	
	OLED_ShowString(3 , 1 , "DataA");
	OLED_ShowString(4 , 1 , "DataB");

		//打印数据转运前的DataA，DataB
		for (int i = 0; i <= 3; i++)
		{
			OLED_ShowHexNum(1, 6 + 3*i, DataA[i], 2);
		}
		
		for (int i = 0; i <= 3; i++)
		{
			OLED_ShowHexNum(2, 6 + 3*i, DataB[i], 2);
		}
		
		MyDMA_Init((uint32_t)DataA, (uint32_t)DataB, 4);
		
	while(1)
	{
		//打印数据转运后的DataA，DataB
		for (int i = 0; i <= 3; i++)
		{
			OLED_ShowHexNum(3, 6 + 3*i, DataA[i], 2);
		}
		
		for (int i = 0; i <= 3; i++)
		{
			OLED_ShowHexNum(4, 6 + 3*i, DataB[i], 2);
		}
		
	}
}

