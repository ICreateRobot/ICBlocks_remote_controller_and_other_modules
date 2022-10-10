#ifndef __Bluetooth_H
#define __Bluetooth_H
#include "stm32f1xx_hal.h"

extern unsigned char Sendbuf;
extern unsigned char Sendbuff[1];
extern unsigned char Sendbuff1[1];
void Send_Cmd(char *cmd);

void Send_String(char *string);
//void Send_String(void); 

//void Send_Data(void);
void Send_Data(uint8_t *data);


void Receive_Data(void);
void DMA_Usart_Send(uint8_t *buf,uint8_t len);//串口发送封装

//extern unsigned char Sendbuff[]; //定义数据发送数组








#endif


