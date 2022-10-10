#include "Bluetooth.h"
#include "main.h"
#include "stm32f1xx_hal.h"
#include "usart.h"
#include "string.h"
#include "stdio.h"


uint8_t Sendcmdbuff[255] ;  //定义数据发送数组
unsigned char Sendbuff[1]= {0}; //定义数据发送数组
unsigned char Sendbuff1[1]= {0}; //定义数据发送数组
//char Senbuff[5] = "sss";  //定义数据发送数组
unsigned char Sendbuf;


void Send_Cmd(char *cmd)
{
    uint16_t len;
    len=snprintf((char *)Sendcmdbuff,sizeof(Sendcmdbuff)-1,"%s\r\n",cmd);//"AT+BAUD"
    HAL_UART_Transmit_DMA(&huart1, (uint8_t *)Sendcmdbuff, len);

    HAL_Delay(100);
}
//void Send_String(uint8_t *Sendbuff)
void Send_String(char *string)
{
    HAL_UART_Transmit_DMA(&huart1, (uint8_t *)string, sizeof(string));
//    HAL_Delay(100);
}
void Send_Data(uint8_t *data)
{
    HAL_UART_Transmit_DMA(&huart1, (uint8_t *)data, 1);
//		while(__HAL_DMA_GET_FLAG(&UART1TxDMA_Handler,DMA_FLAG_TC4)){};
//    HAL_Delay(10);

}
//void Send_Data(void)
//{
//    HAL_UART_Transmit_DMA(&huart1, (uint8_t *)Sendbuf, 2);
//    HAL_Delay(100);
//}

void DMA_Usart_Send(uint8_t *buf,uint8_t len)//串口发送封装
{
    if(HAL_UART_Transmit_DMA(&huart1, buf,len)!= HAL_OK)
    {
        Error_Handler();
    }
}






void Receive_Data(void)
{

//    if(recv_end_flag == 1)  //接收完成标志
//    {

////			DMA_Usart_Send(rx_buffer, rx_len);
//        rx_len = 0;//清除计数
//        recv_end_flag = 0;//清除接收结束标志位

//        /*******用完数据之后清零*******/
////				memset(rx_buffer,0,sizeof(rx_buffer));
//    }


}



