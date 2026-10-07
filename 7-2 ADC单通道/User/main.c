#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "AD.h"

uint16_t ADValue;
float Voltage;

int main(void)
{
	OLED_Init();
	AD_Init();

	OLED_ShowString(1 , 1 , "ADValue:");
	OLED_ShowString(2 , 1 , "Voltage: .   V");
	
	while(1)
	{
		ADValue = Get_ConvResult(ADC1);
		Voltage = Get_Voltage(ADC1);
		
		OLED_ShowNum(1, 9, ADValue, 5);
		OLED_ShowNum(2, 9, (uint16_t)Voltage, 1);
		OLED_ShowNum(2, 11, (uint16_t)(ADValue * 1000) % 1000, 3); //只有整数才能用%取余 
	}
}

