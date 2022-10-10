
#include "stm8s_uart1.h"
#include <stdio.h>

#ifndef _usart_h
#define _usart_h


/******************************************************************************
                               �ⲿ���ù��ܺ���
******************************************************************************/

void USART_Config(void);
void send_uart_data(uint8_t data);
/*
  ���ܣ���int������תΪ2��8��10��16�����ַ���
  ������value --- �����int������
        str --- �洢ת�����ַ���
        radix --- ��������ѡ��
  ע�⣺8λ��Ƭ��int�ֽ�ֻռ2���ֽ�
*/
char *sky_itoa(int value, char *str, unsigned int radix);
/*
  ���ܣ���double������תΪ�ַ���
  ������value --- �����double������
        str --- �洢ת�����ַ���
        eps --- ����С��λѡ�����ٱ���һ��С��λ,���ౣ��4��С��λ
  ע�⣺8λ��Ƭ��int�ֽ�ֻռ2���ֽ�
*/
void sky_ftoa(double value, char *str, unsigned int eps);

void mprintf(char *Data, ...);


#endif
