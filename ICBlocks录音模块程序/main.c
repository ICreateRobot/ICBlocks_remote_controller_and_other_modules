/**
  ******************************************************************************
  * @ICblocks录音模块程序，配套MCU WTshort_us0H4-16S 一线串口
  * @功能已经实现，与老版完全兼容
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

void wtctrl( unsigned char DDATA);//声明wt控制函数





int button_flag=0;

unsigned char cmd[6]={0x01,0x02,0x03,0x04,0x05,0xF6};//定义了6个播放指令，播放内置5段声音和录音


extern u16 GV_Tim2PwmiFreq ;   //引用it.c中的变量，用来实现没有pwm捕获时值为0
extern double GV_Tim2PwmiDuty;//引用it.c中的变量，用来实现没有pwm捕获时值为0



void main(void)
{
 
  //录音3个按键PA1,PA2,PB4
  
  CLK_DeInit();
  CLK_HSICmd(ENABLE);
  CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);//1分频
  delay_init(16);//初始化延时函数
  GPIO_DeInit(GPIOC);
  GPIO_DeInit(GPIOB);
  GPIO_DeInit(GPIOA);
  GPIO_Init(GPIOC, GPIO_PIN_3, GPIO_MODE_OUT_PP_HIGH_FAST);//初始化高速推挽高电平
  GPIO_Init(GPIOC, GPIO_PIN_4, GPIO_MODE_OUT_PP_HIGH_FAST);//初始化高速推挽高电平
  GPIO_Init(GPIOA, GPIO_PIN_1, GPIO_MODE_OUT_PP_HIGH_FAST);//初始化高速推挽高电平
  GPIO_Init(GPIOA, GPIO_PIN_2, GPIO_MODE_OUT_PP_HIGH_FAST);//初始化高速推挽高电平
  enableInterrupts();
  TIM2_PWMI_Init(); //脉冲捕获用，TIM2 CH1
  TIM2_PWMI_Start();//脉冲捕获用, TIM2 CH1
  

  
  
  USART_Config();// 初始化uart
  mprintf("STM8s start...\r\n");

  
  /*  测试wtr芯片 */
  
   wtctrl(0xF1);   //退出自动睡眠
   delay_ms(100);  
   
  
  
  while (1)
  {
    
    
    u16 a = TIM2_PWMI_GetFreq();//获取频率
    int b = TIM2_PWMI_GetDuty()+0.4;//获取占空比,+0.4是为了获取整数

    
    delay_ms(10); 
    
    
    
    /*  下面用来处理就地按键控制   */
    
    //A1按键播放当前选定的按键
    if (GPIO_ReadInputPin(GPIOA, GPIO_PIN_1) == 0)  //SET or RESET
    {
      
      wtctrl(0xFE);   //充当唤醒功能，提前唤醒IC
      delay_ms(100);
      wtctrl(cmd[button_flag]);  //播放选择的声音
      
      while(GPIO_ReadInputPin(GPIOA, GPIO_PIN_1)==0){}
    
      
    };

    
    // B4 录音
    if(GPIO_ReadInputPin(GPIOC, GPIO_PIN_4) == 0){
      
      mprintf("luyin ing....   ! \r\n");
      wtctrl(0xFE);   //充当唤醒功能，提前唤醒IC
      delay_ms(20); 
      wtctrl(0xF5);  //一线录音开始
      //delay_ms(50);
      while(GPIO_ReadInputPin(GPIOC, GPIO_PIN_4) == 0){  }
      delay_ms(100);
      wtctrl( 0xFE);     //停止录音

    }
    
    // A2为切换下一曲
    if(GPIO_ReadInputPin(GPIOA, GPIO_PIN_2) == 0){
            
      
      
      mprintf("play luyin.....  ! \r\n");
      wtctrl(0xFE);  //充当唤醒功能，提前唤醒IC
      delay_ms(100); 
      button_flag++;
      if(button_flag>5){button_flag=0;}
      wtctrl(cmd[button_flag]);  //播放选择的声音
      
      
      
      while(GPIO_ReadInputPin(GPIOA, GPIO_PIN_2)==0){}
      

     

    }
    

    
    
    mprintf("%d \r\n",b);//打印测试
  
    
    /*  下面处理逻辑pwm捕获  */

    
    switch(b){
    
   //播放当前选定的声音
    case 6:{
      
    wtctrl(0xFE);  //充当唤醒功能，提前唤醒IC
    delay_ms(200); 
    wtctrl(cmd[button_flag]);  //播放选择的声音
    GV_Tim2PwmiFreq =0;//清除来自中断的频率采集值
    GV_Tim2PwmiDuty =0;//清除来自中断的占空比采集值
    break;
    }
      
      
    //播放录音
    case 9:{
      
    wtctrl(0xFE);  //充当唤醒功能，提前唤醒IC
    delay_ms(200); 
    wtctrl(0xF6);
    GV_Tim2PwmiFreq =0;//清除来自中断的频率采集值
    GV_Tim2PwmiDuty =0;//清除来自中断的占空比采集值
    break;
    }
    //警车鸣笛
    case 12:{
    wtctrl(0xFE);  //充当唤醒功能，提前唤醒IC
    delay_ms(200); 
    wtctrl(0x02);
    GV_Tim2PwmiFreq =0;//清除来自中断的频率采集值
    GV_Tim2PwmiDuty =0;//清除来自中断的占空比采集值
    break;
    }
    //消防车
    case 15:{
    wtctrl(0xFE);  //充当唤醒功能，提前唤醒IC
    delay_ms(200); 
    wtctrl(0x03);
    GV_Tim2PwmiFreq =0;//清除来自中断的频率采集值
    GV_Tim2PwmiDuty =0;//清除来自中断的占空比采集值
    break;
    }
    //门铃、火车
    case 18:{
    
    wtctrl(0xFE);  //充当唤醒功能，提前唤醒IC
    delay_ms(200); 
    wtctrl(0x04);
    GV_Tim2PwmiFreq =0;//清除来自中断的频率采集值
    GV_Tim2PwmiDuty =0;//清除来自中断的占空比采集值
      break;
    }
    //汽车鸣笛
    case 21:{
    wtctrl(0xFE);  //充当唤醒功能，提前唤醒IC
    delay_ms(200); 
    wtctrl(0x05);
    GV_Tim2PwmiFreq =0;//清除来自中断的频率采集值
    GV_Tim2PwmiDuty =0;//清除来自中断的占空比采集值
      break;
    
    }
    //电话铃声
    case 24:{
    wtctrl(0xFE);  //充当唤醒功能，提前唤醒IC
    delay_ms(200); 
    wtctrl(0x01);
    GV_Tim2PwmiFreq =0;//清除来自中断的频率采集值
    GV_Tim2PwmiDuty =0;//清除来自中断的占空比采集值
    
    
    break;
    }
      
      
      
      
    
    
    }
    
    
    
    
    
    
    
    
   
    
    
    
    



    
   /* 
    //输出呼吸信号
    for(int i=0;i<1000;i++)
    {
      
        TIM1_SetCompare3(i);//设置占空比
        delay_ms(1);
        
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
 GPIO_WriteLow(GPIOC, GPIO_PIN_3);
 delay_ms(5);      //延时5ms
 B_DATA = S_DATA&0x01;
 
 for(j=0;j<8;j++)
 {
  if(B_DATA == 1)
  {
   GPIO_WriteHigh(GPIOC, GPIO_PIN_3);
   delay_us(900);    //延时600us
   GPIO_WriteLow(GPIOC, GPIO_PIN_3);
   delay_us(300);   //延时200us
  }
  else
  {
   GPIO_WriteHigh(GPIOC, GPIO_PIN_3);
   delay_us(300);  //延时200us
   GPIO_WriteLow(GPIOC, GPIO_PIN_3);
   delay_us(900);  //延时600us  
  }
  S_DATA = S_DATA>>1;   // 右移一位
  B_DATA = S_DATA&0x01; // 取出最高位
 } 
 GPIO_WriteHigh(GPIOC, GPIO_PIN_3);//控制完成拉高信号线
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
