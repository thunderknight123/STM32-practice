#ifndef __SERIAL_H
#define __SERIAL_H

void Serial_Init(void);
void Serial_SendByte(uint8_t Byte);
void Serial_SendNumArray(uint8_t* arr, uint8_t Length);
void Serial_SendString(char* String);
void Serial_SendNum(uint16_t Num);
void Serial_SendChar(char ch);
void Serial_printf(char* format, ...);
uint8_t Serial_GetRxData(void);
uint8_t Serial_GetRxFlag(void);

#endif
