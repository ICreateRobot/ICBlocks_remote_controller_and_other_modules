#ifndef __Data_Analysis_H
#define __Data_Analysis_H

#include "stm32f1xx_hal.h"

extern uint8_t Location;
extern uint8_t bbb;
extern uint8_t Interface[99]; //8个接口
extern uint8_t Set_LED_Num ;
extern uint8_t KEY;
extern uint8_t key_up;     //按键松开标志
extern uint16_t Long_key_up;
extern uint16_t Shutdown;  //自动关机标志
extern uint8_t key_running;

/*******************************************/
extern uint16_t Loop_number_Times[5]; //第n个循环次数
extern uint8_t Loop_number_Start[5]; //第n个循环开始位置
extern uint8_t Loop_number_Stop[5];  //第n个循环结束位置
extern uint16_t Loop_Times;
extern uint8_t Loop_Start,Loop_Stop;//循环次数/开始位置/结束位置
extern uint8_t Sort_Order;            //第几个循环块
extern uint8_t Order_Residue;        //剩余几个循环块
extern uint8_t Order[4];
/****************错误提示***********************/
extern uint8_t Programming_Error;
extern uint8_t Programming_Warning;
extern uint8_t LED_Glint;
extern uint8_t Light_times;
/******************确定模块顺序***********************/
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
uint8_t waitting (uint8_t i); //等待模块

void Special_Combination_Module(void);

void Boot_detection(void);
void send_data_time(void);
//void LED_Flashing(uint8_t i);
void LED_Flashing(uint8_t i,uint8_t j);

uint8_t Loop_one(uint16_t Loop_Times,uint8_t Loop_Start,uint8_t Loop_Stop);   //一个完整的循环
void Loop_Movement(void);//需要循环个数/循环次数/开始位置/结束位置
void Circular_Order(void);   //循环顺序


void Auto_OFF(void);    //自动关机

/*********错误判断************/
void Programming_Determine(void); //编程判断
void Error_hint(void);


#endif





































