#ifndef __Data_Analysis_H
#define __Data_Analysis_H
#include "stm32f1xx_hal.h"
extern uint8_t Location;
extern uint8_t bbb;
extern uint8_t Interface[8]; //8¸ö½Ó¿Ú
void Interface_module(void);
void Gyro_Module(void);
void Module_Init(void);
uint8_t KEY_Scan(void);
void emoji_Module(uint8_t i,uint8_t j);
void voice_Module(uint8_t i,uint8_t j);
void other_Module(uint8_t i,uint8_t j);
void Start_Motor_Potentiometer(uint8_t i);
void Start_Potentiometer(uint8_t i);
void Tilt_Movement(uint8_t i);
void straight_Movement(uint8_t i);
void Right_Movement(uint8_t i);
void Left_Movement(uint8_t i);
void Special_Combination_Module(void);
void Boot_detection(void);
void send_data_time(void);
//void LED_Flashing(uint8_t i);
void LED_Flashing(uint8_t i,uint8_t j);
#endif


