#ifndef __Data_Analysis_H
#define __Data_Analysis_H

#include "stm32f1xx_hal.h"

extern uint8_t Location;
extern uint8_t bbb;
extern uint8_t Interface[99]; //8���ӿ�
extern uint8_t Set_LED_Num ;
extern uint8_t KEY;
extern uint8_t key_up;     //�����ɿ���־
extern uint16_t Long_key_up;
extern uint16_t Shutdown;  //�Զ��ػ���־
extern uint8_t key_running;

/*******************************************/
extern uint16_t Loop_number_Times[5]; //��n��ѭ������
extern uint8_t Loop_number_Start[5]; //��n��ѭ����ʼλ��
extern uint8_t Loop_number_Stop[5];  //��n��ѭ������λ��
extern uint16_t Loop_Times;
extern uint8_t Loop_Start,Loop_Stop;//ѭ������/��ʼλ��/����λ��
extern uint8_t Sort_Order;            //�ڼ���ѭ����
extern uint8_t Order_Residue;        //ʣ�༸��ѭ����
extern uint8_t Order[4];
/****************������ʾ***********************/
extern uint8_t Programming_Error;
extern uint8_t Programming_Warning;
extern uint8_t LED_Glint;
extern uint8_t Light_times;
/******************ȷ��ģ��˳��***********************/
void Interface_module(void);
void Gyro_Module(void);
void Module_Init(void);
uint8_t KEY_Scan(void);

uint8_t emoji_Module(uint8_t i);
uint8_t voice_Module(uint8_t i);
uint8_t other_Module(uint8_t i);

void Turn_OFF_LED(uint8_t i);

uint8_t Start_Motor_Potentiometer(uint8_t i);
uint8_t Start_Potentiometer(uint8_t i);
uint8_t Tilt_Movement(uint8_t i);
uint8_t straight_Movement(uint8_t i);
uint8_t Right_Movement(uint8_t i);
uint8_t Left_Movement(uint8_t i);
uint8_t waitting (uint8_t i); //�ȴ�ģ��

void Special_Combination_Module(void);

void Boot_detection(void);
void send_data_time(void);
//void LED_Flashing(uint8_t i);
void LED_Flashing(uint8_t i,uint8_t j);

uint8_t Loop_one(uint16_t Loop_Times,uint8_t Loop_Start,uint8_t Loop_Stop);   //һ��������ѭ��
void Loop_Movement(void);//��Ҫѭ������/ѭ������/��ʼλ��/����λ��
void Circular_Order(void);   //ѭ��˳��


void Auto_OFF(void);    //�Զ��ػ�

/*********�����ж�************/
void Programming_Determine(void); //����ж�
void Error_hint(void);


#endif





































