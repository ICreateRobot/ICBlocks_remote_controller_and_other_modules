/**
******************************************************************************
* @ICblocks电机模块程序，pwm捕获，两路pwm输出
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


extern u16 GV_Tim2PwmiFreq ;   //引用it.c中的变量，用来实现没有pwm捕获时值为0
extern double GV_Tim2PwmiDuty;//引用it.c中的变量，用来实现没有pwm捕获时值为0
extern double cap_flag; 
int value_flag=0;

int power_on_flag=0;
//int b=0;



void main(void)
{
  
  
  CLK_DeInit();
  CLK_HSICmd(ENABLE);
  CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);//1分频
  delay_init(16);//初始化延时函数
  //delay_ms(300);
  GPIO_DeInit(GPIOB);
  GPIO_Init(GPIOB, GPIO_PIN_5, GPIO_MODE_OUT_PP_HIGH_FAST);//初始化高速推挽高电平
  
  
  
  // GPIO_Init(GPIOC, GPIO_PIN_3, GPIO_MODE_OUT_PP_HIGH_FAST);//初始化高速推挽高电平
  //GPIO_Init(GPIOC, GPIO_PIN_4, GPIO_MODE_OUT_PP_HIGH_FAST);//初始化高速推挽高电平
  
  enableInterrupts();
  TIM2_PWMI_Init(); //脉冲捕获用ii，TIM2 CH1
  TIM2_PWMI_Start();//脉冲捕获用, TIM2 CH1
  
  
  USART_Config();// 初始化uart
  mprintf("STM8s start...\r\n");
  Tim1_PWM_Init();//TIM1 CH3输出pwm
  //delay_ms(200);
  // delay_ms(1000);
  while (1)     
  {
    
   //  b=7;
   //  GV_Tim2PwmiDuty=7;
    u16 a = TIM2_PWMI_GetFreq();//获取频率
    int b = TIM2_PWMI_GetDuty()+0.4;//获取占空比
    mprintf("%d \r\n",b);//"key passed! \r\n"
    //mprintf("%d \r\n",cap_flag);//"key passed! \r\n"

   
    delay_ms(10);
    
    
    //先处理旧版遥控器控制旧版主控+新电机的控制
    //stm8实测占空比   24,22,20,33(中值),4,102,101
    //最后一次测试非102，101，只有101，直接拉到满功率
      if(cap_flag==0){
        if((GPIOD->IDR & (uint8_t)GPIO_PIN_4) ? SET : RESET)
        {
          TIM1_SetCompare3(0);//设置占空比
          TIM1_SetCompare4(0);//设置占空比
          delay_ms(2);
        }
        else
        {
          TIM1_SetCompare3(900);//设置占空比
          TIM1_SetCompare4(0);//设置占空比
          delay_ms(10);
        }
        /*
        TIM1_SetCompare3(900);//设置占空比  1，2，3，4，5
        TIM1_SetCompare4(0);//设置占空比
        delay_ms(10);
        */ 
        
      }
    else if(b>=99){
      // b==100
      //b=4的时候是ch4是400
      if(b==102){
        
        TIM1_SetCompare3(1000);//设置占空比
        TIM1_SetCompare4(0);//设置占空比
        delay_ms(2);
        mprintf("qqqqqqqqq");
      }
      if(b==101){
        TIM1_SetCompare3(1000);//设置占空比
        TIM1_SetCompare4(0);//设置占空比
        delay_ms(2);
        mprintf("wwwwwww");
      }
       
      power_on_flag=1;
      
      
    }
   
    else if(b==20){
      
      TIM1_SetCompare3(0);//设置占空比
      TIM1_SetCompare4(400);//设置占空比
      delay_ms(2);
      power_on_flag=1;
      mprintf("eeeeeeee");
    }
    else if(b==22){
      
      TIM1_SetCompare3(0);//设置占空比
      TIM1_SetCompare4(600);//设置占空比
      delay_ms(2);
      mprintf("rrrrrrrr");
      power_on_flag=1;
    }
    
    else if(b==24){
      
      TIM1_SetCompare3(0);//设置占空比
      TIM1_SetCompare4(800);//设置占空比
      delay_ms(2);
      power_on_flag=1;
      mprintf("ttttttttt");
    }
    
    
    //处理旧版遥控器控制旧版主控+新电机的控制逻辑到这里
    
    
    //经过测试，使用电位器控制电机模块时，占空比为 0-12
    //6,7可以认为是中值，死区
    else if(b==6||b==7){  // 6,7 停止
      
      TIM1_SetCompare3(0);//设置占空比
      TIM1_SetCompare4(0);//设置占空比
      delay_ms(2);
      //cap_flag = 0 ;
      mprintf("dddddddd");
      power_on_flag=1;
      
    }
    else if( b>7 && b<14 ){//正转
      
      TIM1_SetCompare3(0);//设置占空比  8,9,10,11,12
      TIM1_SetCompare4((b-7)*200);//设置占空比
      delay_ms(2);
      //cap_flag = 0 ;
      
      mprintf("cccccccccc");
      power_on_flag=1;
    }
    else if( b<6 &&b>0 ){//反转
      
      TIM1_SetCompare3((6-b)*200);//设置占空比  1，2，3，4，5
      TIM1_SetCompare4(0);//设置占空比
      delay_ms(2);
      
      mprintf("bbbbbbbbbb");
      power_on_flag=1;
      
      
    }
    
    
    //旧版遥控器控制主控启动电机对应pwm 
    //启动电机：31,32
    //停止电机：33,34
    else if(b==32||b==31){
      
      
      TIM1_SetCompare3(400);//设置占空比  1，2，3，4，5
      TIM1_SetCompare4(0);//设置占空比
      delay_ms(2);
      power_on_flag=1;
      
    }
    
    else if(b==33||b==34){
      
      
      TIM1_SetCompare3(0);//设置占空比  1，2，3，4，5
      TIM1_SetCompare4(0);//设置占空比
      delay_ms(2);
      power_on_flag=1;
      
    }    
    
    //旧版主控输入控制输出，实测占空比 27，28
    else if(b==27||b==28){//cap_flag==1
      
      
      TIM1_SetCompare3(900);//设置占空比  1，2，3，4，5
      delay_ms(5);
      TIM1_SetCompare4(0);//设置占空比
      delay_ms(20);
      
      mprintf("mmmmmmmm");
      GV_Tim2PwmiFreq = 0;
      GV_Tim2PwmiDuty = 0;
      power_on_flag=1;
      
    }   

    cap_flag = 0;
    
    
    /*    
    else if(b>25){//反转(cap_flag==1)&&(
    
    
    
    TIM1_SetCompare3(0);
    TIM1_SetCompare4(200);
    delay_ms(1);
    cap_flag = 0 ;
    mprintf("fanzhuan");
    
    
  }
    else if((cap_flag==0)&&(b>26)){//反转
    
    
    
    TIM1_SetCompare3(0);
    TIM1_SetCompare4(0);
    delay_ms(1);
    cap_flag = 0 ;
    mprintf("else");
    
    
  }
    
    */
    
    
    
    
    
    //输出呼吸信号
    
    /* 
    
    for(int i=0;i<1000;i++)
    {
    
    TIM1_SetCompare3(i);//设置占空比
    TIM1_SetCompare4(0);//设置占空比
    delay_ms(2);
    
  }
    
    for(int i=0;i<1000;i++)
    {
    
    TIM1_SetCompare3(0);//设置占空比
    TIM1_SetCompare4(i);//设置占空比
    delay_ms(2);
    
  }
    
    
    */
    
    
    
    
    
    
  }
  
  
} 









/*-------------------------------------- 

;功    能:实现一线串口通信函数
;入    参: DDATA为发送数据
;出    参:
;-------------------------------------*/
void wtctrl( unsigned char DDATA)
{
  unsigned char  S_DATA,j;
  unsigned char  B_DATA; 
  
  S_DATA =  DDATA;
  GPIO_WriteLow(GPIOB, GPIO_PIN_5);
  delay_ms(5);      //延时5ms
  B_DATA = S_DATA&0x01;
  
  for(j=0;j<8;j++)
  {
    if(B_DATA == 1)
    {
      GPIO_WriteHigh(GPIOB, GPIO_PIN_5);
      delay_us(1200);    //延时600us
      GPIO_WriteLow(GPIOB, GPIO_PIN_5);
      delay_us(400);   //延时200us
    }
    else
    {
      GPIO_WriteHigh(GPIOB, GPIO_PIN_5);
      delay_us(400);  //延时200us
      GPIO_WriteLow(GPIOB, GPIO_PIN_5);
      delay_us(1200);  //延时600us  
    }
    S_DATA = S_DATA>>1;   // 右移一位
    B_DATA = S_DATA&0x01; // 取出最高位
  } 
  GPIO_WriteHigh(GPIOB, GPIO_PIN_5);//控制完成拉高信号线
}
















#ifdef USE_FULL_ASSERT

/**
* @brief  Reports the name of the source file and the source line number
*   where the assert_param error has occurred.
* @param file: pointer to the source file name
* @param line: assert_param error line source number
* @retval : None
*/
void assert_failed(u8* file, u32 line) //断言
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
