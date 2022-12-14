#include "Data_Analysis.h"
#include "Prestore_Analysis.h"
#include "main.h"
#include "adc.h"
#include "tim.h"
#include "gpio.h"
#include "Bluetooth.h"

uint32_t On_Off_Time;
#define Set_On_Off_Time  5000
#define Set_LED_Time1  5
#define Set_LED_Time2  10
#define LED_times 3  //LED闪烁次数LED_times+1
uint8_t Set_LED_Num =3;

uint8_t Interface[99]= {0}; //8个接口
uint8_t Location;
uint8_t turn_onn;
uint8_t bbb;
/*******************************************/
uint16_t Loop_number_Times[5]= {0}; //第n个循环次数
uint8_t Loop_number_Start[5]= {0}; //第n个循环开始位置
uint8_t Loop_number_Stop[5]= {0};  //第n个循环结束位置
uint16_t Loop_Times=0;
uint8_t Loop_Start=0,Loop_Stop=0;//循环次数/开始位置/结束位置
uint8_t Sort_Order=0;            //第几个循环块
uint8_t Order_Residue=0;        //剩余几个循环块
uint8_t Order[4]= {0};
/******************确定模块顺序***********************/
uint16_t Shutdown=0;  //自动关机标志

uint8_t KEY=0;
uint8_t key_running=0;
uint8_t sport=0;
uint8_t sport1=0;


void Interface_module(void)
{
    for(uint8_t i=0; i<8; i++)
    {
        if(ADC_Value[i]<25)                         //光电模块
        {
            Interface[i]=31;    ////模块11......
        }
        else if((26<=ADC_Value[i])&&(ADC_Value[i]<125))    //光线模块
        {
            Interface[i]=33;    ////模块11......
        }
        else if((125<=ADC_Value[i])&&(ADC_Value[i]<220))//声音模块
        {
            Interface[i]=34;    ////模块11......
        }
        else if((220<=ADC_Value[i])&&(ADC_Value[i]<300))//按钮模块
        {
            Interface[i]=35;////模块11......
        }
        else if((300<=ADC_Value[i])&&(ADC_Value[i]<445))//倾斜模块
        {
            Interface[i]=36;    ////模块11......
        }
        else if((445<=ADC_Value[i])&&(ADC_Value[i]<545)) //旋钮模块
        {
            Interface[i]=32;    ////模块11......
        }
        else if((545<=ADC_Value[i])&&(ADC_Value[i]<640)) //运动模块
        {
            Interface[i]=24;    ////模块11......
        }
        else if((640<=ADC_Value[i])&&(ADC_Value[i]<730))//左转90度/
        {
            Interface[i]=19;    ////模块11......
        }
        else if((730<=ADC_Value[i])&&(ADC_Value[i]<850))//右转90度
        {
            Interface[i]=17;    ////模块11......
        }
        else if((850<=ADC_Value[i])&&(ADC_Value[i]<1010))//后退一步
        {
            Interface[i]=16;    ////模块11......
        }
        else if((1010<=ADC_Value[i])&&(ADC_Value[i]<1100))//前进一步
        {
            Interface[i]=21;    ////模块11......
        }
        else if((1100<=ADC_Value[i])&&(ADC_Value[i]<1190))//左转直到
        {
            Interface[i]=20;    ////模块11......
        }
        else if((1190<=ADC_Value[i])&&(ADC_Value[i]<1275))//右转直到
        {
            Interface[i]=18;    ////模块11......
        }
        else if((1275<=ADC_Value[i])&&(ADC_Value[i]<1360))//前进直到
        {
            Interface[i]=22;    ////模块11......
        }
        else if((1360<=ADC_Value[i])&&(ADC_Value[i]<1475))//停止模块
        {
            Interface[i]=23;    ////模块11......
        }
        else if((1475<=ADC_Value[i])&&(ADC_Value[i]<1575))//微笑表情
        {
            Interface[i]=3;
        }
        else if((1575<=ADC_Value[i])&&(ADC_Value[i]<1655))//生气表情
        {
            Interface[i]=4;
        }
        else if((1655<=ADC_Value[i])&&(ADC_Value[i]<1745))//伤心表情
        {
            Interface[i]=5;
        }
        else if((1745<=ADC_Value[i])&&(ADC_Value[i]<1900))//开心表情
        {
            Interface[i]=1;    //模块1
        }
        else if((1900<=ADC_Value[i])&&(ADC_Value[i]<2010))//哭泣表情
        {
            Interface[i]=2;  //模块2......
        }
        else if((2010<=ADC_Value[i])&&(ADC_Value[i]<2100))//警车鸣笛
        {
            Interface[i]=12;    ////模块11......
        }
        else if((2100<=ADC_Value[i])&&(ADC_Value[i]<2200))//消防车鸣笛
        {
            Interface[i]=13;    ////模块11......
        }
        else if((2200<=ADC_Value[i])&&(ADC_Value[i]<2300))//火车鸣笛
        {
            Interface[i]=15;    ////模块11......
        }
        else if((2300<=ADC_Value[i])&&(ADC_Value[i]<2400))//汽车鸣笛
        {
            Interface[i]=14;    ////模块11......
        }
        else if((2400<=ADC_Value[i])&&(ADC_Value[i]<2490))//电话铃声
        {
            Interface[i]=11;    ////模块11......
        }
        else if((2490<=ADC_Value[i])&&(ADC_Value[i]<2580))//播放录音
        {
            Interface[i]=6;
        }
        else if((2580<=ADC_Value[i])&&(ADC_Value[i]<2675))//开灯
        {
            Interface[i]=7;//Interface[i]=7;
        }
        else if((2675<=ADC_Value[i])&&(ADC_Value[i]<2790))//关灯
        {
            Interface[i]=8;//Interface[i]=8;
        }
        else if((2790<=ADC_Value[i])&&(ADC_Value[i]<2950))//启动电机
        {
            Interface[i]=9;
        }
        else if((2950<=ADC_Value[i])&&(ADC_Value[i]<3120))//停止电机
        {
            Interface[i]=10;
        }
        else if((3120<=ADC_Value[i])&&(ADC_Value[i]<3300))  //前进2步
        {
            Interface[i]=25;    ////模块11......
        }

        /*****************测试块*********************/
//        else if((3300<ADC_Value[i])&&(ADC_Value[i]<3430))//测试模块
//        {
//            Interface[i]=99;    ////模块11......
//        }
        /*****************循环块*********************/

        else if((3300<ADC_Value[i])&&(ADC_Value[i]<3400))//循环1次模块
        {
            Interface[i]=26;
        }
        else if((3410<ADC_Value[i])&&(ADC_Value[i]<3530))//循环2次模块
        {
            Interface[i]=27;
        }
        else if((3530<ADC_Value[i])&&(ADC_Value[i]<3630))//循环3次模块
        {
            Interface[i]=28;
        }
        else if((3630<ADC_Value[i])&&(ADC_Value[i]<3730))//循环多次模块
        {
            Interface[i]=29;
        }
        else if((3730<=ADC_Value[i])&&(ADC_Value[i]<3829))  //预存模块
        {
            Interface[i]=37;
        }
        else if((3830<=ADC_Value[i])&&(ADC_Value[i]<3929))  //读取模块
        {
            Interface[i]=38;    ////模块11......
        }
        else  //没有模块
        {
            Interface[i]=0;
        }
    }
}
void Circular_Order()   //循环顺序
{
    for(uint8_t i=0; i<8; i++)
    {
        if(Interface[i]==26)  //循环一次
        {
            Sort_Order+=1;
            Loop_number_Times[Sort_Order]=1;  //第n个循环次数
            if(Sort_Order>1)
            {
                Loop_number_Start[Sort_Order]=Loop_number_Stop[Sort_Order-1]+2;  //第n个循环开始位置
            }
            else
            {
                Loop_number_Start[Sort_Order]=0;  //第n个循环开始位置
            }
            if(i>0)
            {
                Loop_number_Stop[Sort_Order]=i-1;   //第n个循环结束位置
            }
            else
            {
                Loop_number_Stop[Sort_Order]=i;   //第n个循环结束位置
            }
        }
        else if(Interface[i]==27)  //循环两次
        {
            Sort_Order+=1;
            Loop_number_Times[Sort_Order]=2;  //第n个循环次数
            if(Sort_Order>1)
            {
                Loop_number_Start[Sort_Order]=Loop_number_Stop[Sort_Order-1]+2;  //第n个循环开始位置
            }
            else
            {
                Loop_number_Start[Sort_Order]=0;  //第n个循环开始位置
            }
            if(i>0)
            {
                Loop_number_Stop[Sort_Order]=i-1;   //第n个循环结束位置
            }
            else
            {
                Loop_number_Stop[Sort_Order]=i;   //第n个循环结束位置
            }
        }
        else if(Interface[i]==28)  //循环三次
        {
            Sort_Order+=1;
            Loop_number_Times[Sort_Order]=3;  //第n个循环次数
            if(Sort_Order>1)
            {
                Loop_number_Start[Sort_Order]=Loop_number_Stop[Sort_Order-1]+2;  //第n个循环开始位置
            }
            else
            {
                Loop_number_Start[Sort_Order]=0;  //第n个循环开始位置
            }
            if(i>0)
            {
                Loop_number_Stop[Sort_Order]=i-1;   //第n个循环结束位置
            }
            else
            {
                Loop_number_Stop[Sort_Order]=i;   //第n个循环结束位置
            }
        }
        else if(Interface[i]==29)
        {
            Sort_Order+=1;
            Loop_number_Times[Sort_Order]=65530;  //第n个循环次数
            if(Sort_Order>1)
            {
                Loop_number_Start[Sort_Order]=Loop_number_Stop[Sort_Order-1]+2;  //第n个循环开始位置
            }
            else
            {
                Loop_number_Start[Sort_Order]=0;  //第n个循环开始位置
            }
            if(i>0)
            {
                Loop_number_Stop[Sort_Order]=i-1;   //第n个循环结束位置
            }
            else
            {
                Loop_number_Stop[Sort_Order]=i;   //第n个循环结束位置
            }
        }

    }
}
uint8_t Loop_one(uint16_t Loop_Times,uint8_t Loop_Start,uint8_t Loop_Stop)   //一个完整的循环
{
    uint8_t Motor=0;
    uint8_t S_Movement=0,R_Movement=0,L_Movement=0;
    uint8_t wait_ing=0;
    uint8_t Potentiometer=0;
    uint8_t Tilt_return=0;
    for(uint16_t t=0; t<Loop_Times; t++)            //循环次数
    {
        if(Loop_Times>6100)//死循环
        {
            t=0;
        }

        for(uint8_t y=Loop_Start; y<=Loop_Stop; y++)  //模块循环开始/结束
        {
            if(t<Loop_Times)
            {
                IORD_Running(y);  //读取指令运行

                /**表情模块**/
                if(emoji_Module(y)==1)
                {
                    for(y=Loop_Start; y<=Loop_Stop+1; y++)
                    {
                        GPIO_RESET(y);
                    }
                    return 1;
                }
                if(Interface[y]==38)
                {
                    IORD(y);
                }

                /********直到模块**************/
                R_Movement=Right_Movement(y);
                if( R_Movement==1)
                {

                }
                if(R_Movement==2)
                {
                    for(y=Loop_Start; y<=Loop_Stop+1; y++)
                    {
                        GPIO_RESET(y);
                    }
                    return 1;
                }
                L_Movement=Left_Movement(y);
                if( L_Movement==1)
                {

                }
                if(L_Movement==2)
                {
                    for(y=Loop_Start; y<=Loop_Stop+1; y++)
                    {
                        GPIO_RESET(y);
                    }
                    return 1;
                }
                S_Movement=straight_Movement(y);
                if( S_Movement==1)
                {

                }
                if(S_Movement==2)
                {
                    for(y=Loop_Start; y<=Loop_Stop+1; y++)
                    {
                        GPIO_RESET(y);
                    }
                    return 1;
                }

                /**电位器+启动电机***/
                Motor=Start_Motor_Potentiometer(y);
                if(Motor==1)
                {
                    LED_Flashing(y+1,0);
                }
                if(Motor==2)//按启动按键退出循环
                {
                    for(y=Loop_Start; y<=Loop_Stop+1; y++)
                    {
                        GPIO_RESET(y);
                    }
                    return 1;
                }
                /**电位器+运动模块***/
                Potentiometer=Start_Potentiometer(y);
                if(Potentiometer==1)
                {

                }
                if(Potentiometer==2)//按启动按键退出循环
                {
                    for(y=Loop_Start; y<=Loop_Stop+1; y++)
                    {
                        GPIO_RESET(y);
                    }
                    return 1;
                }
                /**倾斜模块+运动模块***/
                Tilt_return=Tilt_Movement(y);//确定返回值
                if(Tilt_return==1)//上下左右动一次循环一次
                {
//                    for(y=Loop_Start; y<=Loop_Stop+1; y++)
//                    {
//                        GPIO_RESET(y);
//                    }
//                    break;  //退出一次循环次数加一  不用return
                }
                else if(Tilt_return==2)//按启动按键退出循环
                {
                    for(y=Loop_Start; y<=Loop_Stop+1; y++)
                    {
                        GPIO_RESET(y);
                    }
                    return 1;
                }
                /**等待**/
                wait_ing=waitting (y);
                if( wait_ing==2)
                {
                    for(y=Loop_Start; y<=Loop_Stop+1; y++)
                    {
                        GPIO_RESET(y);
                    }
                    return 1;
                }
                /**声音**/
                if(voice_Module(y)==1)
                {
                    for(y=Loop_Start; y<=Loop_Stop+1; y++)
                    {
                        GPIO_RESET(y);
                    }
                    return 1;
                }

                /**其他**/
                if(other_Module(y)==1)
                {
                    for(y=Loop_Start; y<=Loop_Stop+1; y++)
                    {
                        GPIO_RESET(y);
                    }
                    return 1;
                }
//            waitting (y);
            }
        }
    }
    GPIO_RESET(Loop_Stop+1);
    return 0;
}

/**循环+模块**/

void Special_Combination_Module(void)//没有循环模块的情况
{
    for(uint8_t i=0; i<8; i++)
    {
        if(i>=1)
        {
            for(uint8_t k; k<i; k++)
            {
                GPIO_RESET(k);
            }
        }
//        IORD(i);
        IORD_Running(i);  //读取指令运行
        /**表情模块**/
        emoji_Module(i);
        /**电位器+启动电机***/
        Start_Motor_Potentiometer(i);
        /**电位器+运动模块***/
        Start_Potentiometer(i);
        /**倾斜模块+运动模块***/
        Tilt_Movement(i);
        /**声音**/
        voice_Module(i);
        /**其他**/
        waitting (i);
        other_Module(i);
        straight_Movement(i);
        Right_Movement(i);
        Left_Movement(i);
    }

    Sendbuff[0]= 0;
    Sendbuff1[0]=0;
    /*************/
    for(uint8_t i=0; i<8; i++)
    {
        ID_GPIO_OUTPUT_Init(i);
        GPIO_RESET(i);
    }
}
void Loop_Movement(void)//需要循环个数/循环次数/开始位置/结束位置
{
    if(Sort_Order>0) //有外循环模块的情况
    {
        Order_Residue=Sort_Order+1;
        for(uint8_t w=1; w<=Sort_Order; w++)    //有几个循环模块4
        {
            Order_Residue--;
            Loop_Times=Loop_number_Times[w];//第几个循环/循环次数
            Loop_Start=Loop_number_Start[w];//第几个循环/开始位置
            Loop_Stop=Loop_number_Stop[w];  //第几个循环/结束位置
            /**/

            if(Loop_Start>=1)
            {
                for(uint8_t k=0; k<Loop_Start; k++)
                {
                    GPIO_RESET(k);
                }
            }
            /**/
            Loop_one(Loop_Times, Loop_Start, Loop_Stop);
        }
        for(uint8_t y=Loop_Stop+2; y<8; y++)  //模块循环开始结束
        {
            /**表情模块**/
            emoji_Module(y);
            /**电位器+启动电机***/
            Start_Motor_Potentiometer(y);
            /**电位器+运动模块***/
            Start_Potentiometer(y);
            /**倾斜模块+运动模块***/
            Tilt_Movement(y);
            /**声音**/
            voice_Module(y);
            /**其他**/

            waitting (y);
            other_Module(y);
            straight_Movement(y);
            Right_Movement(y);
            Left_Movement(y);
        }
        Sort_Order=0;//循环模块个数清零
        Loop_Times=0;//循环次数清零
        Loop_Start=0;//循环开始清零
        Loop_Stop=0;

    }
    else
    {
        Special_Combination_Module();  //无外循环的情况
    }
    sport=0;
    sport1=0;
}
void IORD_Judge()
{

}
/**旋钮模块+启动电机模块***/

uint8_t Start_Motor_Potentiometer(uint8_t i)
{
    if(Interface[i]==32)  //旋钮模块
    {
        LED_Flashing(i+1,0);
        for(uint8_t y=0; y<i; y++)
        {
            GPIO_RESET(y);
        }
        if(Interface[i+1]==9&&Interface[i+2]==29)  //必须是启动电机模块
        {
            sport1=1;
            LED_Flashing_Time=0;
            while(KEY_Scan()!=1)
            {
                //nterface[i]==2的LED灯闪烁
                LED_Flashing(i+1,0);
                /*****ADC值位置+8******/
                if(ADC_Value[i+8]<600)
                {   sport1=1;
                    Sendbuff[0]= 42;
                    send_data_time();
                    LED_Flashing(i+1,0);
//                    GPIO_RESET(i+1);//亮		Interface[i]==2的LED灯
//                    GPIO_RESET(i+1);//亮		Interface[i]==2的LED灯
                    return 1;
                }
                else if((ADC_Value[i+8]>=600)&&(ADC_Value[i+8]<1200))
                {   sport1=1;
                    Sendbuff[0]= 41;
                    send_data_time();
                    LED_Flashing(i+1,0);
//                    GPIO_RESET(i+1);//亮		Interface[i]==2的LED灯
//                    GPIO_RESET(i+1);//亮		Interface[i]==2的LED灯
                    return 1;
                }
                else if((ADC_Value[i+8]>=1200)&&(ADC_Value[i+8]<1800))
                {   sport1=1;
                    Sendbuff[0]= 40;
                    send_data_time();
                    LED_Flashing(i+1,0);
//                    GPIO_RESET(i+1);//亮		Interface[i]==2的LED灯
//                    GPIO_RESET(i+1);//亮		Interface[i]==2的LED灯
                    return 1;
                }
                else if((ADC_Value[i+8]>=1800)&&(ADC_Value[i+8]<2300))
                {   sport1=1;
                    Sendbuff[0]= 35;//电机停止
                    send_data_time();
                    LED_Flashing(i+1,0);
//                    GPIO_RESET(i+1);//亮		Interface[i]==2的LED灯
//                    GPIO_RESET(i+1);//亮		Interface[i]==2的LED灯
                    return 1;
                }
                else if((ADC_Value[i+8]>=2300)&&(ADC_Value[i+8]<2900))
                {   sport1=1;
                    Sendbuff[0]= 43;
                    send_data_time();
                    LED_Flashing(i+1,0);
//                    GPIO_RESET(i+1);//亮		Interface[i]==2的LED灯
//                    GPIO_RESET(i+1);//亮		Interface[i]==2的LED灯
                    return 1;
                }
                else if((ADC_Value[i+8]>=2900)&&(ADC_Value[i+8]<3500))
                {   sport1=1;
                    Sendbuff[0]= 44;
                    send_data_time();
                    LED_Flashing(i+1,0);
//                    GPIO_RESET(i+1);//亮		Interface[i]==2的LED灯
//                    GPIO_RESET(i+1);//亮		Interface[i]==2的LED灯
                    return 1;
                }
                else if(ADC_Value[i+8]>=3500)
                {   sport1=1;
                    Sendbuff[0]= 45;
                    send_data_time();
                    GPIO_RESET(i+1);//亮		Interface[i]==2的LED灯
                    GPIO_RESET(i+1);//亮		Interface[i]==2的LED灯
                    return 1;
                }
            }
            Set_LED_Num=LED_times;
            GPIO_RESET(i+1);//亮		Interface[i]==2的LED灯
            GPIO_RESET(i+1);//亮		Interface[i]==2的LED灯
            GPIO_RESET(i+1);//亮		Interface[i]==2的LED灯
            return 2;
        }
        else if(Interface[i+1]==24&&Interface[i+2]==29)
        {

        }
        else //等待模式
        {
            for(uint8_t y=i; y<8; y++)
            {
                GPIO_SET(y);
            }
            while(KEY_Scan()!=1)
            {
                if(ADC_Value[i+8]<2048)
                {
                    GPIO_RESET(i);
                    break;
                }
            }
        }
    }
    if(sport1==0)
    {
        if(Interface[i]==9)   //启动电机
        {
            LED_Flashing_Time=0;
            Sendbuff[0]= 34;
            send_data_time();
            Set_LED_Num=LED_times;
            Turn_OFF_LED(i);
            while(Set_LED_Num)
            {
                LED_Flashing(i,Set_LED_Num);
                if(KEY_Scan()==1)  //按键跳出
                {
                    Set_LED_Num=LED_times;
                    GPIO_RESET(i);
                    return 1;
                }
            }
            Set_LED_Num=LED_times;
            GPIO_RESET(i);
        }
    }
    return 0;
}
/**旋钮模块+运动模块***/

uint8_t Start_Potentiometer(uint8_t i)
{
    if(Interface[i]==32)  //旋钮模块
    {
        for(uint8_t y=0; y<i; y++)
        {
            GPIO_RESET(y);
        }
        if(Interface[i+1]==24&&Interface[i+2]==29)  //必须是运动模块
        {
            sport=1;
            LED_Flashing_Time=0;
            while(KEY_Scan()!=1)
            {
                //nterface[i]==2的LED灯闪烁
                LED_Flashing(i+1,0);
                /*****ADC值位置+8******/
                if(ADC_Value[i+8]<600)
                {
                    sport=1;
                    Sendbuff[0]= 48;
                    send_data_time();
                    GPIO_RESET(i+1);//亮		Interface[i]==2的LED灯
                    GPIO_RESET(i+1);//亮		Interface[i]==2的LED灯
                    return 1;
                }
                else if((ADC_Value[i+8]>=600)&&(ADC_Value[i+8]<1200))
                {
                    sport=1;
                    Sendbuff[0]= 47;
                    send_data_time();
                    GPIO_RESET(i+1);//亮		Interface[i]==2的LED灯
                    GPIO_RESET(i+1);//亮		Interface[i]==2的LED灯
                    return 1;
                }
                else if((ADC_Value[i+8]>=1200)&&(ADC_Value[i+8]<1800))
                {
                    sport=1;
                    Sendbuff[0]= 46;
                    send_data_time();
                    GPIO_RESET(i+1);//亮		Interface[i]==2的LED灯
                    GPIO_RESET(i+1);//亮		Interface[i]==2的LED灯
                    return 1;
                }
                else if((ADC_Value[i+8]>=1800)&&(ADC_Value[i+8]<2300))
                {
                    sport=1;
                    Sendbuff[0]= 20;//小车停止
                    send_data_time();
                }
                else if((ADC_Value[i+8]>=2300)&&(ADC_Value[i+8]<2900))
                {
                    sport=1;
                    Sendbuff[0]= 49;
                    send_data_time();
                    GPIO_RESET(i+1);//亮		Interface[i]==2的LED灯
                    GPIO_RESET(i+1);//亮		Interface[i]==2的LED灯
                    return 1;
                }
                else if((ADC_Value[i+8]>=2900)&&(ADC_Value[i+8]<3500))
                {
                    sport=1;
                    Sendbuff[0]= 50;
                    send_data_time();
                    GPIO_RESET(i+1);//亮		Interface[i]==2的LED灯
                    GPIO_RESET(i+1);//亮		Interface[i]==2的LED灯
                    return 1;
                }
                else if(ADC_Value[i+8]>=3500)
                {
                    sport=1;
                    Sendbuff[0]= 51;
                    send_data_time();
                    GPIO_RESET(i+1);//亮		Interface[i]==2的LED灯
                    GPIO_RESET(i+1);//亮		Interface[i]==2的LED灯
                    return 1;
                }
            }
            Set_LED_Num=LED_times;
            GPIO_RESET(i+1);//亮		Interface[i]==2的LED灯
            GPIO_RESET(i+1);//亮		Interface[i]==2的LED灯
            GPIO_RESET(i+1);//亮		Interface[i]==2的LED灯
            return 2;
        }
    }
    /********************/
    if(sport==0)
    {
        if(Interface[i]==24)  //必须是运动模块)
        {
            LED_Flashing_Time=0;
//            Sendbuff[0]= 47;
					                Sendbuff[0]= 19;

            send_data_time();
            Set_LED_Num=LED_times;
            Turn_OFF_LED(i);
            while(Set_LED_Num)
            {
                LED_Flashing(i,Set_LED_Num);
                if(KEY_Scan()==1)  //按键跳出
                {
                    Set_LED_Num=LED_times;
                    GPIO_RESET(i);
                    return 1;
                }
            }
            Set_LED_Num=LED_times;
            GPIO_RESET(i);
        }
    }
    return 0;
}
/**倾斜模块+运动模块**/
uint8_t Tilt_Movement(uint8_t i)
{
    if((Interface[i]==36)||((290<=ADC_Value[i])&&(ADC_Value[i]<420)))  //倾斜模块
    {
        for(uint8_t y=i; y<8; y++)
        {
            GPIO_SET(y);
        }
        if(Interface[i+1]==24&&Interface[i+2]==29)  //必须是运动模块
        {   sport=1;
            LED_Flashing_Time=0;
            while(KEY_Scan()!=1)
            {
                if(ADC_Value[i+8]>4090)  //?
                {
                    high_time[i]=0;
                }
                LED_Flashing(i+1,0);

                Location=i+1;
                Gyro_Module();
                /*****PWM值位置+1******/
                if(high_time[i]==600)//左
                {   sport=1;
                    Sendbuff[0]= 17;
                    send_data_time();
                    GPIO_RESET(i+1);//亮		Interface[i]==2的LED灯
                    GPIO_RESET(i+1);//亮		Interface[i]==2的LED灯
                    return 1;
                }
                else if(high_time[i]==700)//右
                {   sport=1;
                    Sendbuff[0]= 18;
                    send_data_time();
                    GPIO_RESET(i+1);//亮		Interface[i]==2的LED灯
                    GPIO_RESET(i+1);//亮		Interface[i]==2的LED灯
                    return 1;
                }
                else if(high_time[i]==800)//直行
                {   sport=1;
                    Sendbuff[0]= 19;
                    send_data_time();
                    GPIO_RESET(i+1);//亮		Interface[i]==2的LED灯
                    GPIO_RESET(i+1);//亮		Interface[i]==2的LED灯
                    return 1;
                }
                else if(high_time[i]==900)//后
                {   sport=1;
                    Sendbuff[0]= 37;
                    send_data_time();
                    GPIO_RESET(i+1);//亮		Interface[i]==2的LED灯
                    GPIO_RESET(i+1);//亮		Interface[i]==2的LED灯
                    return 1;
                }
                else if(high_time[i]==0)
                {   sport=1;
                    Sendbuff[0]= 20; //小车停
                    send_data_time();
                }
            }
            GPIO_RESET(i+1);//亮		Interface[i]==2的LED灯
            GPIO_RESET(i+1);//亮		Interface[i]==2的LED灯
            GPIO_RESET(i+1);//亮		Interface[i]==2的LED灯
            return 2;
        }
        else  //等待模式
        {
            LED_Flashing_Time=0;
            LED_Flashing(i,0);
            while(KEY_Scan()!=1)
            {
                Location=i+1;
                Gyro_Module();

                if(high_time[i]==600)//左
                {
                    high_time[i]=0;
                    break;
                }
                else if(high_time[i]==700)//右
                {
                    high_time[i]=0;
                    break;
                }
                else if(high_time[i]==800)//直行
                {
                    high_time[i]=0;
                    break;
                }
                else if(high_time[i]==900)//后
                {
                    high_time[i]=0;
                    break;
                }
            }
        }
    }
    return 0;
}

/**等待模块****//**指令模块+运动模块**/
uint8_t waitting (uint8_t i)
{
    uint8_t temp=0;//判断是退出循环还是退出下一步
    if((Interface[i]==31)||(Interface[i]==33)||(Interface[i]==34)||(Interface[i]==35))  //指令模块光电、光线、声音、按钮||(Interface[i]==32)旋钮
    {
        for(uint8_t y=i; y<8; y++)
        {
            GPIO_SET(y);
        }
        if(Interface[i+1]==24)  //必须是运动模块)
        {
            GPIO_SET(i+1);
            LED_Flashing_Time=0;
            while(KEY_Scan()!=1)
            {
                for(uint8_t t=2; t+i<8; t++)
                {
                    GPIO_SET(i+t);
                }
                LED_Flashing(i,0);
                if(ADC_Value[i+8]<2048)
                {
                    Sendbuff[0]= 19;
                    send_data_time();
                    GPIO_RESET(i+1);
                    return 1;
                }
                else
                {
                    Sendbuff[0]= 20; //小车停
                    send_data_time();
                }
            }
            return 2;
        }
        else if(Interface[i+1]==9)  //必须是启动电机模块)
        {
            GPIO_SET(i+1);
            LED_Flashing_Time=0;
            while(KEY_Scan()!=1)
            {
                for(uint8_t t=2; t+i<8; t++)
                {
                    GPIO_SET(i+t);
                }
                LED_Flashing(i,0);
                if(ADC_Value[i+8]<2048)
                {
                    Sendbuff[0]= 42;
                    send_data_time();
                    GPIO_RESET(i+1);
                    return 1;
                }
                else
                {
                    Sendbuff[0]= 35; //小车停
                    send_data_time();
                }
            }
            return 2;
        }
        else  //等待模式
        {
            for(uint8_t y=i; y<8; y++)
            {
                GPIO_SET(y);
            }
            if(Order_Residue>0)
            {
                while(KEY_Scan()!=1)
                {
                    temp=1;
                    if(ADC_Value[i+8]<2048)
                    {
                        temp=0;
                        GPIO_RESET(i);
                        break;
                    }
                }
                if(temp==1)
                {
                    temp=0;
                    return 2;
                }
            }
            else
            {
                while(KEY_Scan()!=1)
                {
                    if(ADC_Value[i+8]<2048)
                    {
                        GPIO_RESET(i);
                        break;
                    }
                }
            }
        }
    }
    return 0;
}
/**表情模块**/
uint8_t emoji_Module(uint8_t i)
{
    if(Interface[i]==1)   //开心表情
    {
        LED_Flashing_Time=0;
        Sendbuff[0]= 24;
        send_data_time();
        Set_LED_Num=7;    //7秒
        Turn_OFF_LED(i);

        while(Set_LED_Num)
        {
            LED_Flashing(i,Set_LED_Num);
            if(KEY_Scan()==1)  //按键跳出
            {
                Set_LED_Num=LED_times;
                GPIO_RESET(i);
                return 1;
            }
        }
        Set_LED_Num=LED_times;
        GPIO_RESET(i);
    }
    else if(Interface[i]==2)   //哭泣表情
    {
        LED_Flashing_Time=0;
        Sendbuff[0]= 25;
        send_data_time();
        Set_LED_Num=7;    //7秒
        Turn_OFF_LED(i);

        while(Set_LED_Num)
        {
            LED_Flashing(i,Set_LED_Num);
            if(KEY_Scan()==1)  //按键跳出
            {
                Set_LED_Num=LED_times;
                GPIO_RESET(i);
                return 1;
            }
        }
        Set_LED_Num=LED_times;
        GPIO_RESET(i);
    }
    else if(Interface[i]==3)   //微笑表情
    {
        LED_Flashing_Time=0;
        Sendbuff[0]= 21;
        send_data_time();
        Set_LED_Num=7;    //7秒
        Turn_OFF_LED(i);

        while(Set_LED_Num)
        {
            LED_Flashing(i,Set_LED_Num);
            if(KEY_Scan()==1)  //按键跳出
            {
                Set_LED_Num=LED_times;
                GPIO_RESET(i);
                return 1;
            }
        }
        Set_LED_Num=LED_times;
        GPIO_RESET(i);
    }
    else if(Interface[i]==4)   //生气表情
    {
        LED_Flashing_Time=0;
        Sendbuff[0]= 22;//
        send_data_time();
        Set_LED_Num=7;    //7秒
        Turn_OFF_LED(i);

        while(Set_LED_Num)
        {
            LED_Flashing(i,Set_LED_Num);
            if(KEY_Scan()==1)  //按键跳出
            {
                Set_LED_Num=LED_times;
                GPIO_RESET(i);
                return 1;
            }
        }
        Set_LED_Num=LED_times;
        GPIO_RESET(i);
    }
    else if(Interface[i]==5)   //伤心表情
    {
        LED_Flashing_Time=0;
        Sendbuff[0]= 23;//
        send_data_time();
        Set_LED_Num=7;    //7秒
        Turn_OFF_LED(i);

        while(Set_LED_Num)
        {
            LED_Flashing(i,Set_LED_Num);
            if(KEY_Scan()==1)  //按键跳出
            {
                Set_LED_Num=LED_times;
                GPIO_RESET(i);
                return 1;
            }
        }
        Set_LED_Num=LED_times;
        GPIO_RESET(i);
    }
    return 0;
}
/********声音模块****************/
uint8_t  voice_Module(uint8_t i)
{
    if(Interface[i]==11)   //电话铃声
    {
        LED_Flashing_Time=0;
        Sendbuff[0]= 30;
        Send_Data(Sendbuff);
        HAL_Delay(20);
        Send_Data(Sendbuff);
        Set_LED_Num=8;
        Turn_OFF_LED(i);

        while(Set_LED_Num)
        {
            LED_Flashing(i,Set_LED_Num);
            if(KEY_Scan()==1)  //按键跳出
            {
                Set_LED_Num=LED_times;
                GPIO_RESET(i);
                return 1;
            }
        }
        Set_LED_Num=LED_times;
        GPIO_RESET(i);
    }
    else if(Interface[i]==12)   //警车鸣笛
    {
        LED_Flashing_Time=0;
        Sendbuff[0]= 26;
        Send_Data(Sendbuff);
        HAL_Delay(20);
        Send_Data(Sendbuff);
        Set_LED_Num=8;
        Turn_OFF_LED(i);

        while(Set_LED_Num)
        {
            LED_Flashing(i,Set_LED_Num);
            if(KEY_Scan()==1)  //按键跳出
            {
                Set_LED_Num=LED_times;
                GPIO_RESET(i);
                return 1;
            }
        }
        Set_LED_Num=LED_times;
        GPIO_RESET(i);
    }
    else if(Interface[i]==13)   //消防车鸣笛
    {
        LED_Flashing_Time=0;
        Sendbuff[0]= 27;
        Send_Data(Sendbuff);
        HAL_Delay(20);
        Send_Data(Sendbuff);
        Set_LED_Num=8;
        Turn_OFF_LED(i);
        while(Set_LED_Num)
        {
            LED_Flashing(i,Set_LED_Num);
            if(KEY_Scan()==1)  //按键跳出
            {
                Set_LED_Num=LED_times;
                GPIO_RESET(i);
                return 1;
            }
        }
        Set_LED_Num=LED_times;
        GPIO_RESET(i);
    }
    else if(Interface[i]==14)   //汽车鸣笛
    {
        LED_Flashing_Time=0;
        Sendbuff[0]= 29;
        Send_Data(Sendbuff);
        HAL_Delay(20);
        Send_Data(Sendbuff);
        Set_LED_Num=8;
        Turn_OFF_LED(i);
        while(Set_LED_Num)
        {
            LED_Flashing(i,Set_LED_Num);
            if(KEY_Scan()==1)  //按键跳出
            {
                Set_LED_Num=LED_times;
                GPIO_RESET(i);
                return 1;
            }
        }
        Set_LED_Num=LED_times;
        GPIO_RESET(i);
    }
    else if(Interface[i]==6)   //播放录音
    {
        LED_Flashing_Time=0;
        Sendbuff[0]= 31;
        Send_Data(Sendbuff);
        HAL_Delay(20);
        Send_Data(Sendbuff);
        Set_LED_Num=10;    //10秒
        Turn_OFF_LED(i);
        while(Set_LED_Num)
        {
            LED_Flashing(i,Set_LED_Num);
            if(KEY_Scan()==1)  //按键跳出
            {
                Set_LED_Num=LED_times;
                GPIO_RESET(i);
                return 1;
            }
        }
        Set_LED_Num=LED_times;
        GPIO_RESET(i);
    }
    else if(Interface[i]==15)   //门铃声/火车鸣笛
    {
        LED_Flashing_Time=0;
        Sendbuff[0]= 28;
        Send_Data(Sendbuff);
        HAL_Delay(20);
        Send_Data(Sendbuff);
        Set_LED_Num=8;
        Turn_OFF_LED(i);
        while(Set_LED_Num)
        {
            LED_Flashing(i,Set_LED_Num);
            if(KEY_Scan()==1)  //按键跳出
            {
                Set_LED_Num=LED_times;
                GPIO_RESET(i);
                return 1;
            }
        }
        Set_LED_Num=LED_times;
        GPIO_RESET(i);
    }
    return 0;
}
/************其他模块************/
uint8_t  other_Module(uint8_t i)
{
    if(Interface[i]==10)   //停止电机
    {
        LED_Flashing_Time=0;
        Set_LED_Num=LED_times;
        Turn_OFF_LED(i);
        while(Set_LED_Num)
        {
            Sendbuff[0]= 35;
            send_data_time();
            LED_Flashing(i,Set_LED_Num);
            if(KEY_Scan()==1)  //按键跳出
            {
                Set_LED_Num=LED_times;
                GPIO_RESET(i);
                return 1;
            }
        }
        Set_LED_Num=LED_times;
        GPIO_RESET(i);
    }
//    else	if(Interface[i]==9)   //启动电机
//    {
//        LED_Flashing_Time=0;
//        Sendbuff[0]= 34;
//        send_data_time();
//        Set_LED_Num=LED_times;
//        Turn_OFF_LED(i);
//        while(Set_LED_Num)
//        {
//            LED_Flashing(i,Set_LED_Num);
//            if(KEY_Scan()==1)  //按键跳出
//            {
//                Set_LED_Num=LED_times;
//                GPIO_RESET(i);
//                return 1;
//            }
//        }
//        Set_LED_Num=LED_times;
//        GPIO_RESET(i);
//    }
//		else if(Interface[i]==24)  //
//        {
//				LED_Flashing_Time=0;
//        Sendbuff[0]= 47;
//        send_data_time();
//        Set_LED_Num=LED_times;
//        Turn_OFF_LED(i);
//        while(Set_LED_Num)
//        {
//            LED_Flashing(i,Set_LED_Num);
//            if(KEY_Scan()==1)  //按键跳出
//            {
//                Set_LED_Num=LED_times;
//                GPIO_RESET(i);
//                return 1;
//            }
//        }
//        Set_LED_Num=LED_times;
//        GPIO_RESET(i);
//				}
    else	if(Interface[i]==23)   //停止模块
    {
        LED_Flashing_Time=0;
        Sendbuff[0]= 20;
        send_data_time();
        Set_LED_Num=LED_times;
        Turn_OFF_LED(i);
        while(Set_LED_Num)
        {
            LED_Flashing(i,Set_LED_Num);
            if(KEY_Scan()==1)  //按键跳出
            {
                Set_LED_Num=LED_times;
                GPIO_RESET(i);
                return 1;
            }
        }
        Set_LED_Num=LED_times;
        GPIO_RESET(i);
    }
    else if(Interface[i]==7)   //开灯
    {
        LED_Flashing_Time=0;
        Sendbuff[0]= 32;
        Send_Data(Sendbuff);
        HAL_Delay(20);
        Send_Data(Sendbuff);
        Set_LED_Num=7;
        Turn_OFF_LED(i);

        while(Set_LED_Num)
        {
            LED_Flashing(i,Set_LED_Num);
            if(KEY_Scan()==1)  //按键跳出
            {
                Set_LED_Num=LED_times;
                GPIO_RESET(i);
                return 1;
            }
        }
        Set_LED_Num=LED_times;
        GPIO_RESET(i);
    }
    else if(Interface[i]==8)   //关灯
    {
        LED_Flashing_Time=0;

        Sendbuff[0]= 33;
        Send_Data(Sendbuff);
        HAL_Delay(20);
        Send_Data(Sendbuff);
        Turn_OFF_LED(i);
        Set_LED_Num=3;

        while(Set_LED_Num)
        {
            LED_Flashing(i,Set_LED_Num);
            if(KEY_Scan()==1)  //按键跳出
            {
                Set_LED_Num=LED_times;
                GPIO_RESET(i);
                return 1;
            }
        }
        Set_LED_Num=LED_times;
        GPIO_RESET(i);
    }
    else if(Interface[i]==16)   //后退一步
    {
        LED_Flashing_Time=0;
        /**/
        HAL_Delay(200);
        Sendbuff[0]= 15;
        Send_Data(Sendbuff);
        Send_Data(Sendbuff);
        HAL_Delay(100);
        Set_LED_Num=5;
        Turn_OFF_LED(i);

        while(Set_LED_Num)
        {
            Sendbuff[0]= 0;
            LED_Flashing(i,Set_LED_Num);
            if(KEY_Scan()==1)  //按键跳出
            {
                Set_LED_Num=LED_times;
                GPIO_RESET(i);
                return 1;
            }
        }
        Set_LED_Num=LED_times;
        GPIO_RESET(i);
    }
    else if(Interface[i]==17)   //右转90
    {
        LED_Flashing_Time=0;
        HAL_Delay(200);
        Sendbuff[0]= 14;
        Send_Data(Sendbuff);
        Send_Data(Sendbuff);
        HAL_Delay(100);

        Turn_OFF_LED(i);
        Set_LED_Num=3;
        while(Set_LED_Num)
        {
            Sendbuff[0]= 0;
            LED_Flashing(i,Set_LED_Num);
            if(KEY_Scan()==1)  //按键跳出
            {
                Set_LED_Num=LED_times;
                GPIO_RESET(i);
                return 1;
            }
        }
        Set_LED_Num=LED_times;
        GPIO_RESET(i);
    }
    else if(Interface[i]==19)   //左转90
    {
        LED_Flashing_Time=0;
        HAL_Delay(200);
        Sendbuff[0]= 13;
        Send_Data(Sendbuff);
        Send_Data(Sendbuff);
        HAL_Delay(100);
        Turn_OFF_LED(i);
        Set_LED_Num=3;

        while(Set_LED_Num)
        {
            Sendbuff[0]= 0;
            LED_Flashing(i,Set_LED_Num);
            if(KEY_Scan()==1)  //按键跳出
            {
                Set_LED_Num=LED_times;
                GPIO_RESET(i);
                return 1;
            }
        }
        Set_LED_Num=LED_times;
        GPIO_RESET(i);
    }
    else if(Interface[i]==21)   //前进一步
    {
        LED_Flashing_Time=0;
        HAL_Delay(200);
        Sendbuff[0]= 16;
        Send_Data(Sendbuff);
        Send_Data(Sendbuff);
        HAL_Delay(100);
        Set_LED_Num=5;
        Turn_OFF_LED(i);

        while(Set_LED_Num)
        {
            LED_Flashing(i,Set_LED_Num);
            if(KEY_Scan()==1)  //按键跳出
            {
                Set_LED_Num=LED_times;
                GPIO_RESET(i);
                return 1;
            }
        }
        Set_LED_Num=LED_times;
        Sendbuff[0]= 0;
        GPIO_RESET(i);
    }
    else if(Interface[i]==25)   //前进2步
    {
        LED_Flashing_Time=0;
        HAL_Delay(200);
        Sendbuff[0]= 36;
        Send_Data(Sendbuff);
        Send_Data(Sendbuff);
        HAL_Delay(100);
        Set_LED_Num=9;
        Turn_OFF_LED(i);

        while(Set_LED_Num)
        {
            LED_Flashing(i,Set_LED_Num);
            if(KEY_Scan()==1)  //按键跳出
            {
                Set_LED_Num=LED_times;
                GPIO_RESET(i);
                return 1;
            }
        }
        Set_LED_Num=LED_times;
        Sendbuff[0]= 0;
        GPIO_RESET(i);
    }
    return 0;
}
/***************陀螺仪模块程序*******************************/
void Gyro_Module(void)
{
    switch(Location)  //陀螺仪位置判断
    {
    case 1:   //位置1
        pwm1_input();
        break ;
    case 2:
        pwm2_input();
        break;
    case 3:
        pwm3_input();
        break;
    case 4:
        pwm4_input();
        break;
    case 5:
        pwm5_input();
        break;
    case 6:
        pwm6_input();
        break;
    case 7:
        pwm7_input();
        break;
    case 8:
        pwm8_input();
        break;
    default:
        break;
    }
}
void Module_Init(void)
{
    for(uint8_t i=0; i<8; i++)
    {
        if((Interface[i]<=30)||(Interface[i]==99))//99测试块
        {
            ID_GPIO_OUTPUT_Init(i);
        }
        else if((Interface[i]>=31)&&(Interface[i]<=35))
        {
            ID_GPIO_ADC_Init(i);
        }
        else if(Interface[i]==36)
        {
            ID_GPIO_PWM_Init(i);
        }
        else
        {
            ID_GPIO_OUTPUT_Init(i);
        }
    }
}
uint8_t turn_on;
void Boot_detection(void)
{
    /********************关机*********************************/
    if(turn_on==1)
    {
        while(HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_11)==0)
        {
            if(HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_10)==1)
            {
                if(HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_10)==1)
                {
                    On_Off_Time++;
                }
                if(On_Off_Time>=Set_On_Off_Time*100)
                {
                    HAL_GPIO_WritePin(GPIOC,GPIO_PIN_10,GPIO_PIN_RESET);
                    On_Off_Time=0;
                    for(uint8_t i=0; i<8; i++)
                    {
                        ID_GPIO_OUTPUT_Init(i);
                        GPIO_SET(i);
                        GPIO_SET(i);
                    }
                    HAL_GPIO_WritePin(GPIOC,GPIO_PIN_10,GPIO_PIN_RESET);
                    turn_onn=1;
                    HAL_GPIO_WritePin(GPIOC,GPIO_PIN_10,GPIO_PIN_RESET);
                }
            }
            else
            {
                On_Off_Time=0;
            }
        }
    }
    if(turn_onn==1&&(HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_11)==1))
    {
        turn_on=0;
        turn_onn=0;
    }
    /*******************开机****************************/
    if(turn_on==0)
    {
        while(HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_11)==0)
        {
            if(HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_10)==0)
            {
                if(HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_10)==0)
                {
                    On_Off_Time++;
                }
                if(On_Off_Time>=Set_On_Off_Time*100)
                {
                    HAL_GPIO_WritePin(GPIOC,GPIO_PIN_10,GPIO_PIN_SET);
                    On_Off_Time=0;
                    for(uint8_t i=0; i<8; i++)
                    {
                        ID_GPIO_OUTPUT_Init(i);
                        GPIO_RESET(i);
                        GPIO_RESET(i);
                    }
                    turn_onn=2;
                }
            }
            else
            {
                On_Off_Time=0;
            }
        }
    }
    if(turn_onn==2&&(HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_11)==1))
    {
        turn_on=1;
        turn_onn=0;
    }
}
/**********按键检测***************/
uint8_t key_up=1;     //按键松开标志
uint16_t Long_key_up=0;
uint8_t KEY_Scan(void)//按键按下返回1，未按下返回0
{
    if(key_up&&(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_8)==0))
    {
        if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_8)==0)
        {
            key_up=0;
            Shutdown=0;  //自动关机计数清零
            return 1;
        }
    }
    else if((key_up==0)&&HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_8)==1)
    {
        key_up=1;
        return 2;
    }
    return 0;   //无按键按下
}
void Auto_OFF()    //自动关机
{
    if(Shutdown>600*90)  //一个半小时无操作自动关机
    {
        HAL_GPIO_WritePin(GPIOC,GPIO_PIN_10,GPIO_PIN_RESET);  //关机
    }
}
void LED_Flashing(uint8_t i,uint8_t j)
{
    if(LED_Flashing_Time>Set_LED_Time2)
    {
        LED_Flashing_Time=0;
        Set_LED_Num--;
        if(Set_LED_Num>250)
        {
            Set_LED_Num=0;
        }
    }
    else  if(LED_Flashing_Time<Set_LED_Time1)
    {
        GPIO_SET(i);
    }
    else if((LED_Flashing_Time>=Set_LED_Time1)&&(LED_Flashing_Time<=Set_LED_Time2))
    {
        GPIO_RESET(i);
    }
}
void send_data_time(void)
{
    Send_Data(Sendbuff);
    HAL_Delay(10);
    Send_Data(Sendbuff);
}
/********关闭第二次循环灯****************/
void Turn_OFF_LED(uint8_t i)
{
    for(uint8_t y=i; y<7; y++)
    {
        GPIO_SET(y);
    }
}
/**直到模块+模块**/
uint8_t straight_Movement(uint8_t i)
{
    if(Interface[i]==22)  //前进直到模块
    {
        if((Interface[i+1]==31)||(Interface[i+1]==32)||(Interface[i+1]==33)||(Interface[i+1]==34)||(Interface[i+1]==35))  //光电模块
        {
            LED_Flashing_Time=0;
            while(KEY_Scan()!=1)
            {
                for(uint8_t t=2; t+i<8; t++)
                {
                    GPIO_SET(i+t);
                }
                Sendbuff[0]= 19;
                send_data_time();
                //nterface[i]==2的LED灯闪烁
                LED_Flashing(i,0);
                /*****ADC值位置+9******/
                if(ADC_Value[i+9]<2048)
                {
                    if(ADC_Value[i+9]<2048)
                    {
                        GPIO_RESET(i);
                        return 1;
                    }
                }
            }
            GPIO_RESET(i);//亮		Interface[i]==2的LED灯
            HAL_Delay(50);
            return 2;
        }
        else if(Interface[i+1]==36)  //等待模式
        {

            LED_Flashing_Time=0;
            LED_Flashing(i,0);
            while(KEY_Scan()!=1)
            {
                for(uint8_t t=2; t+i<8; t++)
                {
                    GPIO_SET(i+t);
                }
                Sendbuff[0]= 19;
                send_data_time();
                //nterface[i]==2的LED灯闪烁
                LED_Flashing(i,0);
                /*****ADC值位置+9******/
                if(ADC_Value[i+9]<3850)
                {
                    if(ADC_Value[i+9]<3850)
                    {
                        GPIO_RESET(i);
                        return 1;
                    }
                }
            }
            GPIO_RESET(i);//亮		Interface[i]==2的LED灯
            HAL_Delay(50);
            return 2;
        }
        GPIO_RESET(i);//亮		Interface[i]==2的LED灯
    }
    return 0;
}
/**右转直到模块+模块**/
uint8_t Right_Movement(uint8_t i)
{
    if(Interface[i]==18)  //右转直到模块
    {
        if((Interface[i+1]==31)||(Interface[i+1]==32)||(Interface[i+1]==33)||(Interface[i+1]==34)||(Interface[i+1]==35))  //光电模块
        {
            LED_Flashing_Time=0;
            while(KEY_Scan()!=1)
            {
                for(uint8_t t=2; t+i<8; t++)
                {
                    GPIO_SET(i+t);
                }
                Sendbuff[0]= 18;
                send_data_time();
                //nterface[i]==2的LED灯闪烁
                LED_Flashing(i,0);
                /*****ADC值位置+9******/
                if(ADC_Value[i+9]<2048)
                {
                    HAL_Delay(10);
                    if(ADC_Value[i+9]<2048)
                    {
                        GPIO_RESET(i);
                        return 1;
                    }
                }
            }
            GPIO_RESET(i);//亮		Interface[i]==2的LED灯
            HAL_Delay(50);
            return 2;
        }
        GPIO_RESET(i);//亮		Interface[i]==2的LED灯
    }
    return 0;
}



/**左转直到模块+模块**/
uint8_t Left_Movement(uint8_t i)
{
    if(Interface[i]==20)  //左转直到模块
    {
        if((Interface[i+1]==31)||(Interface[i+1]==32)||(Interface[i+1]==33)||(Interface[i+1]==34)||(Interface[i+1]==35))  //光电模块
        {
            LED_Flashing_Time=0;
            while(KEY_Scan()!=1)
            {
                for(uint8_t t=2; t+i<8; t++)
                {
                    GPIO_SET(i+t);
                }
                Sendbuff[0]= 17;
                send_data_time();
                //nterface[i]==2的LED灯闪烁
                LED_Flashing(i,0);
                /*****ADC值位置+9******/
                if(ADC_Value[i+9]<2048)
                {
                    if(ADC_Value[i+9]<2048)
                    {
                        GPIO_RESET(i);
                        return 1;
                    }
                }
            }
            GPIO_RESET(i);//亮		Interface[i]==2的LED灯
            HAL_Delay(50);
            return 2;
        }
        GPIO_RESET(i);//亮		Interface[i]==2的LED灯
    }
    return 0;
}


