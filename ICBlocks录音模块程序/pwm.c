//TIM1 ch3输出pwm信号初始化函数

#include "stm8s.h"
#include "stdio.h"



void Tim1_PWM_Init(void)
{
/* Time base configuration,clk=cpu=16M,预分频15，计数周期为1000Hz=1ms */
    TIM1_TimeBaseInit(15, TIM1_COUNTERMODE_UP,1000,0);
 
    /* PWM1 Mode configuration: Channel1 */
    TIM1_OC3Init(TIM1_OCMODE_PWM1, TIM1_OUTPUTSTATE_ENABLE,TIM1_OUTPUTNSTATE_DISABLE,
                    1000>>1, TIM1_OCPOLARITY_LOW,TIM1_OCNPOLARITY_LOW,
                    TIM1_OCIDLESTATE_RESET,TIM1_OCNIDLESTATE_RESET);
    TIM1_OC3PreloadConfig(ENABLE);
    TIM1_CCxCmd(TIM1_CHANNEL_3,ENABLE);
    TIM1_CtrlPWMOutputs(ENABLE); // 若不使能，TIM1是不会产生PWM输出的
    //TIM1_ARRPreloadConfig(ENABLE);
 
    /* enable counter */
    TIM1_Cmd(ENABLE);
}









