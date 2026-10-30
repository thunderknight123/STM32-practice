#include "stm32f10x.h"                  // Device header
#include "Delay.h"

//I2C的时序逻辑其实很巧妙：SCL高电平时，SDA的动作决定了是 读/写 还是 开始/停止


/*  三个最基本的 "写CLK" "写Data" "读Data" 每次读写一个bit   */
/*===========================================================*/

void MyI2C_W_SCL(uint8_t BitValue) 
{
	GPIO_WriteBit(GPIOB, GPIO_Pin_10, (BitAction)BitValue);
	Delay_us(10); //可以慢不能快
}

void MyI2C_W_SDA(uint8_t BitValue)
{
	GPIO_WriteBit(GPIOB, GPIO_Pin_11, (BitAction)BitValue);
	Delay_us(10);
}

uint8_t MyI2C_R_SDA(void)
{
	uint8_t BitValue;
	BitValue = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11);
	Delay_us(10);
	return BitValue;
}

/*===========================================================*/


//初始化引脚，设置为开漏输出（Set时浮空， Reset时低电平）
void MyI2C_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_SetBits(GPIOB, GPIO_Pin_10 | GPIO_Pin_11);
}


//I2C中，“先SDA先0， SCL后0”表示开始
void MyI2C_Start(void)
{
	MyI2C_W_SDA(1); //最好是先释放SDA
	MyI2C_W_SCL(1);
	MyI2C_W_SDA(0);
	MyI2C_W_SCL(0);
}


//“SCL先1，SDA后1”是停止
void MyI2C_Stop(void)
{
	MyI2C_W_SDA(0); //如果SDA上升之前，SCL已经是1了，那省事了，不需要把它拉低再释放了，所以这里就没有MyI2C_W_SDA(0)
	MyI2C_W_SCL(1);
	MyI2C_W_SDA(1);
}


//发送一个byte（重复8遍写SDA）
void MyI2C_SendByte(uint8_t Byte)
{
	uint8_t i;
	for (i = 0; i < 8; i ++)
	{
		MyI2C_W_SDA(!!(Byte & (0x80 >> i))); //这个 !! 双重逻辑取反是防御性写法，可以把>0的任何数变成1（因为逻辑取反的结果只有0/1）
		MyI2C_W_SCL(1); //释放SCL，SCL高电平的时候从机才读取
		MyI2C_W_SCL(0);
	}
}


//接收一个byte（重复8遍读SDA）
uint8_t MyI2C_ReceiveByte(void)
{
	uint8_t i, Byte = 0x00;
	MyI2C_W_SDA(1);
	for (i = 0; i < 8; i ++)
	{
		MyI2C_W_SCL(1);
		if (MyI2C_R_SDA()){Byte |= (0x80 >> i);}
		MyI2C_W_SCL(0);
	}
	return Byte;
}


//发送应答:表示主机对从机发送过来的Data的应答，此应答用于告诉从机要不要继续发送（发0表示应答Ack，“发”1表示无应答NAck）
void MyI2C_SendAck(uint8_t AckBit)
{
	MyI2C_W_SDA(AckBit);
	MyI2C_W_SCL(1);
	MyI2C_W_SCL(0);
}


//接收应答：用于主机问从机有没有收到，从机给的应答（0为Ack，1为NAck）
uint8_t MyI2C_ReceiveAck(void)
{
	uint8_t AckBit;
	MyI2C_W_SDA(1);
	MyI2C_W_SCL(1);
	AckBit = MyI2C_R_SDA();
	MyI2C_W_SCL(0);
	return AckBit;
}
