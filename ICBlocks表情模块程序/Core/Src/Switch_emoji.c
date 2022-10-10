#include "Switch_emoji.h"
#include "stm32f1xx_hal.h"
#include "main.h"
#include "oled.h"
#include "flash.h"
#include "tim.h"
extern uint8_t MODE;
uint8_t KEY0,KEY1;
int8_t emoji_num,num_data;
uint8_t  KEY_DOWN,KEY_DOWN1=0;

#define Face_Delay1 500
#define Face_Delay2 1000
#define Face_Delay3 1500


void Read_KEY()
{
    static uint8_t key_up=1;     //按键松开标志
    KEY0 = HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_7);
    if(MODE==1)  //表情
    {
        if(key_up&&KEY0==0)
        {
            HAL_Delay(20);
            KEY_DOWN=1;
            if(key_up&&KEY0==0)
            {
                OLED_CLS();
                emoji_num++;
                key_up=0;
                if(emoji_num>5)
                {
                    emoji_num=0;
                }
            }
        }
        else if(KEY0==1)
        {
            key_up=1;
        }
    }
    else     //数字
    {
        if(key_up&&KEY0==0)
        {
            HAL_Delay(20);
            KEY_DOWN=1;
            if(key_up&&KEY0==0)
            {
                num_data++;
                key_up=0;
                if(num_data>99)
                {
                    OLED_CLS();
                    num_data=0;
                }
            }
        }
        else if(KEY0==1)
        {
            key_up=1;
        }
    }
}

void num_display()
{
    if(num_data<10)
    {

        OLED_ShowNum(47,0,num_data,1,16);
    }
    else
    {
        OLED_ShowNum(30,0,num_data,2,16);
    }
}
uint16_t iii;
void emoji_display()
{
    if(time_up>Face_Delay3)
    {
        time_up=0;
    }
    switch (emoji_num)
    {
    case 0:
        HAL_TIM_Base_Start_IT(&htim3);
        if(time_up<Face_Delay1)
        {
            OLED_DrawBMP(0,1,127,8,BMP1);
        }
        else if(time_up<Face_Delay2)
        {
            OLED_DrawBMP(0,1,127,8,BMP2);
        }
        else if(time_up<Face_Delay3)
        {
            OLED_DrawBMP(0,1,127,8,BMP3);
        }
        else
        {
            HAL_TIM_Base_Stop_IT(&htim3);
            time_up=0;
        }
        break ;
    case 1:
        HAL_TIM_Base_Start_IT(&htim3);
        if(time_up<Face_Delay1)
        {
            OLED_DrawBMP(0,1,127,8,BMP4);
        }
        else if(time_up<Face_Delay2)
        {
            OLED_DrawBMP(0,1,127,8,BMP5);
        }
        else if(time_up<Face_Delay3)
        {
            OLED_DrawBMP(0,1,127,8,BMP6);
        }
        else
        {
            HAL_TIM_Base_Stop_IT(&htim3);
            time_up=0;
        }
        break ;
    case 2:
        HAL_TIM_Base_Start_IT(&htim3);
        if(time_up<Face_Delay1)
        {
            OLED_DrawBMP(0,1,127,8,BMP7);
        }
//        else if(time_up<Face_Delay2)
//        {
//            OLED_DrawBMP(0,1,127,8,BMP8);
//        }
        else if(time_up<Face_Delay2)
        {
            OLED_DrawBMP(0,1,127,8,BMP9);
        }
        else
        {
            HAL_TIM_Base_Stop_IT(&htim3);
            time_up=0;
        }
        break ;
    case 3:
        HAL_TIM_Base_Start_IT(&htim3);
        if(time_up<Face_Delay1)
        {
            OLED_DrawBMP(0,1,127,8,BMP10);
        }
        else if(time_up<Face_Delay2)
        {
            OLED_DrawBMP(0,1,127,8,BMP11);
        }
        else if(time_up<Face_Delay3)
        {
            OLED_DrawBMP(0,1,127,8,BMP12);
        }
        else
        {
            HAL_TIM_Base_Stop_IT(&htim3);
            time_up=0;
        }
        break ;
    case 4:
        HAL_TIM_Base_Start_IT(&htim3);
        if(time_up<Face_Delay1)
        {
            OLED_DrawBMP(0,1,127,8,BMP13);
        }
        else if(time_up<Face_Delay2)
        {
            OLED_DrawBMP(0,1,127,8,BMP14);
        }
        else if(time_up<Face_Delay3)
        {
            OLED_DrawBMP(0,1,127,8,BMP15);
        }
        else
        {
            HAL_TIM_Base_Stop_IT(&htim3);
            time_up=0;
        }
        break ;
    case 5:
        HAL_TIM_Base_Start_IT(&htim3);
        if(time_up<Face_Delay1)
        {
            OLED_DrawBMP(0,1,127,8,BMP16);
        }
        else if(time_up<Face_Delay2)
        {
            OLED_DrawBMP(0,1,127,8,BMP17);
        }
        else if(time_up<Face_Delay3)
        {
            OLED_DrawBMP(0,1,127,8,BMP18);
        }
        else
        {
            HAL_TIM_Base_Stop_IT(&htim3);
            time_up=0;
        }
        break ;

    default:
        break ;

    }

}




