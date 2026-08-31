#ifndef __LED_H
#define __LED_H

//头文件里不#include "stm32f10x.h" 也没关系，.h只是起到一个声明作用
//但是！！！每个.c文件是独立编译的！因此.c文件中必须#include "stm32f10x.h"，否则编译错误

//配置GPIO口的函数
void LED_Init_PAx(uint16_t x);

//点亮PAx处引脚的LED的函数（LED是低电平点亮）
void PAx_LED_ON(uint16_t x); //x传的是GPIO_Pin_x

//熄灭PAx处引脚的LED的函数（LED是高电平熄灭）
void PAx_LED_OFF(uint16_t x);

#endif
