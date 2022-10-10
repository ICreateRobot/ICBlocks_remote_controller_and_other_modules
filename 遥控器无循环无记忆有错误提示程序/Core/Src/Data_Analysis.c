#include "Data_Analysis.h"
#include "main.h"
#include "adc.h"
#include "tim.h"
#include "gpio.h"
#include "Bluetooth.h"
#include "stm32f1xx_it.h"
uint32_t On_Off_Time;
#define Set_On_Off_Time  5000
#define Set_LED_Time1  5
#define Set_LED_Time2  10
#define LED_times 3  //LED��˸����LED_times+1
uint8_t Set_LED_Num =3;
//uint8_t Module[8]={0};  //8���ӿ�
uint8_t Interface[8]= {0}; //8���ӿ�
uint8_t Location;
uint8_t turn_onn;
uint8_t bbb;
/******************ȷ��ģ��˳��***********************/
void Interface_module(void)
{
    for(uint8_t i=0; i<8; i++)
    {
        if(ADC_Value[i]<25)                         //���ģ��
        {
            Interface[i]=31;    ////ģ��11......
        }
        else if((26<=ADC_Value[i])&&(ADC_Value[i]<125))    //����ģ��
        {
            Interface[i]=33;    ////ģ��11......
        }
        else if((125<=ADC_Value[i])&&(ADC_Value[i]<220))//����ģ��
        {
            Interface[i]=34;    ////ģ��11......
        }
        else if((220<=ADC_Value[i])&&(ADC_Value[i]<300))//��ťģ��
        {
            Interface[i]=35;////ģ��11......
        }
        else if((300<=ADC_Value[i])&&(ADC_Value[i]<445))//��бģ��
        {
            Interface[i]=36;    ////ģ��11......
        }
        else if((445<=ADC_Value[i])&&(ADC_Value[i]<545))  //��ťģ��
        {
            Interface[i]=32;    ////ģ��11......
        }
        else if((545<=ADC_Value[i])&&(ADC_Value[i]<640))  //�˶�ģ��
        {
            Interface[i]=24;    ////ģ��11......
        }
        else if((640<=ADC_Value[i])&&(ADC_Value[i]<730))//��ת90��/*****
        {
            Interface[i]=19;    ////ģ��11......
        }
        else if((730<=ADC_Value[i])&&(ADC_Value[i]<850))//��ת90��
        {
            Interface[i]=17;    ////ģ��11......
        }
        else if((850<=ADC_Value[i])&&(ADC_Value[i]<1010))//����һ��
        {
            Interface[i]=16;    ////ģ��11......
        }
        else if((1010<=ADC_Value[i])&&(ADC_Value[i]<1100))//ǰ��һ��
        {
            Interface[i]=21;    ////ģ��11......
        }
        else if((1100<=ADC_Value[i])&&(ADC_Value[i]<1190))//��תֱ��
        {
            Interface[i]=20;    ////ģ��11......
        }
        else if((1190<=ADC_Value[i])&&(ADC_Value[i]<1275))//��תֱ��  ***
        {
            Interface[i]=18;    ////ģ��11......
        }
        else if((1275<=ADC_Value[i])&&(ADC_Value[i]<1360))//ǰ��ֱ��
        {
            Interface[i]=22;    ////ģ��11......
        }
        else if((1360<=ADC_Value[i])&&(ADC_Value[i]<1475))//ֹͣģ��
        {
            Interface[i]=23;    ////ģ��11......
        }
        else if((1475<=ADC_Value[i])&&(ADC_Value[i]<1575))//΢Ц����
        {
            Interface[i]=3;
        }
        else if((1575<=ADC_Value[i])&&(ADC_Value[i]<1655))//��������
        {
            Interface[i]=4;
        }
        else if((1655<=ADC_Value[i])&&(ADC_Value[i]<1745))//���ı���
        {
            Interface[i]=5;
        }
        else if((1745<=ADC_Value[i])&&(ADC_Value[i]<1900))//���ı���
        {
            Interface[i]=1;    //ģ��1
        }
        else if((1900<=ADC_Value[i])&&(ADC_Value[i]<2010))//��������
        {
            Interface[i]=2;  //ģ��2......
        }
        else if((2010<=ADC_Value[i])&&(ADC_Value[i]<2100))//��������
        {
            Interface[i]=12;    ////ģ��11......
        }
        else if((2100<=ADC_Value[i])&&(ADC_Value[i]<2200))//����������
        {
            Interface[i]=13;    ////ģ��11......
        }
        else if((2200<=ADC_Value[i])&&(ADC_Value[i]<2300))//������/������
        {
            Interface[i]=15;    ////ģ��11......
        }
        else if((2300<=ADC_Value[i])&&(ADC_Value[i]<2400))//��������
        {
            Interface[i]=14;    ////ģ��11......
        }
        else if((2400<=ADC_Value[i])&&(ADC_Value[i]<2490))//�绰����
        {
            Interface[i]=11;    ////ģ��11......
        }
        else if((2490<=ADC_Value[i])&&(ADC_Value[i]<2580))//����¼��
        {
            Interface[i]=6;
        }
        else if((2580<=ADC_Value[i])&&(ADC_Value[i]<2675))//����
        {
            Interface[i]=7;
        }
        else if((2675<=ADC_Value[i])&&(ADC_Value[i]<2790))//�ص�
        {
            Interface[i]=8;
        }
        else if((2790<=ADC_Value[i])&&(ADC_Value[i]<2950))//�������
        {
            Interface[i]=9;
        }
        else if((2950<=ADC_Value[i])&&(ADC_Value[i]<3120))//ֹͣ���
        {
            Interface[i]=10;
        }
        else if((3120<=ADC_Value[i])&&(ADC_Value[i]<3295))  //ǰ��2��
        {
            Interface[i]=25;    ////ģ��11......
        }
        /*****************���Կ�*********************/
        else if((3300<ADC_Value[i])&&(ADC_Value[i]<3430))//����ģ��
        {
            Interface[i]=99;    ////ģ��11......
        }
        else  //û��ģ��
        {
            Interface[i]=0;
        }
    }
}
void Special_Combination_Module(void)
{
    for(uint8_t i=0; i<8; i++)
    {
        uint8_t j=5;

        /**����ģ��**/
        emoji_Module( i, j);
        /**�������+��λ��***/
        Start_Motor_Potentiometer(i);
        /**��λ��+�˶�ģ��***/
        Start_Potentiometer(i);
        /**��бģ��+�˶�ģ��***/
        Tilt_Movement(i);
        /**ֱ��ģ��+ģ��**/
        straight_Movement(i);
        /**��ת90��ģ��+ģ��**/
        Right_Movement(i);
        /**��ת90��ģ��+ģ��**/
        Left_Movement(i);
        /**����**/
        voice_Module( i, j);
        /**����**/
        other_Module( i, j);
    }
    /***��������ָ�Ĭ�ϱ���***/
    Sendbuff[0]= 55;
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
/**�������+��ťģ��***/
void Start_Motor_Potentiometer(uint8_t i)
{
    if(Interface[i]==9)  //�������
    {
        if(Interface[i+1]==32)  //��������ťģ��
        {
            LED_Flashing_Time=0;
            while(KEY_Scan()!=1)
            {
                //nterface[i]==2��LED����˸
                LED_Flashing(i,0);
                /*****ADCֵλ��+9******/
                if(ADC_Value[i+9]<600)
                {
                    Sendbuff[0]= 42;
                    send_data_time();
                }
                else if((ADC_Value[i+9]>=600)&&(ADC_Value[i+9]<1200))
                {
                    Sendbuff[0]= 41;
                    send_data_time();
                }
                else if((ADC_Value[i+9]>=1200)&&(ADC_Value[i+9]<1800))
                {
                    Sendbuff[0]= 40;
                    send_data_time();
                }
                else if((ADC_Value[i+9]>=1800)&&(ADC_Value[i+9]<2300))
                {
                    Sendbuff[0]= 35;//���ֹͣ
                    send_data_time();
                }
                else if((ADC_Value[i+9]>=2300)&&(ADC_Value[i+9]<2900))
                {
                    Sendbuff[0]= 43;
                    send_data_time();
                }
                else if((ADC_Value[i+9]>=2900)&&(ADC_Value[i+9]<3500))
                {
                    Sendbuff[0]= 44;
                    send_data_time();
                }
                else if(ADC_Value[i+9]>=3500)
                {
                    Sendbuff[0]= 45;
                    send_data_time();
                }
            }
            Set_LED_Num=LED_times;
            GPIO_RESET(i);//��		Interface[i]==2��LED��
            GPIO_RESET(i);//��		Interface[i]==2��LED��
            GPIO_RESET(i);//��		Interface[i]==2��LED��
        }
        else
        {
            LED_Flashing_Time=0;
            Sendbuff[0]= 34;
            send_data_time();
            while(Set_LED_Num)
            {
                LED_Flashing(i,Set_LED_Num);
                if(KEY_Scan()==1)  //��������
                {
                    Set_LED_Num=LED_times;
                    GPIO_RESET(i);
                    break;
                }
            }
            Set_LED_Num=LED_times;
            GPIO_RESET(i);
        }
        Set_LED_Num=LED_times;
        HAL_Delay(200);
    }
}
/**��ťģ��+�˶�ģ��***/
void Start_Potentiometer(uint8_t i)
{
    if(Interface[i]==32)  //��ťģ��
    {
        if(Interface[i+1]==24)  //�������˶�ģ��
        {
            LED_Flashing_Time=0;
            while(KEY_Scan()!=1)
            {
                //nterface[i]==2��LED����˸
                LED_Flashing(i+1,0);
                /*****ADCֵλ��+8******/
                if(ADC_Value[i+8]<600)
                {
                    Sendbuff[0]= 48;
                    send_data_time();
                }
                else if((ADC_Value[i+8]>=600)&&(ADC_Value[i+8]<1200))
                {
                    Sendbuff[0]= 47;
                    send_data_time();
                }
                else if((ADC_Value[i+8]>=1200)&&(ADC_Value[i+8]<1800))
                {
                    Sendbuff[0]= 46;
                    send_data_time();
                }
                else if((ADC_Value[i+8]>=1800)&&(ADC_Value[i+8]<2300))
                {
                    Sendbuff[0]= 20;//С��ֹͣ
                    send_data_time();
                }
                else if((ADC_Value[i+8]>=2300)&&(ADC_Value[i+8]<2900))
                {
                    Sendbuff[0]= 49;
                    send_data_time();
                }
                else if((ADC_Value[i+8]>=2900)&&(ADC_Value[i+8]<3500))
                {
                    Sendbuff[0]= 50;
                    send_data_time();
                }
                else if(ADC_Value[i+8]>=3500)
                {
                    Sendbuff[0]= 51;
                    send_data_time();
                }
            }
            Set_LED_Num=LED_times;
            GPIO_RESET(i+1);//��		Interface[i]==2��LED��
            GPIO_RESET(i+1);//��		Interface[i]==2��LED��
            GPIO_RESET(i+1);//��		Interface[i]==2��LED��
        }
    }
}
/**��бģ��+�˶�ģ��**/
void Tilt_Movement(uint8_t i)
{
    if((Interface[i]==36)||((280<=ADC_Value[i])&&(ADC_Value[i]<420)))  //��бģ��
    {
        if(Interface[i+1]==24)  //�������˶�ģ��
        {
            LED_Flashing_Time=0;
            while(KEY_Scan()!=1)
            {
                if(ADC_Value[i+8]>4090)
                {
                    high_time[i]=0;
                }
                //nterface[i]==2��LED����˸
                LED_Flashing(i+1,0);
                Location=i+1;
                Gyro_Module();
                /*****PWMֵλ��+1******/
                if(high_time[i]==600)//��
                {
                    Sendbuff[0]= 17;
                    send_data_time();
                }
                else if(high_time[i]==700)//��
                {
                    Sendbuff[0]= 18;
                    send_data_time();
                }
                else if(high_time[i]==800)//ֱ��
                {
                    Sendbuff[0]= 19;
                    send_data_time();
                }
                else if(high_time[i]==900)//��
                {
                    Sendbuff[0]= 37;
                    send_data_time();
                }
                else if(high_time[i]==0)
                {
                    Sendbuff[0]= 20; //С��ͣ
                    send_data_time();
                }
            }
            GPIO_RESET(i+1);//��		Interface[i]==2��LED��
            GPIO_RESET(i+1);//��		Interface[i]==2��LED��
            GPIO_RESET(i+1);//��		Interface[i]==2��LED��
        }
    }
}
/**ֱ��ģ��+ģ��**/
void straight_Movement(uint8_t i)
{
    if(Interface[i]==22)  //ǰ��ֱ��ģ��
    {
        if((Interface[i+1]==31)||(Interface[i+1]==32)||(Interface[i+1]==33)||(Interface[i+1]==34)||(Interface[i+1]==35))  //���ģ��
        {
            LED_Flashing_Time=0;
            while(KEY_Scan()!=1)
            {
                Sendbuff[0]= 19;
                send_data_time();
                //nterface[i]==2��LED����˸
                LED_Flashing(i,0);
                /*****ADCֵλ��+9******/
                if(ADC_Value[i+9]<2048)
                {
                    if(ADC_Value[i+9]<2048)
                    {
                        GPIO_RESET(i);
                        break;
                    }
                }
            }
            GPIO_RESET(i);//��		Interface[i]==2��LED��
            HAL_Delay(50);
        }
        GPIO_RESET(i);//��		Interface[i]==2��LED��
    }
}
/**��תֱ��ģ��+ģ��**/
void Right_Movement(uint8_t i)
{
    if(Interface[i]==18)  //��תֱ��ģ��
    {
        if((Interface[i+1]==31)||(Interface[i+1]==32)||(Interface[i+1]==33)||(Interface[i+1]==34)||(Interface[i+1]==35))  //���ģ��
        {
            LED_Flashing_Time=0;
            while(KEY_Scan()!=1)
            {
                Sendbuff[0]= 18;
                send_data_time();
                //nterface[i]==2��LED����˸
                LED_Flashing(i,0);
                /*****ADCֵλ��+9******/
                if(ADC_Value[i+9]<2048)
                {
                    HAL_Delay(10);
                    if(ADC_Value[i+9]<2048)
                    {
                        GPIO_RESET(i);
                        break;
                    }
                }
            }
            GPIO_RESET(i);//��		Interface[i]==2��LED��
            HAL_Delay(50);
        }
        GPIO_RESET(i);//��		Interface[i]==2��LED��
    }
}
/**��תֱ��ģ��+ģ��**/
void Left_Movement(uint8_t i)
{
    if(Interface[i]==20)  //��תֱ��ģ��
    {
        if((Interface[i+1]==31)||(Interface[i+1]==32)||(Interface[i+1]==33)||(Interface[i+1]==34)||(Interface[i+1]==35))  //���ģ��
        {
            LED_Flashing_Time=0;
            while(KEY_Scan()!=1)
            {
                Sendbuff[0]= 17;
                send_data_time();
                //nterface[i]==2��LED����˸
                LED_Flashing(i,0);
                /*****ADCֵλ��+9******/
                if(ADC_Value[i+9]<2048)
                {
                    if(ADC_Value[i+9]<2048)
                    {
                        GPIO_RESET(i);
                        break;
                    }
                }
            }
            GPIO_RESET(i);//��		Interface[i]==2��LED��
            HAL_Delay(50);
        }
        GPIO_RESET(i);//��		Interface[i]==2��LED��
    }
}
/**����ģ��**/
void emoji_Module(uint8_t i,uint8_t j)
{
    if(Interface[i]==1)   //���ı���
    {
        LED_Flashing_Time=0;
        Sendbuff[0]= 24;
        send_data_time();
        Set_LED_Num=7;    //7��
        while(Set_LED_Num)
        {
            LED_Flashing(i,Set_LED_Num);
            if(KEY_Scan()==1)  //��������
            {
                Set_LED_Num=LED_times;
                GPIO_RESET(i);
                break;
            }
        }
        Set_LED_Num=LED_times;
        GPIO_RESET(i);
    }
    else if(Interface[i]==2)   //��������
    {
        LED_Flashing_Time=0;
        Sendbuff[0]= 25;
        send_data_time();
        Set_LED_Num=7;    //7��
        while(Set_LED_Num)
        {
            LED_Flashing(i,Set_LED_Num);
            if(KEY_Scan()==1)  //��������
            {
                Set_LED_Num=LED_times;
                GPIO_RESET(i);
                break;
            }
        }
        Set_LED_Num=LED_times;
        GPIO_RESET(i);
    }
    else if(Interface[i]==3)   //΢Ц����
    {
        LED_Flashing_Time=0;
        Sendbuff[0]= 21;
        send_data_time();
        Set_LED_Num=7;    //7��
        while(Set_LED_Num)
        {
            LED_Flashing(i,Set_LED_Num);
            if(KEY_Scan()==1)  //��������
            {
                Set_LED_Num=LED_times;
                GPIO_RESET(i);
                break;
            }
        }
        Set_LED_Num=LED_times;
        GPIO_RESET(i);
    }
    else if(Interface[i]==4)   //��������
    {
        LED_Flashing_Time=0;
        Sendbuff[0]= 22;//
        send_data_time();
        Set_LED_Num=7;    //7��
        while(Set_LED_Num)
        {
            LED_Flashing(i,Set_LED_Num);
            if(KEY_Scan()==1)  //��������
            {
                Set_LED_Num=LED_times;
                GPIO_RESET(i);
                break;
            }
        }
        Set_LED_Num=LED_times;
        GPIO_RESET(i);
    }
    else if(Interface[i]==5)   //���ı���
    {
        LED_Flashing_Time=0;
        Sendbuff[0]= 23;//
        send_data_time();
        Set_LED_Num=7;    //7��
        while(Set_LED_Num)
        {
            LED_Flashing(i,Set_LED_Num);
            if(KEY_Scan()==1)  //��������
            {
                Set_LED_Num=LED_times;
                GPIO_RESET(i);
                break;
            }
        }
        Set_LED_Num=LED_times;
        GPIO_RESET(i);
    }
}
/********����ģ��****************/
void voice_Module(uint8_t i,uint8_t j)
{
    if(Interface[i]==11)   //�绰����
    {
        LED_Flashing_Time=0;
        Sendbuff[0]= 30;
        Send_Data(Sendbuff);
        HAL_Delay(20);
        Send_Data(Sendbuff);
        Set_LED_Num=8;
        while(Set_LED_Num)
        {
            LED_Flashing(i,Set_LED_Num);
            if(KEY_Scan()==1)  //��������
            {
                Set_LED_Num=LED_times;
                GPIO_RESET(i);
                break;
            }
        }
        Set_LED_Num=LED_times;
        GPIO_RESET(i);
    }
    else if(Interface[i]==12)   //��������
    {
        LED_Flashing_Time=0;
        Sendbuff[0]= 26;
        Send_Data(Sendbuff);
        HAL_Delay(20);
        Send_Data(Sendbuff);
        Set_LED_Num=8;
        while(Set_LED_Num)
        {
            LED_Flashing(i,Set_LED_Num);
            if(KEY_Scan()==1)  //��������
            {
                Set_LED_Num=LED_times;
                GPIO_RESET(i);
                break;
            }
        }
        Set_LED_Num=LED_times;
        GPIO_RESET(i);
    }
    else if(Interface[i]==13)   //����������
    {
        LED_Flashing_Time=0;
        Sendbuff[0]= 27;
        Send_Data(Sendbuff);
        HAL_Delay(20);
        Send_Data(Sendbuff);
        Set_LED_Num=8;
        while(Set_LED_Num)
        {
            LED_Flashing(i,Set_LED_Num);
            if(KEY_Scan()==1)  //��������
            {
                Set_LED_Num=LED_times;
                GPIO_RESET(i);
                break;
            }
        }
        Set_LED_Num=LED_times;
        GPIO_RESET(i);
    }
    else if(Interface[i]==14)   //��������
    {
        LED_Flashing_Time=0;
        Sendbuff[0]= 29;
        Send_Data(Sendbuff);
        HAL_Delay(20);
        Send_Data(Sendbuff);
        Set_LED_Num=8;
        while(Set_LED_Num)
        {
            LED_Flashing(i,Set_LED_Num);
            if(KEY_Scan()==1)  //��������
            {
                Set_LED_Num=LED_times;
                GPIO_RESET(i);
                break;
            }
        }
        Set_LED_Num=LED_times;
        GPIO_RESET(i);
    }
    else if(Interface[i]==6)   //����¼��
    {
        LED_Flashing_Time=0;
        Sendbuff[0]= 31;
        Send_Data(Sendbuff);
        HAL_Delay(20);
        Send_Data(Sendbuff);
        Set_LED_Num=10;    //10��
        while(Set_LED_Num)
        {
            LED_Flashing(i,Set_LED_Num);
            if(KEY_Scan()==1)  //��������
            {
                Set_LED_Num=LED_times;
                GPIO_RESET(i);
                break;
            }
        }
        Set_LED_Num=LED_times;
        GPIO_RESET(i);
    }
    else if(Interface[i]==15)   //������/������
    {
        LED_Flashing_Time=0;
        Sendbuff[0]= 28;
        Send_Data(Sendbuff);
        HAL_Delay(20);
        Send_Data(Sendbuff);
        Set_LED_Num=8;
        while(Set_LED_Num)
        {
            LED_Flashing(i,Set_LED_Num);
            if(KEY_Scan()==1)  //��������
            {
                Set_LED_Num=LED_times;
                GPIO_RESET(i);
                break;
            }
        }
        Set_LED_Num=LED_times;
        GPIO_RESET(i);
    }
}
/************����ģ��************/
void other_Module(uint8_t i,uint8_t j)
{
    if(Interface[i]==10)   //ֹͣ���
    {
        LED_Flashing_Time=0;
        Set_LED_Num=LED_times;
        while(Set_LED_Num)
        {
            Sendbuff[0]= 35;
            send_data_time();
            LED_Flashing(i,Set_LED_Num);
            if(KEY_Scan()==1)  //��������
            {
                Set_LED_Num=LED_times;
                GPIO_RESET(i);
                break;
            }
        }
        Set_LED_Num=LED_times;
        GPIO_RESET(i);
    }
    else	if(Interface[i]==23)   //ֹͣģ��
    {
        LED_Flashing_Time=0;
        Sendbuff[0]= 20;
        send_data_time();
        Set_LED_Num=LED_times;
        while(Set_LED_Num)
        {
            LED_Flashing(i,Set_LED_Num);
            if(KEY_Scan()==1)  //��������
            {
                Set_LED_Num=LED_times;
                GPIO_RESET(i);
                break;
            }
        }
        Set_LED_Num=LED_times;
        GPIO_RESET(i);
    }
    else if(Interface[i]==7)   //����
    {
        LED_Flashing_Time=0;
        Sendbuff[0]= 32;
        Send_Data(Sendbuff);
        HAL_Delay(20);
        Send_Data(Sendbuff);
        Set_LED_Num=7;
        while(Set_LED_Num)
        {
            LED_Flashing(i,Set_LED_Num);
            if(KEY_Scan()==1)  //��������
            {
                Set_LED_Num=LED_times;
                GPIO_RESET(i);
                break;
            }
        }
        Set_LED_Num=LED_times;
        GPIO_RESET(i);
    }
    else if(Interface[i]==8)   //�ص�
    {
        LED_Flashing_Time=0;

        Sendbuff[0]= 33;
        Send_Data(Sendbuff);
        HAL_Delay(20);
        Send_Data(Sendbuff);
        while(Set_LED_Num)
        {
            LED_Flashing(i,Set_LED_Num);
            if(KEY_Scan()==1)  //��������
            {
                Set_LED_Num=LED_times;
                GPIO_RESET(i);
                break;
            }
        }
        Set_LED_Num=LED_times;
        GPIO_RESET(i);
    }
    else if(Interface[i]==16)   //����һ��
    {
        LED_Flashing_Time=0;
        /**/
        HAL_Delay(200);
        Sendbuff[0]= 15;
        Send_Data(Sendbuff);
        Send_Data(Sendbuff);
        HAL_Delay(100);
        Set_LED_Num=5;
        while(Set_LED_Num)
        {
            Sendbuff[0]= 0;
            LED_Flashing(i,Set_LED_Num);
            if(KEY_Scan()==1)  //��������
            {
                Set_LED_Num=LED_times;
                GPIO_RESET(i);
                break;
            }
        }
        Set_LED_Num=LED_times;
        GPIO_RESET(i);
    }
    else if(Interface[i]==17)   //��ת90
    {
        LED_Flashing_Time=0;
        HAL_Delay(200);
        Sendbuff[0]= 14;
        Send_Data(Sendbuff);
        Send_Data(Sendbuff);
        HAL_Delay(100);
        while(Set_LED_Num)
        {
            Sendbuff[0]= 0;
            LED_Flashing(i,Set_LED_Num);
            if(KEY_Scan()==1)  //��������
            {
                Set_LED_Num=LED_times;
                GPIO_RESET(i);
                break;
            }
        }
        Set_LED_Num=LED_times;
        GPIO_RESET(i);
    }
    else if(Interface[i]==19)   //��ת90
    {
        LED_Flashing_Time=0;
        HAL_Delay(200);
        Sendbuff[0]= 13;
        Send_Data(Sendbuff);
        Send_Data(Sendbuff);
        HAL_Delay(100);
        while(Set_LED_Num)
        {
            Sendbuff[0]= 0;
            LED_Flashing(i,Set_LED_Num);
            if(KEY_Scan()==1)  //��������
            {
                Set_LED_Num=LED_times;
                GPIO_RESET(i);
                break;
            }
        }
        Set_LED_Num=LED_times;
        GPIO_RESET(i);
    }
    else if(Interface[i]==21)   //ǰ��һ��
    {
        LED_Flashing_Time=0;
        HAL_Delay(200);
        Sendbuff[0]= 16;
        Send_Data(Sendbuff);
        Send_Data(Sendbuff);
        HAL_Delay(100);
        Set_LED_Num=5;
        while(Set_LED_Num)
        {
            LED_Flashing(i,Set_LED_Num);
            if(KEY_Scan()==1)  //��������
            {
                Set_LED_Num=LED_times;
                GPIO_RESET(i);
                break;
            }
        }
        Set_LED_Num=LED_times;
        Sendbuff[0]= 0;
        GPIO_RESET(i);
    }
    else if(Interface[i]==25)   //ǰ��2��
    {
        LED_Flashing_Time=0;
        HAL_Delay(200);
        Sendbuff[0]= 36;
        Send_Data(Sendbuff);
        Send_Data(Sendbuff);
        HAL_Delay(100);
        Set_LED_Num=9;
        while(Set_LED_Num)
        {
            LED_Flashing(i,Set_LED_Num);
            if(KEY_Scan()==1)  //��������
            {
                Set_LED_Num=LED_times;
                GPIO_RESET(i);
                break;
            }
        }
        Set_LED_Num=LED_times;
        Sendbuff[0]= 0;
        GPIO_RESET(i);
    }
}
/***************������ģ�����*******************************/
void Gyro_Module(void)
{
    switch(Location)  //������λ���ж�
    {
    case 1:   //λ��1
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
        if((Interface[i]<=30)||(Interface[i]==99))//99���Կ�
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
    /********************�ػ�*********************************/
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
    /*******************����****************************/
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
/**********�������***************/
uint8_t key_up=1;     //�����ɿ���־
uint8_t KEY_Scan(void)
{
    if(key_up&&(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_8)==0))
    {
        HAL_Delay(20);
        if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_8)==0)
        {
            key_up=0;
            return 1;
        }
    }
    else if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_8)==1)
    {
        key_up=1;
    }
    return 0;   //�ް�������
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
/****************�ж��Ƿ��пյı����********************/
uint8_t location;
uint8_t Programming_Error;
uint8_t Programming_Warning;
uint8_t LED_Glint;
uint8_t NO_Touch=9;
uint8_t Expression_flag=0;
uint8_t Light_flag=0;
uint8_t Recording_flag=0;
uint8_t Motor_flag=0;
uint8_t G_location[8]= {9};
uint8_t Light_times;

void Programming_Determine(void) //����ж�
{
    Programming_Error=0;
    Programming_Warning=0;
    for(uint8_t t=0; t<8; t++)
    {
        G_location[t]=9;
    }

    for(uint8_t i=0; i<8; i++)
    {
        Interface[i]=0;
    }
    Sendbuff[0]= 250;   //��ѯ������ID�˿�
    Send_Data(Sendbuff);
    HAL_Delay(10);
    Interface_module();
    for(uint8_t i=0; i<8; i++)
    {
        if(Interface[i]!=0)
        {
            location=i;
        }
        if(rx_buffer[0]==250)  //���յ���ȷ����
        {
            if((rx_buffer[1]!=32)&&(rx_buffer[2]!=32)&&(rx_buffer[3]!=32)&&(rx_buffer[4]!=32))//����û�б���ģ��
            {
                if((Interface[i]==1)||(Interface[i]==2)||(Interface[i]==3)||(Interface[i]==4)||(Interface[i]==5))//����ģ��
                {
                    Programming_Warning=1;
                    G_location[i]=i;
                }
            }
            if((rx_buffer[1]!=33)&&(rx_buffer[2]!=33)&&(rx_buffer[3]!=33)&&(rx_buffer[4]!=33))//�����е�
            {
                if((Interface[i]==7)||(Interface[i]==8))//��ģ��
                {
                    Programming_Warning=1;
                    G_location[i]=i;
                }
            }
            if((rx_buffer[1]!=34)&&(rx_buffer[2]!=34)&&(rx_buffer[3]!=34)&&(rx_buffer[4]!=34))//������¼��
            {

                if((Interface[i]==6)||(Interface[i]==11)||(Interface[i]==12)||(Interface[i]==13)||(Interface[i]==14)||(Interface[i]==15))//¼��ģ��
                {
                    Programming_Warning=1;
                    G_location[i]=i;
                }
            }
            if((rx_buffer[1]!=35)&&(rx_buffer[2]!=35)&&(rx_buffer[3]!=35)&&(rx_buffer[4]!=35))//���������
            {
                if((Interface[i]==9)||(Interface[i]==10))//���ģ��
                {
                    Programming_Warning=1;
                    G_location[i]=i;
                }
            }
        }
        /******ֱ��ģ��******/
        if((Interface[i]==18)||(Interface[i]==20)||(Interface[i]==22))
        {
            if((Interface[i+1]!=31)&&(Interface[i+1]!=32)&&(Interface[i+1]!=33)&&(Interface[i+1]!=34)&&(Interface[i+1]!=35)&&(Interface[i+1]!=36))
            {
                Programming_Warning=1;
                G_location[i]=i;
            }
        }
    }
    for(uint8_t j=0; j<location; j++)//�ж��Ƿ�Ӵ�
    {
        if(Interface[j]==0)
        {
            Programming_Error=1;
            NO_Touch=j;
            break ;
        }
    }
    if(Light_times>=3)
    {
        if((Programming_Error==1)||(Programming_Warning==1))
        {
            LED_Glint++;
        }
        Light_times=0;
    }
    if(Programming_Error==1) //��̴���
    {
        if(LED_Glint%2==0)
        {
            for(uint8_t k=0; k<=location; k++)
            {
                if(NO_Touch==k)
                {
                    GPIO_SET(k);
                }
                else
                {
                    GPIO_RESET(k);
                }
            }
        }
        else
        {
            for(uint8_t k=0; k<=7; k++)
            {
                GPIO_SET(k);
            }
        }
        if(LED_Glint>254)
        {
            LED_Glint=0;
        }
    }
    else if(Programming_Warning==1)  //��̾���
    {
        for(uint8_t k=0; k<=location; k++)
        {
            if(G_location[k]==k)
            {
                if(LED_Glint%2==0)
                {
                    GPIO_RESET(k);
                }
                else
                {
                    GPIO_SET(k);
                }
            }
            else
            {
                GPIO_RESET(k);
            }
            if(LED_Glint>254)
            {
                LED_Glint=0;
            }
        }
        if(LED_Glint>254)
        {
            LED_Glint=0;
        }
    }
    else  //�������
    {
        for(uint8_t k=0; k<=location; k++)
        {
            GPIO_RESET(k);
        }
    }
}




