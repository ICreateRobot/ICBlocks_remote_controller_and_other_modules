/**
******************************************************************************
* @ICblocks���ģ�����pwm������·pwm���
* @file    Project/main.c 
* @author  Mr.Li
* @version V0.0.1
* @date    2021-08-10
* @brief   Main program body
******************************************************************************
* @attention
******************************************************************************
*/ 


/* Includes ------------------------------------------------------------------*/
#include "stm8s.h"
#include "pwm.h"
#include "delay.h"
#include "usart.h"
#include "pwm_capture.h"


extern u16 GV_Tim2PwmiFreq ;   //����it.c�еı���������ʵ��û��pwm����ʱֵΪ0
extern double GV_Tim2PwmiDuty;//����it.c�еı���������ʵ��û��pwm����ʱֵΪ0
extern double cap_flag; 
int value_flag=0;

int power_on_flag=0;
//int b=0;



void main(void)
{
  
  
  CLK_DeInit();
  CLK_HSICmd(ENABLE);
  CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);//1��Ƶ
  delay_init(16);//��ʼ����ʱ����
  //delay_ms(300);
  GPIO_DeInit(GPIOB);
  GPIO_Init(GPIOB, GPIO_PIN_5, GPIO_MODE_OUT_PP_HIGH_FAST);//��ʼ����������ߵ�ƽ
  
  
  
  // GPIO_Init(GPIOC, GPIO_PIN_3, GPIO_MODE_OUT_PP_HIGH_FAST);//��ʼ����������ߵ�ƽ
  //GPIO_Init(GPIOC, GPIO_PIN_4, GPIO_MODE_OUT_PP_HIGH_FAST);//��ʼ����������ߵ�ƽ
  
  enableInterrupts();
  TIM2_PWMI_Init(); //���岶����ii��TIM2 CH1
  TIM2_PWMI_Start();//���岶����, TIM2 CH1
  
  
  USART_Config();// ��ʼ��uart
  mprintf("STM8s start...\r\n");
  Tim1_PWM_Init();//TIM1 CH3���pwm
  //delay_ms(200);
  // delay_ms(1000);
  while (1)     
  {
    
   //  b=7;
   //  GV_Tim2PwmiDuty=7;
    u16 a = TIM2_PWMI_GetFreq();//��ȡƵ��
    int b = TIM2_PWMI_GetDuty()+0.4;//��ȡռ�ձ�
    mprintf("%d \r\n",b);//"key passed! \r\n"
    //mprintf("%d \r\n",cap_flag);//"key passed! \r\n"

   
    delay_ms(10);
    
    
    //�ȴ���ɰ�ң�������ƾɰ�����+�µ���Ŀ���
    //stm8ʵ��ռ�ձ�   24,22,20,33(��ֵ),4,102,101
    //���һ�β��Է�102��101��ֻ��101��ֱ������������
      if(cap_flag==0){
        if((GPIOD->IDR & (uint8_t)GPIO_PIN_4) ? SET : RESET)
        {
          TIM1_SetCompare3(0);//����ռ�ձ�
          TIM1_SetCompare4(0);//����ռ�ձ�
          delay_ms(2);
        }
        else
        {
          TIM1_SetCompare3(900);//����ռ�ձ�
          TIM1_SetCompare4(0);//����ռ�ձ�
          delay_ms(10);
        }
        /*
        TIM1_SetCompare3(900);//����ռ�ձ�  1��2��3��4��5
        TIM1_SetCompare4(0);//����ռ�ձ�
        delay_ms(10);
        */ 
        
      }
    else if(b>=99){
      // b==100
      //b=4��ʱ����ch4��400
      if(b==102){
        
        TIM1_SetCompare3(1000);//����ռ�ձ�
        TIM1_SetCompare4(0);//����ռ�ձ�
        delay_ms(2);
        mprintf("qqqqqqqqq");
      }
      if(b==101){
        TIM1_SetCompare3(1000);//����ռ�ձ�
        TIM1_SetCompare4(0);//����ռ�ձ�
        delay_ms(2);
        mprintf("wwwwwww");
      }
       
      power_on_flag=1;
      
      
    }
   
    else if(b==20){
      
      TIM1_SetCompare3(0);//����ռ�ձ�
      TIM1_SetCompare4(400);//����ռ�ձ�
      delay_ms(2);
      power_on_flag=1;
      mprintf("eeeeeeee");
    }
    else if(b==22){
      
      TIM1_SetCompare3(0);//����ռ�ձ�
      TIM1_SetCompare4(600);//����ռ�ձ�
      delay_ms(2);
      mprintf("rrrrrrrr");
      power_on_flag=1;
    }
    
    else if(b==24){
      
      TIM1_SetCompare3(0);//����ռ�ձ�
      TIM1_SetCompare4(800);//����ռ�ձ�
      delay_ms(2);
      power_on_flag=1;
      mprintf("ttttttttt");
    }
    
    
    //����ɰ�ң�������ƾɰ�����+�µ���Ŀ����߼�������
    
    
    //�������ԣ�ʹ�õ�λ�����Ƶ��ģ��ʱ��ռ�ձ�Ϊ 0-12
    //6,7������Ϊ����ֵ������
    else if(b==6||b==7){  // 6,7 ֹͣ
      
      TIM1_SetCompare3(0);//����ռ�ձ�
      TIM1_SetCompare4(0);//����ռ�ձ�
      delay_ms(2);
      //cap_flag = 0 ;
      mprintf("dddddddd");
      power_on_flag=1;
      
    }
    else if( b>7 && b<14 ){//��ת
      
      TIM1_SetCompare3(0);//����ռ�ձ�  8,9,10,11,12
      TIM1_SetCompare4((b-7)*200);//����ռ�ձ�
      delay_ms(2);
      //cap_flag = 0 ;
      
      mprintf("cccccccccc");
      power_on_flag=1;
    }
    else if( b<6 &&b>0 ){//��ת
      
      TIM1_SetCompare3((6-b)*200);//����ռ�ձ�  1��2��3��4��5
      TIM1_SetCompare4(0);//����ռ�ձ�
      delay_ms(2);
      
      mprintf("bbbbbbbbbb");
      power_on_flag=1;
      
      
    }
    
    
    //�ɰ�ң���������������������Ӧpwm 
    //���������31,32
    //ֹͣ�����33,34
    else if(b==32||b==31){
      
      
      TIM1_SetCompare3(400);//����ռ�ձ�  1��2��3��4��5
      TIM1_SetCompare4(0);//����ռ�ձ�
      delay_ms(2);
      power_on_flag=1;
      
    }
    
    else if(b==33||b==34){
      
      
      TIM1_SetCompare3(0);//����ռ�ձ�  1��2��3��4��5
      TIM1_SetCompare4(0);//����ռ�ձ�
      delay_ms(2);
      power_on_flag=1;
      
    }    
    
    //�ɰ�����������������ʵ��ռ�ձ� 27��28
    else if(b==27||b==28){//cap_flag==1
      
      
      TIM1_SetCompare3(900);//����ռ�ձ�  1��2��3��4��5
      delay_ms(5);
      TIM1_SetCompare4(0);//����ռ�ձ�
      delay_ms(20);
      
      mprintf("mmmmmmmm");
      GV_Tim2PwmiFreq = 0;
      GV_Tim2PwmiDuty = 0;
      power_on_flag=1;
      
    }   

    cap_flag = 0;
    
    
    /*    
    else if(b>25){//��ת(cap_flag==1)&&(
    
    
    
    TIM1_SetCompare3(0);
    TIM1_SetCompare4(200);
    delay_ms(1);
    cap_flag = 0 ;
    mprintf("fanzhuan");
    
    
  }
    else if((cap_flag==0)&&(b>26)){//��ת
    
    
    
    TIM1_SetCompare3(0);
    TIM1_SetCompare4(0);
    delay_ms(1);
    cap_flag = 0 ;
    mprintf("else");
    
    
  }
    
    */
    
    
    
    
    
    //��������ź�
    
    /* 
    
    for(int i=0;i<1000;i++)
    {
    
    TIM1_SetCompare3(i);//����ռ�ձ�
    TIM1_SetCompare4(0);//����ռ�ձ�
    delay_ms(2);
    
  }
    
    for(int i=0;i<1000;i++)
    {
    
    TIM1_SetCompare3(0);//����ռ�ձ�
    TIM1_SetCompare4(i);//����ռ�ձ�
    delay_ms(2);
    
  }
    
    
    */
    
    
    
    
    
    
  }
  
  
} 









/*-------------------------------------- 

;��    ��:ʵ��һ�ߴ���ͨ�ź���
;��    ��: DDATAΪ��������
;��    ��:
;-------------------------------------*/
void wtctrl( unsigned char DDATA)
{
  unsigned char  S_DATA,j;
  unsigned char  B_DATA; 
  
  S_DATA =  DDATA;
  GPIO_WriteLow(GPIOB, GPIO_PIN_5);
  delay_ms(5);      //��ʱ5ms
  B_DATA = S_DATA&0x01;
  
  for(j=0;j<8;j++)
  {
    if(B_DATA == 1)
    {
      GPIO_WriteHigh(GPIOB, GPIO_PIN_5);
      delay_us(1200);    //��ʱ600us
      GPIO_WriteLow(GPIOB, GPIO_PIN_5);
      delay_us(400);   //��ʱ200us
    }
    else
    {
      GPIO_WriteHigh(GPIOB, GPIO_PIN_5);
      delay_us(400);  //��ʱ200us
      GPIO_WriteLow(GPIOB, GPIO_PIN_5);
      delay_us(1200);  //��ʱ600us  
    }
    S_DATA = S_DATA>>1;   // ����һλ
    B_DATA = S_DATA&0x01; // ȡ�����λ
  } 
  GPIO_WriteHigh(GPIOB, GPIO_PIN_5);//������������ź���
}
















#ifdef USE_FULL_ASSERT

/**
* @brief  Reports the name of the source file and the source line number
*   where the assert_param error has occurred.
* @param file: pointer to the source file name
* @param line: assert_param error line source number
* @retval : None
*/
void assert_failed(u8* file, u32 line) //����
{ 
  /* User can add his own implementation to report the file name and line number,
  ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  
  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
