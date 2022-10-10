//TIM1 ch3输出pwm信号初始化函数

#include "stm8s.h"
#include "stdio.h"



void Tim1_PWM_Init(void)
{
/* Time base configuration,clk=cpu=16M,预分频15，计数周期为1000Hz=1ms */
    TIM1_TimeBaseInit(15,TIM1_COUNTERMODE_UP,1000,0);//初始化定时器1
    TIM1_ARRPreloadConfig(ENABLE);//使能自动重装
    TIM1_OC3Init(TIM1_OCMODE_PWM2,TIM1_OUTPUTSTATE_ENABLE,TIM1_OUTPUTNSTATE_ENABLE,500,TIM1_OCPOLARITY_LOW,TIM1_OCNPOLARITY_LOW,TIM1_OCIDLESTATE_SET,TIM1_OCNIDLESTATE_SET);
    TIM1_OC4Init(TIM1_OCMODE_PWM2,TIM1_OUTPUTSTATE_ENABLE, 500, TIM1_OCPOLARITY_LOW, TIM1_OCIDLESTATE_SET);



    
    TIM1_Cmd(ENABLE);//开定时器
    TIM1_CtrlPWMOutputs(ENABLE);//使能PWM主输出
    
    

    
    
    
    
}









