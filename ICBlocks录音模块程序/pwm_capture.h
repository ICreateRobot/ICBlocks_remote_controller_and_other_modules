//TIM1 ch3输出pwm信号初始化函数

#include "stm8s.h"


#ifndef _pwm_capture_h
#define _pwm_capture_h


/******************************************************************************
                               外部调用功能函数
******************************************************************************/


void TIM2_PWMI_Init();        //TIM2 PWM 输入初始化
void TIM2_PWMI_Start();       //开始 PWM 捕获
void TIM2_PWMI_Stop();        //终止 PWM 捕获
u16 TIM2_PWMI_GetFreq();      //获取 TIM2 CH1 捕获的 PWM 频率
double TIM2_PWMI_GetDuty();  //获取 TIM2 CH1 捕获的 PWM 正占空比





#endif
