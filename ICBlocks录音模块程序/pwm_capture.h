//TIM1 ch3���pwm�źų�ʼ������

#include "stm8s.h"


#ifndef _pwm_capture_h
#define _pwm_capture_h


/******************************************************************************
                               �ⲿ���ù��ܺ���
******************************************************************************/


void TIM2_PWMI_Init();        //TIM2 PWM �����ʼ��
void TIM2_PWMI_Start();       //��ʼ PWM ����
void TIM2_PWMI_Stop();        //��ֹ PWM ����
u16 TIM2_PWMI_GetFreq();      //��ȡ TIM2 CH1 ����� PWM Ƶ��
double TIM2_PWMI_GetDuty();  //��ȡ TIM2 CH1 ����� PWM ��ռ�ձ�





#endif
