#include "stm32f10x.h"                  // Device header

void MyDMA_Init(uint32_t BaseAddr_P, uint32_t BaseAddr_M, uint32_t Size)
{
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE); //注意DMA接在AHB总线
	
	DMA_InitTypeDef DMA_InitStructure;
	DMA_InitStructure.DMA_PeripheralBaseAddr = BaseAddr_P; //“外设”基地址（填uint32_t的数，不要直接填地址，要强转）
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; //数据宽度（字节uint8_t，半字uint16_t，字uint32_t）
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Enable; //设置地址是否自增（自增的话就是“往后挪个坑位”，外设寄存器的地址是固定的，不可
	DMA_InitStructure.DMA_MemoryBaseAddr = BaseAddr_M;              //                  自增，自增就到了其他寄存器了，不过我这里填的是存储器的地址）
	DMA_InitStructure.DMA_MemoryDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal; //设置“传输计数器”的“自动重装器”（Circular“循环”就是自动重装）
	DMA_InitStructure.DMA_BufferSize = Size; //即“传输计数器”初始值
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC; //设置传输方向
	DMA_InitStructure.DMA_M2M = DMA_M2M_Enable; //Memory-to-Memory置Enable就是设置成软件触发（这个“软件触发”不需要额外调用函数，是自动的）
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium; //设置优先级
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);
	
	DMA_Cmd(DMA1_Channel1, ENABLE);
}

