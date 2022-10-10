#ifndef __Bluetooth_H
#define __Bluetooth_H
#include "stm32f1xx_hal.h"

extern unsigned char Sendbuf;
extern unsigned char Sendbuff[1];
extern unsigned char Sendbuff1[1];

/************/
extern UART_HandleTypeDef huart1; //UART���
extern uint8_t usart1_rx_buffer[18]; //����1��DMA���ջ�����
extern uint8_t usart1_tx_buffer[18]; //����1��DMA���ͻ�����
extern uint8_t usart1_rx_flag; //DMA���ճɹ���־ 0��δ���յ�/1,���յ��ȴ�����
extern uint16_t usart1_rx_len; //DMAһ�ο����жϽ��յ������ݳ���
extern uint8_t receive_data[18];	  //DMA�������ݻ�����

void HAL_UART_ReceiveIdle(UART_HandleTypeDef *huart);		//���ڿ����жϴ�������
void UART1_TX_DMA_Send(uint8_t *buffer, uint16_t length);				//����HAL_UART_Transmit_DMA�������д��ڷ���
void Debug_printf(const char *format, ...);					//����UART1_TX_DMA_Sendʵ�ָ�ʽ�����


void Send_Cmd(char *cmd);

void Send_String(char *string);
//void Send_String(void); 

//void Send_Data(void);
void Send_Data(uint8_t *data);


void Receive_Data(void);
void DMA_Usart_Send(uint8_t *buf,uint8_t len);//���ڷ��ͷ�װ

//extern unsigned char Sendbuff[]; //�������ݷ�������








#endif


