#ifndef __Data_Analysis_H
#define __Data_Analysis_H

#include "stm32f1xx_hal.h"

extern uint8_t Location;
extern uint8_t bbb;
extern uint8_t Interface[8]; //8个接口
extern uint8_t Programming_Error;
extern uint8_t Programming_Warning;
extern uint8_t LED_Glint;
extern uint8_t Light_times;


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

void Programming_Determine(void); //编程判断





#endif


