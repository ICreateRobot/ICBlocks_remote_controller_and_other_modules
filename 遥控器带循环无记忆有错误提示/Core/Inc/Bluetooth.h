#ifndef __Bluetooth_H
#define __Bluetooth_H
#include "stm32f1xx_hal.h"

extern unsigned char Sendbuf;
extern unsigned char Sendbuff[1];
extern unsigned char Sendbuff1[1];

/************/
extern UART_HandleTypeDef huart1; //UART句柄
extern uint8_t usart1_rx_buffer[18]; //串口1的DMA接收缓冲区
extern uint8_t usart1_tx_buffer[18]; //串口1的DMA发送缓冲区
extern uint8_t usart1_rx_flag; //DMA接收成功标志 0，未接收到/1,接收到等待处理
extern uint16_t usart1_rx_len; //DMA一次空闲中断接收到的数据长度
extern uint8_t receive_data[18];	  //DMA接收数据缓存区

void HAL_UART_ReceiveIdle(UART_HandleTypeDef *huart);		//串口空闲中断处处理函数
void UART1_TX_DMA_Send(uint8_t *buffer, uint16_t length);				//调用HAL_UART_Transmit_DMA函数进行串口发送
void Debug_printf(const char *format, ...);					//调用UART1_TX_DMA_Send实现格式化输出


void Send_Cmd(char *cmd);

void Send_String(char *string);
//void Send_String(void); 

//void Send_Data(void);
void Send_Data(uint8_t *data);


void Receive_Data(void);
void DMA_Usart_Send(uint8_t *buf,uint8_t len);//串口发送封装

//extern unsigned char Sendbuff[]; //定义数据发送数组








#endif


