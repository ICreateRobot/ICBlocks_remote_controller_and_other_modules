#include <stdio.h>
#include "stm8s.h"
#include "stm8s_uart1.h"
#include "usart.h"
#include "stdarg.h"


/**
  *****************************************************************************
  * @Name   : 串口初始化
  *****************************************************************************
**/


void USART_Config(void)
{
  UART1_Init(115200, UART1_WORDLENGTH_8D, UART1_STOPBITS_1, UART1_PARITY_NO, UART1_SYNCMODE_CLOCK_DISABLE, UART1_MODE_TXRX_ENABLE);//初始化uart1
  UART1_ITConfig(UART1_IT_RXNE_OR, ENABLE);//中断配置，接收数据寄存器非空或溢出错误中断
  UART1_Cmd(ENABLE);//使能uart1
  
}
 
/*发送串口数据*/
void send_uart_data(uint8_t data)
{
  UART1_SendData8(data);
  while(UART1_GetFlagStatus(UART1_FLAG_TXE)==RESET);//等待发送完成

}
 
/*
  功能：将int型数据转为2，8，10，16进制字符串
  参数：value --- 输入的int整型数
        str --- 存储转换的字符串
        radix --- 进制类型选择
  注意：8位单片机int字节只占2个字节
*/
char *sky_itoa(int value, char *str, unsigned int radix)
{
  char list[] = "0123456789ABCDEF";
  unsigned int tmp_value;
  int i = 0, j, k = 0;
  if (NULL == str) {
    return NULL;
  }
  if (2 != radix && 8 != radix && 10 != radix && 16 != radix) {
    return NULL;
  }
  if (radix == 10 && value < 0) {
    //十进制且为负数
    tmp_value = (unsigned int)(0 - value);
    str[i++] = '-';
    k = 1;
  } else {
    tmp_value = (unsigned int)value;
  }
  //数据转换为字符串，逆序存储
  do {
    str[i ++] = list[tmp_value%radix];
    tmp_value /= radix;
  } while(tmp_value);
  str[i] = '\0';
  //将逆序字符串转换为正序
  char tmp;
  for (j = k; j < (i+k)/2; j++) {
    tmp = str[j];
    str[j] = str[i-j-1+k];
    str[i-j-1+k] = tmp;
  }
  return str;
}
 
/*
  功能：将double型数据转为字符串
  参数：value --- 输入的double浮点数
        str --- 存储转换的字符串
        eps --- 保留小数位选择，至少保留一个小数位,至多保留4个小数位
  注意：8位单片机int字节只占2个字节
*/
void sky_ftoa(double value, char *str, unsigned int eps)
{
  unsigned int integer;
  double decimal;
  char list[] = "0123456789";
  int i = 0, j, k = 0;
  //将整数及小数部分提取出来
  if (value < 0) {
    decimal = (double)(((int)value) - value);
    integer = (unsigned int)(0 - value);
    str[i ++] = '-';
    k = 1;
  } else {
    integer = (unsigned int)(value);
    decimal = (double)(value - integer);
  }
  //整数部分数据转换为字符串，逆序存储
  do {
    str[i ++] = list[integer%10];
    integer /= 10;
  } while(integer);
  str[i] = '\0';
  //将逆序字符串转换为正序
  char tmp;
  for (j = k; j < (i+k)/2; j++) {
    tmp = str[j];
    str[j] = str[i-j-1+k];
    str[i-j-1+k] = tmp;
  }
  //处理小数部分
  if (eps < 1 || eps > 4) {
    eps = 4;
  }
  
  //精度问题，防止输入1.2输出1.19等情况
  double pp = 0.1;
  for (j = 0; j <= eps; j++) {
    pp *= 0.1;
  }
  decimal += pp;
  while (eps) {
    decimal *= 10;
    eps --;
  }
  int tmp_decimal = (int)decimal;
  str[i ++] = '.';
  k = i;
  //整数部分数据转换为字符串，逆序存储
  do {
    str[i ++] = list[tmp_decimal%10];
    tmp_decimal /= 10;
  } while(tmp_decimal);
  str[i] = '\0';
  //将逆序字符串转换为正序
  for (j = k; j < (i+k)/2; j++) {
    tmp = str[j];
    str[j] = str[i-j-1+k];
    str[i-j-1+k] = tmp;
  }
  str[i] = '\0';
}
 
 
void mprintf(char *Data, ...)
{
  const char *s;
  int d;   
  char buf[16];
  uint8_t txdata;
  va_list ap;
  va_start(ap, Data);
  while ( * Data != 0 ) {				                          
    if ( * Data == 0x5c )  {									  
      switch ( *++Data ) {
        case 'r':							          
          txdata = 0x0d;
          send_uart_data(txdata);
          Data ++;
          break;
        case 'n':							          
          txdata = 0x0a;
          send_uart_data(txdata);
          Data ++;
          break;
        default:
          Data ++;
          break;
      }			 
    } else if ( * Data == '%') {									  
      switch ( *++Data ) {				
      case 's':										 
        s = va_arg(ap, const char *);
        for ( ; *s; s++) {
          send_uart_data(*((uint8_t *)s));
        }				
        Data++;				
        break;
      case 'd':			
        d = va_arg(ap, int);					
        sky_itoa(d, buf, 10);					
        for (s = buf; *s; s++) {
          send_uart_data(*((uint8_t *)s));
        }					
        Data++;				
        break;
      case 'x': {
        d = va_arg(ap, int);					
        sky_itoa(d, buf, 16);					
        for (s = buf; *s; s++) {
          send_uart_data(*((uint8_t *)s));
        }					
        Data++;			
        break;
      }
      
     /* 

      case 'f': {
        double num = va_arg(ap, double);					
        sky_ftoa(num, buf, 4);
        for (s = buf; *s; s++) {
          send_uart_data(*((uint8_t *)s));
        }					
        Data++;			
        break;
      }

      */
      
      
      
      default:
        Data++;				
        break;				
      }		 
    } else {
        send_uart_data(*((uint8_t *)Data));
        Data++;
    }
  }
}



