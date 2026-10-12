#include "stm32f10x.h"                   // Device header

uint16_t AD_Value[4] = {0}; //ADC的结果是uint16_t，所以转运目的地的数组元素也要是uint16_t

void MyDMA_Init(void)
{
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE); //注意DMA接在AHB总线
	
	DMA_InitTypeDef DMA_InitStructure;
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&ADC1->DR; //“外设”基地址（填uint32_t的数，不要直接填地址，要强转）
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; //数据宽度（字节uint8_t，半字uint16_t，字uint32_t）
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; //这个填的寄存器的地址，故绝对不能自增，寄存器的地址是固定的
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)AD_Value;              
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal; //设置“传输计数器”的“自动重装器”（Circular“循环”就是自动重装）
	DMA_InitStructure.DMA_BufferSize = 4; //即“传输计数器”初始值
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC; //设置传输方向
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable; //Memory-to-Memory置Disable就是设置成硬件触发（让ADC来触发）
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium; //设置优先级
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);
	
	DMA_Cmd(DMA1_Channel1, ENABLE); //硬件有对应的DMA1通道，ADC对应DMA1的正好就是Channel1（通道1）
}

