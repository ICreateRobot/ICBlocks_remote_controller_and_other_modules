//TIM2 ch1 pwm信号捕获

#include "stm8s.h"



  extern u16 GV_Tim2PwmiFreq ;       //【系统使用】定时器捕获的 PWM 频率
  extern double GV_Tim2PwmiDuty;  //【系统使用】定时器捕获的 PWM 正占空比
 extern int cap_flag;



/**
 * TIM2 PWM 输入初始化
 */
void TIM2_PWMI_Init()
{
    // 重置初始化
    TIM2_DeInit();
 
    // 初始化 TIME1 时基单元
    // 16预分频，向上计数，每 1s/1M = 1us 计数器值加 1（累加 1000 次需要 1 毫秒）
    // 参数说明：预分配值，自动重装载值（计数器值到 65536 后重新装载）
    TIM2_TimeBaseInit(TIM2_PRESCALER_16, 65536-1);
 
    // PWM的通道1配置
    //
    // 参数说明：
    //　　1. TIM2 通道1
    //　　2. 极性：上升沿捕获
    //　　3. 输入脚：TIM2_ICSELECTION_DIRECTTI 表示直接将 T1 信号经过 TI1FP1 发送给 IC1 模块进行处理
    //　　4. 预分频：意思是控制在多少个输入周期做一次捕获；【经过实际测试，复位触发模式下该值无效，都是按照1分频执行】
    //       　　　　1M 系统时钟，捕获 20Hz PWM 一个周期需要计数 50000  次（1M/20=50000），TIM2 最大计数有65536，可以捕获
    //       　　　　1M 系统时钟，捕获 15Hz PWM 一个周期需要计数 66666  次（1M/15=66666），TIM2 最大计数有65536，无法捕获
    //　　5. 滤波频率：经历几个周期相同的跳变则波形稳定。如果频率捕获偶发值不对，那么可以启用 0x02（4个事件）
    TIM2_PWMIConfig(TIM2_CHANNEL_1, TIM2_ICPOLARITY_RISING, TIM2_ICSELECTION_DIRECTTI, TIM2_ICPSC_DIV1, 0x00);
    
    // PWM的通道2配置
    //【经过实际测试，通道2 可以不用配置，也可以配置，但是如果配置那么参数必须要填写正确：TIM2_ICPOLARITY_FALLING，TIM2_ICSELECTION_INDIRECTTI】
    //【至于为什么不用配置系统仍旧能正常运行，猜想可能是 STM8库函数 内部自动做了一些处理】
    // 
    // 参数说明：
    //　　1. TIM2 通道2
    //　　2. 极性：下降沿捕获
    //　　3. TIM2_ICSELECTION_INDIRECTTI 表示直接将 T1 信号经过 TI1FP2 发送给 IC2 模块进行处理
    //　　4. 预分频：意思是控制在多少个输入周期做一次捕获；【经过实际测试，复位触发模式下该值无效，都是按照1分频执行】
    //　　5. 滤波频率：经历几个周期相同的跳变则波形稳定。如果频率捕获偶发值不对，那么可以启用 0x02（4个事件）
    //TIM2_PWMIConfig(TIM2_CHANNEL_2, TIM2_ICPOLARITY_FALLING, TIM2_ICSELECTION_INDIRECTTI, TIM2_ICPSC_DIV1, 0x00);
 
    // 输入捕获/比较输出使能
    //【经过实际测试，以下使能代码可以注释掉，但是为了安心还是加上去】
    TIM2_CCxCmd(TIM2_CHANNEL_1, ENABLE);
    TIM2_CCxCmd(TIM2_CHANNEL_2, ENABLE);
 
}
 
/**
 * 开始 PWM 捕获
 */
void TIM2_PWMI_Start()
{
    // 清空俘获中断标志
    TIM2_ClearITPendingBit(TIM2_IT_CC1);
    TIM2_ClearFlag(TIM2_FLAG_CC1);
    
    // 启用中断函数：俘获比较通道CH1中断
    TIM2_ITConfig(TIM2_IT_CC1, ENABLE);
 
    // 开定时器
    //【经过实际测试，定时器就算未开启，上面的 TIM2_IT_CC1 中断仍正常进入，但是获取的计数器值不确定】
    TIM2_Cmd(ENABLE);
}
 
 
/**
 * 终止 PWM 捕获
 * 利用 TIM2_Cmd(DISABLE) 关闭输出，注意其他函数可能受影响，例如：TIM2_PWMI_Init
 */
void TIM2_PWMI_Stop()
{
    // 关闭中断函数：俘获比较通道CH1中断
    TIM2_ITConfig(TIM2_IT_CC1, DISABLE);
 
    // 关闭定时器
    TIM2_Cmd(DISABLE);
}
 
 
/**
 * 获取 TIM2 CH1 捕获的 PWM 频率
 *
 * @return integer  频率
 */
u16 TIM2_PWMI_GetFreq()
{
    return GV_Tim2PwmiFreq;
}
 
 
/**
 * 获取 TIM2 CH1 捕获的 PWM 正占空比
 *
 * @return double  占空比
 */


double TIM2_PWMI_GetDuty()
{
    return GV_Tim2PwmiDuty;
}










