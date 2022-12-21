/**
  ******************************************************************************
  * @ICblocks¼��ģ���������MCU WTshort_us0H4-16S һ�ߴ���
  * @�����Ѿ�ʵ�֣����ϰ���ȫ����
  * @file    Project/main.c 
  * @author  Mr.Li
  * @version V0.0.2
  * @date    2021-09-15
  * @brief   Main pro4gram body
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
#include "stm8s_it.h"

void wtctrl( unsigned char DDATA);//����wt���ƺ���





int button_flag=0;

unsigned char cmd[6]={0x01,0x02,0x03,0x04,0x05,0xF6};//������6������ָ���������5��������¼��


extern u16 GV_Tim2PwmiFreq ;   //����it.c�еı���������ʵ��û��pwm����ʱֵΪ0
extern double GV_Tim2PwmiDuty;//����it.c�еı���������ʵ��û��pwm����ʱֵΪ0



void main(void)
{
 
  //¼��3������PA1,PA2,PB4
  
  CLK_DeInit();
  CLK_HSICmd(ENABLE);
  CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);//1��Ƶ
  delay_init(16);//��ʼ����ʱ����
  GPIO_DeInit(GPIOC);
  GPIO_DeInit(GPIOB);
  GPIO_DeInit(GPIOA);
  GPIO_Init(GPIOC, GPIO_PIN_3, GPIO_MODE_OUT_PP_HIGH_FAST);//��ʼ����������ߵ�ƽ
  GPIO_Init(GPIOC, GPIO_PIN_4, GPIO_MODE_OUT_PP_HIGH_FAST);//��ʼ����������ߵ�ƽ
  GPIO_Init(GPIOA, GPIO_PIN_1, GPIO_MODE_OUT_PP_HIGH_FAST);//��ʼ����������ߵ�ƽ
  GPIO_Init(GPIOA, GPIO_PIN_2, GPIO_MODE_OUT_PP_HIGH_FAST);//��ʼ����������ߵ�ƽ
  enableInterrupts();
  TIM2_PWMI_Init(); //���岶���ã�TIM2 CH1
  TIM2_PWMI_Start();//���岶����, TIM2 CH1
  

  
  
  USART_Config();// ��ʼ��uart
  mprintf("STM8s start...\r\n");

  
  /*  ����wtrоƬ */
  
   wtctrl(0xF1);   //�˳��Զ�˯��
   delay_ms(100);  
   
  
  
  while (1)
  {
    
    
    u16 a = TIM2_PWMI_GetFreq();//��ȡƵ��
    int b = TIM2_PWMI_GetDuty()+0.4;//��ȡռ�ձ�,+0.4��Ϊ�˻�ȡ����

    
    delay_ms(10); 
    
    
    
    /*  ������������͵ذ�������   */
    
    //A1�������ŵ�ǰѡ���İ���
    if (GPIO_ReadInputPin(GPIOA, GPIO_PIN_1) == 0)  //SET or RESET
    {
      
      wtctrl(0xFE);   //�䵱���ѹ��ܣ���ǰ����IC
      delay_ms(100);
      wtctrl(cmd[button_flag]);  //����ѡ�������
      
      while(GPIO_ReadInputPin(GPIOA, GPIO_PIN_1)==0){}
    
      
    };

    
    // B4 ¼��
    if(GPIO_ReadInputPin(GPIOC, GPIO_PIN_4) == 0){
      
      mprintf("luyin ing....   ! \r\n");
      wtctrl(0xFE);   //�䵱���ѹ��ܣ���ǰ����IC
      delay_ms(20); 
      wtctrl(0xF5);  //һ��¼����ʼ
      //delay_ms(50);
      while(GPIO_ReadInputPin(GPIOC, GPIO_PIN_4) == 0){  }
      delay_ms(100);
      wtctrl( 0xFE);     //ֹͣ¼��

    }
    
    // A2Ϊ�л���һ��
    if(GPIO_ReadInputPin(GPIOA, GPIO_PIN_2) == 0){
            
      
      
      mprintf("play luyin.....  ! \r\n");
      wtctrl(0xFE);  //�䵱���ѹ��ܣ���ǰ����IC
      delay_ms(100); 
      button_flag++;
      if(button_flag>5){button_flag=0;}
      wtctrl(cmd[button_flag]);  //����ѡ�������
      
      
      
      while(GPIO_ReadInputPin(GPIOA, GPIO_PIN_2)==0){}
      

     

    }
    

    
    
    mprintf("%d \r\n",b);//��ӡ����
  
    
    /*  ���洦���߼�pwm����  */

    
    switch(b){
    
   //���ŵ�ǰѡ��������
    case 6:{
      
    wtctrl(0xFE);  //�䵱���ѹ��ܣ���ǰ����IC
    delay_ms(200); 
    wtctrl(cmd[button_flag]);  //����ѡ�������
    GV_Tim2PwmiFreq =0;//��������жϵ�Ƶ�ʲɼ�ֵ
    GV_Tim2PwmiDuty =0;//��������жϵ�ռ�ձȲɼ�ֵ
    break;
    }
      
      
    //����¼��
    case 9:{
      
    wtctrl(0xFE);  //�䵱���ѹ��ܣ���ǰ����IC
    delay_ms(200); 
    wtctrl(0xF6);
    GV_Tim2PwmiFreq =0;//��������жϵ�Ƶ�ʲɼ�ֵ
    GV_Tim2PwmiDuty =0;//��������жϵ�ռ�ձȲɼ�ֵ
    break;
    }
    //��������
    case 12:{
    wtctrl(0xFE);  //�䵱���ѹ��ܣ���ǰ����IC
    delay_ms(200); 
    wtctrl(0x02);
    GV_Tim2PwmiFreq =0;//��������жϵ�Ƶ�ʲɼ�ֵ
    GV_Tim2PwmiDuty =0;//��������жϵ�ռ�ձȲɼ�ֵ
    break;
    }
    //������
    case 15:{
    wtctrl(0xFE);  //�䵱���ѹ��ܣ���ǰ����IC
    delay_ms(200); 
    wtctrl(0x03);
    GV_Tim2PwmiFreq =0;//��������жϵ�Ƶ�ʲɼ�ֵ
    GV_Tim2PwmiDuty =0;//��������жϵ�ռ�ձȲɼ�ֵ
    break;
    }
    //���塢��
    case 18:{
    
    wtctrl(0xFE);  //�䵱���ѹ��ܣ���ǰ����IC
    delay_ms(200); 
    wtctrl(0x04);
    GV_Tim2PwmiFreq =0;//��������жϵ�Ƶ�ʲɼ�ֵ
    GV_Tim2PwmiDuty =0;//��������жϵ�ռ�ձȲɼ�ֵ
      break;
    }
    //��������
    case 21:{
    wtctrl(0xFE);  //�䵱���ѹ��ܣ���ǰ����IC
    delay_ms(200); 
    wtctrl(0x05);
    GV_Tim2PwmiFreq =0;//��������жϵ�Ƶ�ʲɼ�ֵ
    GV_Tim2PwmiDuty =0;//��������жϵ�ռ�ձȲɼ�ֵ
      break;
    
    }
    //�绰����
    case 24:{
    wtctrl(0xFE);  //�䵱���ѹ��ܣ���ǰ����IC
    delay_ms(200); 
    wtctrl(0x01);
    GV_Tim2PwmiFreq =0;//��������жϵ�Ƶ�ʲɼ�ֵ
    GV_Tim2PwmiDuty =0;//��������жϵ�ռ�ձȲɼ�ֵ
    
    
    break;
    }
      
      
      
      
    
    
    }
    
    
    
    
    
    
    
    
   
    
    
    
    



    
   /* 
    //��������ź�
    for(int i=0;i<1000;i++)
    {
      
        TIM1_SetCompare3(i);//����ռ�ձ�
        delay_ms(1);
        
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
 GPIO_WriteLow(GPIOC, GPIO_PIN_3);
 delay_ms(5);      //��ʱ5ms
 B_DATA = S_DATA&0x01;
 
 for(j=0;j<8;j++)
 {
  if(B_DATA == 1)
  {
   GPIO_WriteHigh(GPIOC, GPIO_PIN_3);
   delay_us(900);    //��ʱ600us
   GPIO_WriteLow(GPIOC, GPIO_PIN_3);
   delay_us(300);   //��ʱ200us
  }
  else
  {
   GPIO_WriteHigh(GPIOC, GPIO_PIN_3);
   delay_us(300);  //��ʱ200us
   GPIO_WriteLow(GPIOC, GPIO_PIN_3);
   delay_us(900);  //��ʱ600us  
  }
  S_DATA = S_DATA>>1;   // ����һλ
  B_DATA = S_DATA&0x01; // ȡ�����λ
 } 
 GPIO_WriteHigh(GPIOC, GPIO_PIN_3);//������������ź���
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
