//TIM2 ch1 pwm�źŲ���

#include "stm8s.h"



  extern u16 GV_Tim2PwmiFreq ;       //��ϵͳʹ�á���ʱ������� PWM Ƶ��
  extern double GV_Tim2PwmiDuty;  //��ϵͳʹ�á���ʱ������� PWM ��ռ�ձ�
 extern int cap_flag;



/**
 * TIM2 PWM �����ʼ��
 */
void TIM2_PWMI_Init()
{
    // ���ó�ʼ��
    TIM2_DeInit();
 
    // ��ʼ�� TIME1 ʱ����Ԫ
    // 16Ԥ��Ƶ�����ϼ�����ÿ 1s/1M = 1us ������ֵ�� 1���ۼ� 1000 ����Ҫ 1 ���룩
    // ����˵����Ԥ����ֵ���Զ���װ��ֵ��������ֵ�� 65536 ������װ�أ�
    TIM2_TimeBaseInit(TIM2_PRESCALER_16, 65536-1);
 
    // PWM��ͨ��1����
    //
    // ����˵����
    //����1. TIM2 ͨ��1
    //����2. ���ԣ������ز���
    //����3. ����ţ�TIM2_ICSELECTION_DIRECTTI ��ʾֱ�ӽ� T1 �źž��� TI1FP1 ���͸� IC1 ģ����д���
    //����4. Ԥ��Ƶ����˼�ǿ����ڶ��ٸ�����������һ�β��񣻡�����ʵ�ʲ��ԣ���λ����ģʽ�¸�ֵ��Ч�����ǰ���1��Ƶִ�С�
    //       ��������1M ϵͳʱ�ӣ����� 20Hz PWM һ��������Ҫ���� 50000  �Σ�1M/20=50000����TIM2 ��������65536�����Բ���
    //       ��������1M ϵͳʱ�ӣ����� 15Hz PWM һ��������Ҫ���� 66666  �Σ�1M/15=66666����TIM2 ��������65536���޷�����
    //����5. �˲�Ƶ�ʣ���������������ͬ�����������ȶ������Ƶ�ʲ���ż��ֵ���ԣ���ô�������� 0x02��4���¼���
    TIM2_PWMIConfig(TIM2_CHANNEL_1, TIM2_ICPOLARITY_RISING, TIM2_ICSELECTION_DIRECTTI, TIM2_ICPSC_DIV1, 0x00);
    
    // PWM��ͨ��2����
    //������ʵ�ʲ��ԣ�ͨ��2 ���Բ������ã�Ҳ�������ã��������������ô��������Ҫ��д��ȷ��TIM2_ICPOLARITY_FALLING��TIM2_ICSELECTION_INDIRECTTI��
    //������Ϊʲô��������ϵͳ�Ծ����������У���������� STM8�⺯�� �ڲ��Զ�����һЩ����
    // 
    // ����˵����
    //����1. TIM2 ͨ��2
    //����2. ���ԣ��½��ز���
    //����3. TIM2_ICSELECTION_INDIRECTTI ��ʾֱ�ӽ� T1 �źž��� TI1FP2 ���͸� IC2 ģ����д���
    //����4. Ԥ��Ƶ����˼�ǿ����ڶ��ٸ�����������һ�β��񣻡�����ʵ�ʲ��ԣ���λ����ģʽ�¸�ֵ��Ч�����ǰ���1��Ƶִ�С�
    //����5. �˲�Ƶ�ʣ���������������ͬ�����������ȶ������Ƶ�ʲ���ż��ֵ���ԣ���ô�������� 0x02��4���¼���
    //TIM2_PWMIConfig(TIM2_CHANNEL_2, TIM2_ICPOLARITY_FALLING, TIM2_ICSELECTION_INDIRECTTI, TIM2_ICPSC_DIV1, 0x00);
 
    // ���벶��/�Ƚ����ʹ��
    //������ʵ�ʲ��ԣ�����ʹ�ܴ������ע�͵�������Ϊ�˰��Ļ��Ǽ���ȥ��
    TIM2_CCxCmd(TIM2_CHANNEL_1, ENABLE);
    TIM2_CCxCmd(TIM2_CHANNEL_2, ENABLE);
 
}
 
/**
 * ��ʼ PWM ����
 */
void TIM2_PWMI_Start()
{
    // ��շ����жϱ�־
    TIM2_ClearITPendingBit(TIM2_IT_CC1);
    TIM2_ClearFlag(TIM2_FLAG_CC1);
    
    // �����жϺ���������Ƚ�ͨ��CH1�ж�
    TIM2_ITConfig(TIM2_IT_CC1, ENABLE);
 
    // ����ʱ��
    //������ʵ�ʲ��ԣ���ʱ������δ����������� TIM2_IT_CC1 �ж����������룬���ǻ�ȡ�ļ�����ֵ��ȷ����
    TIM2_Cmd(ENABLE);
}
 
 
/**
 * ��ֹ PWM ����
 * ���� TIM2_Cmd(DISABLE) �ر������ע����������������Ӱ�죬���磺TIM2_PWMI_Init
 */
void TIM2_PWMI_Stop()
{
    // �ر��жϺ���������Ƚ�ͨ��CH1�ж�
    TIM2_ITConfig(TIM2_IT_CC1, DISABLE);
 
    // �رն�ʱ��
    TIM2_Cmd(DISABLE);
}
 
 
/**
 * ��ȡ TIM2 CH1 ����� PWM Ƶ��
 *
 * @return integer  Ƶ��
 */
u16 TIM2_PWMI_GetFreq()
{
    return GV_Tim2PwmiFreq;
}
 
 
/**
 * ��ȡ TIM2 CH1 ����� PWM ��ռ�ձ�
 *
 * @return double  ռ�ձ�
 */


double TIM2_PWMI_GetDuty()
{
    return GV_Tim2PwmiDuty;
}










