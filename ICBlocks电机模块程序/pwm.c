//TIM1 ch3���pwm�źų�ʼ������

#include "stm8s.h"
#include "stdio.h"



void Tim1_PWM_Init(void)
{
/* Time base configuration,clk=cpu=16M,Ԥ��Ƶ15����������Ϊ1000Hz=1ms */
    TIM1_TimeBaseInit(15,TIM1_COUNTERMODE_UP,1000,0);//��ʼ����ʱ��1
    TIM1_ARRPreloadConfig(ENABLE);//ʹ���Զ���װ
    TIM1_OC3Init(TIM1_OCMODE_PWM2,TIM1_OUTPUTSTATE_ENABLE,TIM1_OUTPUTNSTATE_ENABLE,500,TIM1_OCPOLARITY_LOW,TIM1_OCNPOLARITY_LOW,TIM1_OCIDLESTATE_SET,TIM1_OCNIDLESTATE_SET);
    TIM1_OC4Init(TIM1_OCMODE_PWM2,TIM1_OUTPUTSTATE_ENABLE, 500, TIM1_OCPOLARITY_LOW, TIM1_OCIDLESTATE_SET);



    
    TIM1_Cmd(ENABLE);//����ʱ��
    TIM1_CtrlPWMOutputs(ENABLE);//ʹ��PWM�����
    
    

    
    
    
    
}









