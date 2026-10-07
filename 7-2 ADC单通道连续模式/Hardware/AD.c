#include "stm32f10x.h"                  // Device header

void AD_Init(void)
{
	//开启所有要开的外设的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	
	RCC_ADCCLKConfig(RCC_PCLK2_Div6); //配置ADC预分频器（最大14Mhz，时钟源是内部时钟72MHz）
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN; //模拟输入模式
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 1, ADC_SampleTime_55Cycles5); //规则通道配置
	
	ADC_InitTypeDef ADC_InitStructure;
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE; //打开“连续转换模式”
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; //选右对齐读出来的就直接是转换结果
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; //选择触发ADC规则通道启动转换的外部事件源，None就是不适用外部触发，而用软件触发
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent; //用于配置 ADC 的工作模式，用来决定各个ADC模块是独立工作还是协同（同步，交替，混合）
	ADC_InitStructure.ADC_NbrOfChannel = 1; //指定你要转换的规则通道数量，>1还需打开扫描模式才行，否则与设置成1没区别
	ADC_InitStructure.ADC_ScanConvMode = DISABLE; //是否打开“扫描模式”
	ADC_Init(ADC1, &ADC_InitStructure);
	
	ADC_Cmd(ADC1, ENABLE); //上电
	
	//上电之后执行一次校准（先复位校准RSTCAL，再A/D校准CAL）
	ADC_ResetCalibration(ADC1); //RSTCAL
	while(ADC_GetResetCalibrationStatus(ADC1) == SET); //完成校准后硬件会将校准标志位自动清零
	ADC_StartCalibration(ADC1); //CAL
	while(ADC_GetCalibrationStatus(ADC1) == SET);

	ADC_SoftwareStartConvCmd(ADC1, ENABLE); //打开软件触发
}

uint16_t Get_ConvResult(ADC_TypeDef* ADCx)
{
	return ADC_GetConversionValue(ADCx); //ADC_GetConversionValue这个函数是专门用来读规则组的
}

float Get_Voltage(ADC_TypeDef* ADCx)
{
	uint16_t Temp = ADC_GetConversionValue(ADCx);
	return (float)Temp / 4095 * 3.3; //这里因为一些原因一定会有误差，不能达到3.3
}

