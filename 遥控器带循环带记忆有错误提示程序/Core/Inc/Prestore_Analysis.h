#ifndef __Prestore_Analysis_H
#define __Prestore_Analysis_H

#include "stm32f1xx_hal.h"

extern uint8_t Prestore_Data[99]; //Ԥ������
extern uint8_t Prestore_Times;       //Ԥ�������ִ�
extern uint8_t Data_Flag;
extern uint8_t IORD_Prestore_Times;    //����ִ��8��ģ��ִ�е��ڼ���
extern uint8_t Clear_KEY;
extern uint8_t IORD_Sort_Order;            //�ڼ���ѭ����

void IOWR(void);  //Ԥ��ָ��ģ��д��
void IORD(uint8_t j);  //��ȡָ��
void IORD_Running(uint8_t j);  //��ȡָ������
void IORD_Analyse(uint8_t j);  //��ȡָ�����
uint8_t Loop_Number(void);  //ѭ������

uint8_t IOWR_Judge(void);  //��ȡָ���ж�
uint8_t Module_Performs(uint8_t i,uint8_t j);
uint8_t Module_Judge(void);
void Starting_Procedure_With_IOWR(void);   //�ж�ȡģ��ĳ���ִ��
uint8_t IORD_Loop_one(uint16_t Loop_Times,uint8_t Loop_Start,uint8_t Loop_Stop,uint8_t j);   //һ��������ѭ��
uint8_t Internal_Loop_Judgment(void);   //8��������Ƿ�����ѭ��ģ��
void Clear_Data(void); //�������
uint8_t Clear_KEY_DOWN(void); //�������


#endif


