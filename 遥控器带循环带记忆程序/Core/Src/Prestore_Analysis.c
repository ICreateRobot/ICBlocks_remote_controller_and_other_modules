#include "Prestore_Analysis.h"
#include "Data_Analysis.h"
#include "main.h"
#include "adc.h"
#include "tim.h"
#include "gpio.h"
#include "Bluetooth.h"
/**************预存读取指令涉及变量**********************/
uint8_t Prestore_Data[99]= {0}; //预存数据
uint8_t Prestore_Times=0;       //预存数据轮次
uint8_t Data_Flag=0;
uint8_t IOWR_Flag=0;
#define LED_times 3  //LED闪烁次数LED_times+1
/*******************************************/
uint16_t IORD_Loop_number_Times[99]= {0}; //第n个循环次数
uint8_t IORD_Loop_number_Start[99]= {0}; //第n个循环开始位置
uint8_t IORD_Loop_number_Stop[99]= {0};  //第n个循环结束位置
uint16_t IORD_Loop_Times[49]= {0};
uint8_t IORD_Loop_Start[49]= {0},IORD_Loop_Stop[49]= {0}; //循环次数/开始位置/结束位置
uint8_t IORD_Sort_Order=0;            //第几个循环块
uint8_t IORD_Order_Residue=0;        //剩余几个循环块
uint8_t IORD_Order[5]= {0};
uint8_t Last_IORD_Sort_Order=0;    //上一次预存的循环个数
uint8_t Last_Loop_number_Stop=0;   //上次预存的循环结束位置
uint8_t IORD_Prestore_Times=0;    //程序执行8个模块执行到第几次
uint8_t IOWD_Prestore_Times=0;
uint8_t IORD_Sort_Order_S[99]= {0};
/******************确定模块顺序***********************/
uint8_t Clear_KEY=0;
uint8_t IOWD_Nummber=0;
uint8_t Write_Finish=0;  //存入完成
uint8_t Write_Interface[99]= {0};
/*************可以存入的模块指令******************/
uint8_t Module_Judge()  //模块指令判断  确定那些可以存入那些不可以存入  返回1 可以存入
{
    IOWR_Flag=IOWR_Judge();
//	Clear_KEY_DOWN();
    for(uint8_t i=0; i<8; i++)
    {
        if(((Interface[0]==37)||(Interface[7]==37))&&(Clear_KEY==1))
        {
            return 2;   //清除标志
        }
        else if((Interface[7]==37)&&(IOWR_Flag==1))
        {
            return 1;  //可以存入标志
        }
        else if(Interface[7]==37)
        {
            return 3;  //有预存模块标志
        }
    }
    return 0;
}
/***************************/
void IOWR()  //预存指令模块写入
{
    uint8_t k=0;
    if(Interface[7]==37)   //有预存模块
    {
        if((KEY_Scan()==2)&&(Module_Judge()==1)&&(Write_Finish==0))
        {
            IOWD_Prestore_Times+=1;

            for(uint8_t j=8*(IOWD_Prestore_Times-1); j<(8*IOWD_Prestore_Times-1); j++)
            {
                Prestore_Data[j]=Interface[k];  //存入数据数组
                Write_Interface[k]=Interface[k];
                k+=1;
            }
            Prestore_Times++;                   //预存数据轮次加一
            for(uint8_t k=0; k<6; k++)
            {
                for(uint8_t i=0; i<8; i++)
                {
                    GPIO_SET(i);
                }
                HAL_Delay(100);
                for(uint8_t i=0; i<8; i++)
                {
                    GPIO_RESET(i);
                }
                HAL_Delay(100);
                Write_Finish=1;
            }
        }
//            HAL_Delay(500);                     //等待500ms后准备下一次预存
    }
}


void IORD(uint8_t j)  //读取指令
{
    if(Interface[0]==37)  //读取指令
    {
        for(IORD_Prestore_Times=IORD_Prestore_Times+1; IORD_Prestore_Times<=IOWD_Prestore_Times; IORD_Prestore_Times++)
        {
//        if(IORD_Sort_Order>0) //有内循环模块的情况
            if(Internal_Loop_Judgment()==1)//有内循环模块的情况
            {
//            Order_Residue=Sort_Order+1;
                for(uint8_t w=1; w<=IORD_Sort_Order_S[IORD_Prestore_Times]; w++)    //有几个循环模块4
                {
                    Order_Residue--;
//                Loop_Times=Loop_number_Times[w];//第几个循环/循环次数
//                Loop_Start=Loop_number_Start[w];//第几个循环/开始位置
//                Loop_Stop=Loop_number_Stop[w];  //第几个循环/结束位置
                    IORD_Loop_one(IORD_Loop_Times[w-1+IORD_Prestore_Times], IORD_Loop_Start[w-1+IORD_Prestore_Times], IORD_Loop_Stop[w-1+IORD_Prestore_Times],j);
                }
                for(uint8_t y=IORD_Loop_Stop[IORD_Prestore_Times]+1; y<=(7*IORD_Prestore_Times); y++)  //模块循环开始结束
                {
                    if(Module_Performs(y,j)==1)
                    {
                        break;
                    }
                }
            }
            else  //无内循环
            {
                for(uint8_t i=8*(IORD_Prestore_Times-1)+1; i<=(8*IORD_Prestore_Times-1); i++)
                {
                    if(Module_Performs(i,j)==1)
                    {
                        break;
                    }
                }
            }
        }
    }
}
void IORD_Running(uint8_t j)  //读取指令运行
{
    if(j==0)
    {
        if(Interface[j]==37)   //读取模块
        {
            if(IORD_Sort_Order>0) //有内循环模块的情况)
            {
                for(uint8_t w=1; w<=IORD_Sort_Order; w++)    //有几个循环模块4
                {
                    IORD_Loop_one( IORD_Loop_Times[w],IORD_Loop_Start[w],IORD_Loop_Stop[w],j);
                    for(uint8_t z=1; z<=7; z++)
                    {
                        GPIO_SET(z);
                    }
                }
                for(uint8_t y=IORD_Loop_Stop[IORD_Sort_Order]+1; y<=(7*IOWD_Prestore_Times); y++)  //模块循环开始结束
                {
                    if(Module_Performs(y,j)==1)
                    {
                        break;
                    }
                }
            }
            else   //无内循环
            {
                for(uint8_t i=0; i<=IOWD_Prestore_Times*7; i++)
                {
                    if(Module_Performs(i,j)==1)
                    {
                        break;
                    }
                }
            }
        }
    }
}
void IORD_Analyse(uint8_t j)  //读取指令分析
{
    Loop_Number();   //是否有内循环判断

}
uint8_t Loop_Number()  //循环个数
{
    for(uint8_t i=0; i<IOWD_Prestore_Times*8; i++)
    {
        if(Prestore_Data[i]==26)  //循环一次
        {
            IORD_Sort_Order+=1;
            IORD_Loop_Times[IORD_Sort_Order]=1;  //第n个循环次数
            if(IORD_Sort_Order>1)
            {
                IORD_Loop_Start[IORD_Sort_Order]=IORD_Loop_Stop[IORD_Sort_Order-1]+2;  //第n个循环开始位置
            }
            else
            {
                IORD_Loop_Start[IORD_Sort_Order]=0;  //第n个循环开始位置
            }
            if(i>0)
            {
                IORD_Loop_Stop[IORD_Sort_Order]=i-1;   //第n个循环结束位置
            }
            else
            {
                IORD_Loop_Stop[IORD_Sort_Order]=i;   //第n个循环结束位置
            }
        }
        else if(Prestore_Data[i]==27)  //循环两次
        {
            IORD_Sort_Order+=1;
            IORD_Loop_Times[IORD_Sort_Order]=2;  //第n个循环次数
            if(IORD_Sort_Order>1)
            {
                IORD_Loop_Start[IORD_Sort_Order]=IORD_Loop_Stop[IORD_Sort_Order-1]+2;  //第n个循环开始位置
            }
            else
            {
                IORD_Loop_Start[IORD_Sort_Order]=0;  //第n个循环开始位置
            }
            if(i>0)
            {
                IORD_Loop_Stop[IORD_Sort_Order]=i-1;   //第n个循环结束位置
            }
            else
            {
                IORD_Loop_Stop[IORD_Sort_Order]=i;   //第n个循环结束位置
            }
        }
        else if(Prestore_Data[i]==28)  //循环三次
        {
            IORD_Sort_Order+=1;
            IORD_Loop_Times[IORD_Sort_Order]=3;  //第n个循环次数
            if(IORD_Sort_Order>1)
            {
                IORD_Loop_Start[IORD_Sort_Order]=IORD_Loop_Stop[IORD_Sort_Order-1]+2;  //第n个循环开始位置
            }
            else
            {
                IORD_Loop_Start[IORD_Sort_Order]=0;  //第n个循环开始位置
            }
            if(i>0)
            {
                IORD_Loop_Stop[IORD_Sort_Order]=i-1;   //第n个循环结束位置
            }
            else
            {
                IORD_Loop_Stop[IORD_Sort_Order]=i;   //第n个循环结束位置
            }
        }
        else if(Prestore_Data[i]==29)          //循环多次
        {
            IORD_Sort_Order+=1;
            IORD_Loop_Times[IORD_Sort_Order]=65530;  //第n个循环次数
            if(IORD_Sort_Order>1)
            {
                IORD_Loop_Start[IORD_Sort_Order]=IORD_Loop_Stop[IORD_Sort_Order-1]+2;  //第n个循环开始位置
            }
            else
            {
                IORD_Loop_Start[IORD_Sort_Order]=0;  //第n个循环开始位置
            }
            if(i>0)
            {
                IORD_Loop_Stop[IORD_Sort_Order]=i-1;   //第n个循环结束位置
            }
            else
            {
                IORD_Loop_Stop[IORD_Sort_Order]=i;   //第n个循环结束位置
            }
        }

    }
    if(IORD_Sort_Order>0)
    {
        return 1;
    }
    return 0;
}
uint8_t Internal_Loop_Judgment()   //8个块里边是否有内循环模块
{
    for(uint8_t i=8*(IORD_Prestore_Times-1)+1; i<(8*IORD_Prestore_Times-1); i++)
    {
        if((Prestore_Data[i]==26)||(Prestore_Data[i]==27)||(Prestore_Data[i]==28)||(Prestore_Data[i]==29))
        {
            return 1;
        }
    }
    return 0;
}

uint8_t IORD_Loop_one(uint16_t Loop_Times,uint8_t Loop_Start,uint8_t Loop_Stop,uint8_t j)   //一个完整的循环
{
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
                /**表情模块**/
                if(Module_Performs(y,0)==1)
                {
                    for(y=Loop_Start; y<=Loop_Stop+1; y++)
                    {
                        GPIO_RESET(y);
                    }
                    return 1;
                }
            }
        }
    }
    GPIO_RESET(Loop_Stop+1);
    return 0;
}

uint8_t Module_Performs(uint8_t i,uint8_t j)     //
{
    /*****************表情模块*****************/
    if(Prestore_Data[i]==1)   //开心表情
    {
        LED_Flashing_Time=0;
        Sendbuff[0]= 24;
        send_data_time();
        Set_LED_Num=7;    //7秒
//        Turn_OFF_LED(j);
        while(Set_LED_Num)
        {
            LED_Flashing(j,Set_LED_Num);
            if(KEY_Scan()==1)  //按键跳出
            {
                Set_LED_Num=LED_times;
//                GPIO_RESET(j);
                return 1;
            }
        }
        Set_LED_Num=LED_times;
//        GPIO_RESET(j);
    }
    else if(Prestore_Data[i]==2)   //哭泣表情
    {
        LED_Flashing_Time=0;
        Sendbuff[0]= 25;
        send_data_time();
        Set_LED_Num=7;    //7秒
        Turn_OFF_LED(j);
        while(Set_LED_Num)
        {
            LED_Flashing(j,Set_LED_Num);
            if(KEY_Scan()==1)  //按键跳出
            {
                Set_LED_Num=LED_times;
                GPIO_RESET(j);
                return 1;
            }
        }
        Set_LED_Num=LED_times;
        GPIO_RESET(j);
    }
    else if(Prestore_Data[i]==3)   //微笑表情
    {
        LED_Flashing_Time=0;
        Sendbuff[0]= 21;
        send_data_time();
        Set_LED_Num=7;    //7秒
//        Turn_OFF_LED(j);
        while(Set_LED_Num)
        {
            LED_Flashing(j,Set_LED_Num);
            if(KEY_Scan()==1)  //按键跳出
            {
                Set_LED_Num=LED_times;
//                GPIO_RESET(j);
                return 1;
            }
        }
        Set_LED_Num=LED_times;
//        GPIO_RESET(j);
    }
    else if(Prestore_Data[i]==4)   //生气表情
    {
        LED_Flashing_Time=0;
        Sendbuff[0]= 22;//
        send_data_time();
        Set_LED_Num=7;    //7秒
        Turn_OFF_LED(j);
        while(Set_LED_Num)
        {
            LED_Flashing(j,Set_LED_Num);
            if(KEY_Scan()==1)  //按键跳出
            {
                Set_LED_Num=LED_times;
                GPIO_RESET(j);
                return 1;
            }
        }
        Set_LED_Num=LED_times;
        GPIO_RESET(j);
    }
    else if(Prestore_Data[i]==5)   //伤心表情
    {
        LED_Flashing_Time=0;
        Sendbuff[0]= 23;//
        send_data_time();
        Set_LED_Num=7;    //7秒
        Turn_OFF_LED(j);
        while(Set_LED_Num)
        {
            LED_Flashing(j,Set_LED_Num);
            if(KEY_Scan()==1)  //按键跳出
            {
                Set_LED_Num=LED_times;
                GPIO_RESET(j);
                return 1;
            }
        }
        Set_LED_Num=LED_times;
        GPIO_RESET(j);
    }
    /***************声音模块********************/
    else if(Prestore_Data[i]==11)   //电话铃声
    {
        LED_Flashing_Time=0;
        Sendbuff[0]= 30;
        Send_Data(Sendbuff);
//        HAL_Delay(20);
//        Send_Data(Sendbuff);
        Set_LED_Num=8;
        Turn_OFF_LED(j);

        while(Set_LED_Num)
        {
            LED_Flashing(j,Set_LED_Num);
            if(KEY_Scan()==1)  //按键跳出
            {
                Set_LED_Num=LED_times;
                GPIO_RESET(j);
                return 1;
            }
        }
        Set_LED_Num=LED_times;
        GPIO_RESET(j);
    }
    else if(Prestore_Data[i]==12)   //警车鸣笛
    {
        LED_Flashing_Time=0;
        Sendbuff[0]= 26;
        Send_Data(Sendbuff);
//        HAL_Delay(20);
//        Send_Data(Sendbuff);
        Set_LED_Num=8;
        Turn_OFF_LED(j);

        while(Set_LED_Num)
        {
            LED_Flashing(j,Set_LED_Num);
            if(KEY_Scan()==1)  //按键跳出
            {
                Set_LED_Num=LED_times;
                GPIO_RESET(j);
                return 1;
            }
        }
        Set_LED_Num=LED_times;
        GPIO_RESET(j);
    }
    else if(Prestore_Data[i]==13)   //消防车鸣笛
    {
        LED_Flashing_Time=0;
        Sendbuff[0]= 27;
        Send_Data(Sendbuff);
//        HAL_Delay(20);
//        Send_Data(Sendbuff);
        Set_LED_Num=8;
        Turn_OFF_LED(j);
        while(Set_LED_Num)
        {
            LED_Flashing(j,Set_LED_Num);
            if(KEY_Scan()==1)  //按键跳出
            {
                Set_LED_Num=LED_times;
                GPIO_RESET(j);
                return 1;
            }
        }
        Set_LED_Num=LED_times;
        GPIO_RESET(j);
    }
    else if(Prestore_Data[i]==14)   //汽车鸣笛
    {
        LED_Flashing_Time=0;
        Sendbuff[0]= 29;
        Send_Data(Sendbuff);
//        HAL_Delay(20);
//        Send_Data(Sendbuff);
        Set_LED_Num=8;
        Turn_OFF_LED(j);
        while(Set_LED_Num)
        {
            LED_Flashing(j,Set_LED_Num);
            if(KEY_Scan()==1)  //按键跳出
            {
                Set_LED_Num=LED_times;
                GPIO_RESET(j);
                return 1;
            }
        }
        Set_LED_Num=LED_times;
        GPIO_RESET(j);
    }
    else if(Prestore_Data[i]==6)   //播放录音
    {
        LED_Flashing_Time=0;
        Sendbuff[0]= 31;
        Send_Data(Sendbuff);
//        HAL_Delay(20);
//        Send_Data(Sendbuff);
        Set_LED_Num=10;    //10秒
        Turn_OFF_LED(j);
        while(Set_LED_Num)
        {
            LED_Flashing(j,Set_LED_Num);
            if(KEY_Scan()==1)  //按键跳出
            {
                Set_LED_Num=LED_times;
                GPIO_RESET(j);
                return 1;
            }
        }
        Set_LED_Num=LED_times;
        GPIO_RESET(j);
    }
    else if(Prestore_Data[i]==15)   //门铃声/火车鸣笛
    {
        LED_Flashing_Time=0;
        Sendbuff[0]= 28;
        Send_Data(Sendbuff);
//        HAL_Delay(20);
//        Send_Data(Sendbuff);
        Set_LED_Num=8;
        Turn_OFF_LED(j);
        while(Set_LED_Num)
        {
            LED_Flashing(j,Set_LED_Num);
            if(KEY_Scan()==1)  //按键跳出
            {
                Set_LED_Num=LED_times;
                GPIO_RESET(j);
                return 1;
            }
        }
        Set_LED_Num=LED_times;
        GPIO_RESET(j);
    }
    /**********运动类模块**********************/
    else if(Prestore_Data[i]==10)   //停止电机
    {
        LED_Flashing_Time=0;
        Set_LED_Num=LED_times;
        Turn_OFF_LED(j);
        while(Set_LED_Num)
        {
            Sendbuff[0]= 35;
            send_data_time();
            LED_Flashing(j,Set_LED_Num);
            if(KEY_Scan()==1)  //按键跳出
            {
                Set_LED_Num=LED_times;
                GPIO_RESET(j);
                return 1;
            }
        }
        Set_LED_Num=LED_times;
        GPIO_RESET(j);
    }
    else	if(Prestore_Data[i]==9)   //启动电机
    {
        LED_Flashing_Time=0;
        Sendbuff[0]= 34;
        send_data_time();
        Set_LED_Num=LED_times;
        Turn_OFF_LED(j);
        while(Set_LED_Num)
        {
            LED_Flashing(j,Set_LED_Num);
            if(KEY_Scan()==1)  //按键跳出
            {
                Set_LED_Num=LED_times;
                GPIO_RESET(j);
                return 1;
            }
        }
        Set_LED_Num=LED_times;
        GPIO_RESET(j);
    }
    else if(Prestore_Data[i]==24)  //
    {
        LED_Flashing_Time=0;
        Sendbuff[0]= 47;
        send_data_time();
        Set_LED_Num=LED_times;
        Turn_OFF_LED(j);
        while(Set_LED_Num)
        {
            LED_Flashing(j,Set_LED_Num);
            if(KEY_Scan()==1)  //按键跳出
            {
                Set_LED_Num=LED_times;
                GPIO_RESET(j);
                return 1;
            }
        }
        Set_LED_Num=LED_times;
        GPIO_RESET(j);
    }
    else	if(Prestore_Data[i]==23)   //停止模块
    {
        LED_Flashing_Time=0;
        Sendbuff[0]= 20;
        send_data_time();
        Set_LED_Num=LED_times;
        Turn_OFF_LED(j);
        while(Set_LED_Num)
        {
            LED_Flashing(j,Set_LED_Num);
            if(KEY_Scan()==1)  //按键跳出
            {
                Set_LED_Num=LED_times;
                GPIO_RESET(j);
                return 1;
            }
        }
        Set_LED_Num=LED_times;
        GPIO_RESET(j);
    }
    else if(Prestore_Data[i]==7)   //开灯
    {
        LED_Flashing_Time=0;
        Sendbuff[0]= 32;
        Send_Data(Sendbuff);
        HAL_Delay(20);
        Send_Data(Sendbuff);
        Set_LED_Num=7;
        Turn_OFF_LED(j);

        while(Set_LED_Num)
        {
            LED_Flashing(j,Set_LED_Num);
            if(KEY_Scan()==1)  //按键跳出
            {
                Set_LED_Num=LED_times;
                GPIO_RESET(j);
                return 1;
            }
        }
        Set_LED_Num=LED_times;
        GPIO_RESET(j);
    }
    else if(Prestore_Data[i]==8)   //关灯
    {
        LED_Flashing_Time=0;

        Sendbuff[0]= 33;
        Send_Data(Sendbuff);
        HAL_Delay(20);
        Send_Data(Sendbuff);
        Turn_OFF_LED(j);
        Set_LED_Num=3;

        while(Set_LED_Num)
        {
            LED_Flashing(j,Set_LED_Num);
            if(KEY_Scan()==1)  //按键跳出
            {
                Set_LED_Num=LED_times;
                GPIO_RESET(j);
                return 1;
            }
        }
        Set_LED_Num=LED_times;
        GPIO_RESET(j);
    }
    else if(Prestore_Data[i]==16)   //后退一步
    {
        LED_Flashing_Time=0;
        /**/
        HAL_Delay(200);
        Sendbuff[0]= 15;
        Send_Data(Sendbuff);
        Send_Data(Sendbuff);
        HAL_Delay(100);
        Set_LED_Num=5;
        Turn_OFF_LED(j);

        while(Set_LED_Num)
        {
            Sendbuff[0]= 0;
            LED_Flashing(j,Set_LED_Num);
            if(KEY_Scan()==1)  //按键跳出
            {
                Set_LED_Num=LED_times;
                GPIO_RESET(j);
                return 1;
            }
        }
        Set_LED_Num=LED_times;
        GPIO_RESET(j);
    }
    else if(Prestore_Data[i]==17)   //右转90
    {
        LED_Flashing_Time=0;
        HAL_Delay(200);
        Sendbuff[0]= 14;
        Send_Data(Sendbuff);
        Send_Data(Sendbuff);
        HAL_Delay(100);

        Turn_OFF_LED(j);
        Set_LED_Num=3;
        while(Set_LED_Num)
        {
            Sendbuff[0]= 0;
            LED_Flashing(j,Set_LED_Num);
            if(KEY_Scan()==1)  //按键跳出
            {
                Set_LED_Num=LED_times;
                GPIO_RESET(j);
                return 1;
            }
        }
        Set_LED_Num=LED_times;
        GPIO_RESET(j);
    }
    else if(Prestore_Data[i]==19)   //左转90
    {
        LED_Flashing_Time=0;
        HAL_Delay(200);
        Sendbuff[0]= 13;
        Send_Data(Sendbuff);
        Send_Data(Sendbuff);
        HAL_Delay(100);
//        Turn_OFF_LED(j);
        Set_LED_Num=3;

        while(Set_LED_Num)
        {
            Sendbuff[0]= 0;
            LED_Flashing(j,Set_LED_Num);
            if(KEY_Scan()==1)  //按键跳出
            {
                Set_LED_Num=LED_times;
//                GPIO_RESET(j);
                return 1;
            }
        }
        Set_LED_Num=LED_times;
//        GPIO_RESET(j);
    }
    else if(Prestore_Data[i]==21)   //前进一步
    {
        LED_Flashing_Time=0;
        HAL_Delay(200);
        Sendbuff[0]= 16;
        Send_Data(Sendbuff);
        Send_Data(Sendbuff);
        HAL_Delay(100);
        Set_LED_Num=5;
        Turn_OFF_LED(j);

        while(Set_LED_Num)
        {
            LED_Flashing(j,Set_LED_Num);
            if(KEY_Scan()==1)  //按键跳出
            {
                Set_LED_Num=LED_times;
                GPIO_RESET(j);
                return 1;
            }
        }
        Set_LED_Num=LED_times;
        Sendbuff[0]= 0;
        GPIO_RESET(j);
    }
    else if(Prestore_Data[i]==25)   //前进2步
    {
        LED_Flashing_Time=0;
        HAL_Delay(200);
        Sendbuff[0]= 36;
        Send_Data(Sendbuff);
        Send_Data(Sendbuff);
        HAL_Delay(100);
        Set_LED_Num=9;
        Turn_OFF_LED(j);

        while(Set_LED_Num)
        {
            LED_Flashing(j,Set_LED_Num);
            if(KEY_Scan()==1)  //按键跳出
            {
                Set_LED_Num=LED_times;
                GPIO_RESET(j);
                return 1;
            }
        }
        Set_LED_Num=LED_times;
        Sendbuff[0]= 0;
        GPIO_RESET(j);
    }
    return 0;
}
uint8_t IOWR_Judge()  //读取指令判断
{


    if((Interface[0]==Write_Interface[0])&&(Interface[1]==Write_Interface[1])&&(Interface[2]==Write_Interface[2])&&(Interface[3]==Write_Interface[3])&&
            (Interface[4]==Write_Interface[4])&&(Interface[5]==Write_Interface[5])&&(Interface[6]==Write_Interface[6]))
    {
        Write_Finish=1;
        return 0;  //不可以存入标志
    }
    for(uint8_t i=0; i<8; i++)
    {
        if((Interface[i]==18)||(Interface[i]==20)||(Interface[i]==22)||(Interface[i]==31)||(Interface[i]==32)||(Interface[i]==33)||(Interface[i]==34)||
                (Interface[i]==35)||(Interface[i]==36)||(Interface[i]==38))//(Interface[i]==26)||(Interface[i]==27)||(Interface[i]==28)||(Interface[i]==29)||
        {
            Write_Finish=1;
            return 0;  //不可以存入标志
        }
    }
    Write_Finish=0;
    return 1;
}


void Starting_Procedure_With_IOWR(void)   //有读取模块的程序执行
{
    for(uint8_t i=0; i<8; i++)
    {
//        Module_Performs(i,j);
    }
    /***程序结束恢复默认表情***/
//    Sendbuff[0]= 55;   //截止符
    Send_Data(Sendbuff);
    HAL_Delay(100);
    Send_Data(Sendbuff);
    HAL_Delay(100);
    Send_Data(Sendbuff);
    HAL_Delay(100);
    Send_Data(Sendbuff);

    Sendbuff[0]= 0;
    Sendbuff1[0]=0;
    /*************/
    for(uint8_t i=0; i<8; i++)
    {
        ID_GPIO_OUTPUT_Init(i);
        GPIO_RESET(i);
    }
}
uint8_t Clear_KEY_DOWN() //清除数据
{
//    Prestore_Times=0;
//    for(uint8_t k=0; k<99; k++) //数据清零
//    {
//        Prestore_Data[k]=0;
//    }
    if(key_up==0)
    {
        Long_key_up++;
        if(Long_key_up>=80) //按键按下时间=80*50ms=4s
        {
            Long_key_up=0;
            Clear_KEY=1;
            return 1;
        }
    }
    else
    {
        Long_key_up=0;
    }
    return 0;
}
void Clear_Data() //清除数据
{
    Prestore_Times=0;
    IORD_Sort_Order=0;
    IOWD_Prestore_Times=0;
    Write_Finish=1;
    for(uint8_t i=0; i<7; i++)
    {
        Write_Interface[i]=0;
    }
    for(uint8_t k=0; k<99; k++) //数据清零
    {
        Prestore_Data[k]=0;
    }
    for(uint8_t k=0; k<49; k++) //数据清零
    {
        IORD_Loop_Times[k]=0;
        IORD_Loop_Start[k]=0;
        IORD_Loop_Stop[k]=0;
    }
    Clear_KEY=0;//清零
    if(Interface[0]==37)
    {
        for(uint8_t k=0; k<8; k++)
        {
//        for(uint8_t i=0; i<7; i++)
//        {
            GPIO_SET(0);
//        }
            HAL_Delay(200);
//        for(uint8_t i=0; i<7; i++)
//        {
            GPIO_RESET(0);
//        }
            HAL_Delay(200);
        }
    }
    else if(Interface[7]==37)
    {
        for(uint8_t k=0; k<8; k++)
        {
//        for(uint8_t i=0; i<7; i++)
//        {
            GPIO_SET(7);
//        }
            HAL_Delay(200);
//        for(uint8_t i=0; i<7; i++)
//        {
            GPIO_RESET(7);
//        }
            HAL_Delay(200);
        }
    }

}



