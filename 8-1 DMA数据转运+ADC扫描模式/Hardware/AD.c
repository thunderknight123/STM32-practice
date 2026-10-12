#include "stm32f10x.h"                  // Device header


void AD_Init(void)
{
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	//配置GPIO口，模拟输入模式
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //输入模式的话这个成员没用了，不过尽量也要初始化了
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	//*配置ADC的预分频器,由于与总线有关，所以相关函数在rcc.h里（选6分频\8分频，最高14MHz）
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);
	
	//规则组通道配置：Rank是给扫描模式用来排序的 1~16 ；SampleTime是采样时间，时间越长数据越准
	ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 2, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 3, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_3, 4, ADC_SampleTime_55Cycles5);
	
	ADC_InitTypeDef ADC_InitStructure;
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent; //让各个ADC模块独立工作
	ADC_InitStructure.ADC_NbrOfChannel = 4;
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;
	ADC_Init(ADC1, &ADC_InitStructure);
	
	ADC_Cmd(ADC1, ENABLE);
	
	/*想要ADC转换数据的时候触发DMA还需：开启ADC到DMA的输出（而且必须跟在ADC_Cmd后面才有效）*/
	ADC_DMACmd(ADC1, ENABLE);
	
	//执行校准（先“复位”校准，再A\D校准）
	ADC_ResetCalibration(ADC1); //RSTCAL
	while (ADC_GetResetCalibrationStatus(ADC1) == SET); //校准之后，标志位会自动清零
	ADC_StartCalibration(ADC1); //CAL
	while (ADC_GetCalibrationStatus(ADC1) == SET);

}

void ConvStart(ADC_TypeDef* ADCx)
{
	//先重置DMA的计数器（重置要先关掉DMA）
	DMA_Cmd(DMA1_Channel1, DISABLE);
	DMA_SetCurrDataCounter(DMA1_Channel1, 4);
	DMA_Cmd(DMA1_Channel1, ENABLE);
	
	ADC_SoftwareStartConvCmd(ADCx, ENABLE);
	//转换完成需要时间 = SampleTime + 12.5个ADC Cycles ，通过检测EOC标志位状态确保转换完成
	//while (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET); //读完ADC_DR寄存器时自动清除EOC标志位,ADC_GetConversionValue这个函数就会读ADC_DR
	
	//等待转化，转运都完成（这里还利用了转运一定发生在转换后面）
	while (DMA_GetFlagStatus(DMA1_FLAG_TC1) == RESET);
	DMA_ClearFlag(DMA1_FLAG_TC1);
}

