
#include "stm8s_uart1.h"
#include <stdio.h>

#ifndef _usart_h
#define _usart_h


/******************************************************************************
                               外部调用功能函数
******************************************************************************/

void USART_Config(void);
void send_uart_data(uint8_t data);
/*
  功能：将int型数据转为2，8，10，16进制字符串
  参数：value --- 输入的int整型数
        str --- 存储转换的字符串
        radix --- 进制类型选择
  注意：8位单片机int字节只占2个字节
*/
char *sky_itoa(int value, char *str, unsigned int radix);
/*
  功能：将double型数据转为字符串
  参数：value --- 输入的double浮点数
        str --- 存储转换的字符串
        eps --- 保留小数位选择，至少保留一个小数位,至多保留4个小数位
  注意：8位单片机int字节只占2个字节
*/
void sky_ftoa(double value, char *str, unsigned int eps);

void mprintf(char *Data, ...);


#endif
