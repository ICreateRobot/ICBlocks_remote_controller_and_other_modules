#include "Prestore_Analysis.h"
#include "Data_Analysis.h"
#include "main.h"
#include "adc.h"
#include "tim.h"
#include "gpio.h"
#include "Bluetooth.h"
/**************Ԥ���ȡָ���漰����**********************/
uint8_t Prestore_Data[99]= {0}; //Ԥ������
uint8_t Prestore_Times=0;       //Ԥ�������ִ�
uint8_t Data_Flag=0;
uint8_t IOWR_Flag=0;
#define LED_times 3  //LED��˸����LED_times+1
/*******************************************/
uint16_t IORD_Loop_number_Times[99]= {0}; //��n��ѭ������
uint8_t IORD_Loop_number_Start[99]= {0}; //��n��ѭ����ʼλ��
uint8_t IORD_Loop_number_Stop[99]= {0};  //��n��ѭ������λ��
uint16_t IORD_Loop_Times[49]= {0};
uint8_t IORD_Loop_Start[49]= {0},IORD_Loop_Stop[49]= {0}; //ѭ������/��ʼλ��/����λ��
uint8_t IORD_Sort_Order=0;            //�ڼ���ѭ����
uint8_t IORD_Order_Residue=0;        //ʣ�༸��ѭ����
uint8_t IORD_Order[5]= {0};
uint8_t Last_IORD_Sort_Order=0;    //��һ��Ԥ���ѭ������
uint8_t Last_Loop_number_Stop=0;   //�ϴ�Ԥ���ѭ������λ��
uint8_t IORD_Prestore_Times=0;    //����ִ��8��ģ��ִ�е��ڼ���
uint8_t IOWD_Prestore_Times=0;
uint8_t IORD_Sort_Order_S[99]= {0};
/******************ȷ��ģ��˳��***********************/
uint8_t Clear_KEY=0;
uint8_t IOWD_Nummber=0;
uint8_t Write_Finish=0;  //�������
uint8_t Write_Interface[99]= {0};
/*************���Դ����ģ��ָ��******************/
uint8_t Module_Judge()  //ģ��ָ���ж�  ȷ����Щ���Դ�����Щ�����Դ���  ����1 ���Դ���
{
    IOWR_Flag=IOWR_Judge();
//	Clear_KEY_DOWN();
    for(uint8_t i=0; i<8; i++)
    {
        if(((Interface[0]==37)||(Interface[7]==37))&&(Clear_KEY==1))
        {
            return 2;   //�����־
        }
        else if((Interface[7]==37)&&(IOWR_Flag==1))
        {
            return 1;  //���Դ����־
        }
        else if(Interface[7]==37)
        {
            return 3;  //��Ԥ��ģ���־
        }
    }
    return 0;
}
/***************************/
void IOWR()  //Ԥ��ָ��ģ��д��
{
    uint8_t k=0;
    if(Interface[7]==37)   //��Ԥ��ģ��
    {
        if((KEY_Scan()==2)&&(Module_Judge()==1)&&(Write_Finish==0))
        {
            IOWD_Prestore_Times+=1;

            for(uint8_t j=8*(IOWD_Prestore_Times-1); j<(8*IOWD_Prestore_Times-1); j++)
            {
                Prestore_Data[j]=Interface[k];  //������������
                Write_Interface[k]=Interface[k];
                k+=1;
            }
            Prestore_Times++;                   //Ԥ�������ִμ�һ
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
//            HAL_Delay(500);                     //�ȴ�500ms��׼����һ��Ԥ��
    }
}


void IORD(uint8_t j)  //��ȡָ��
{
    if(Interface[0]==37)  //��ȡָ��
    {
        for(IORD_Prestore_Times=IORD_Prestore_Times+1; IORD_Prestore_Times<=IOWD_Prestore_Times; IORD_Prestore_Times++)
        {
//        if(IORD_Sort_Order>0) //����ѭ��ģ������
            if(Internal_Loop_Judgment()==1)//����ѭ��ģ������
            {
//            Order_Residue=Sort_Order+1;
                for(uint8_t w=1; w<=IORD_Sort_Order_S[IORD_Prestore_Times]; w++)    //�м���ѭ��ģ��4
                {
                    Order_Residue--;
//                Loop_Times=Loop_number_Times[w];//�ڼ���ѭ��/ѭ������
//                Loop_Start=Loop_number_Start[w];//�ڼ���ѭ��/��ʼλ��
//                Loop_Stop=Loop_number_Stop[w];  //�ڼ���ѭ��/����λ��
                    IORD_Loop_one(IORD_Loop_Times[w-1+IORD_Prestore_Times], IORD_Loop_Start[w-1+IORD_Prestore_Times], IORD_Loop_Stop[w-1+IORD_Prestore_Times],j);
                }
                for(uint8_t y=IORD_Loop_Stop[IORD_Prestore_Times]+1; y<=(7*IORD_Prestore_Times); y++)  //ģ��ѭ����ʼ����
                {
                    if(Module_Performs(y,j)==1)
                    {
                        break;
                    }
                }
            }
            else  //����ѭ��
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
void IORD_Running(uint8_t j)  //��ȡָ������
{
    if(j==0)
    {
        if(Interface[j]==37)   //��ȡģ��
        {
            if(IORD_Sort_Order>0) //����ѭ��ģ������)
            {
                for(uint8_t w=1; w<=IORD_Sort_Order; w++)    //�м���ѭ��ģ��4
                {
                    IORD_Loop_one( IORD_Loop_Times[w],IORD_Loop_Start[w],IORD_Loop_Stop[w],j);
                    for(uint8_t z=1; z<=7; z++)
                    {
                        GPIO_SET(z);
                    }
                }
                for(uint8_t y=IORD_Loop_Stop[IORD_Sort_Order]+1; y<=(7*IOWD_Prestore_Times); y++)  //ģ��ѭ����ʼ����
                {
                    if(Module_Performs(y,j)==1)
                    {
                        break;
                    }
                }
            }
            else   //����ѭ��
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
void IORD_Analyse(uint8_t j)  //��ȡָ�����
{
    Loop_Number();   //�Ƿ�����ѭ���ж�

}
uint8_t Loop_Number()  //ѭ������
{
    for(uint8_t i=0; i<IOWD_Prestore_Times*8; i++)
    {
        if(Prestore_Data[i]==26)  //ѭ��һ��
        {
            IORD_Sort_Order+=1;
            IORD_Loop_Times[IORD_Sort_Order]=1;  //��n��ѭ������
            if(IORD_Sort_Order>1)
            {
                IORD_Loop_Start[IORD_Sort_Order]=IORD_Loop_Stop[IORD_Sort_Order-1]+2;  //��n��ѭ����ʼλ��
            }
            else
            {
                IORD_Loop_Start[IORD_Sort_Order]=0;  //��n��ѭ����ʼλ��
            }
            if(i>0)
            {
                IORD_Loop_Stop[IORD_Sort_Order]=i-1;   //��n��ѭ������λ��
            }
            else
            {
                IORD_Loop_Stop[IORD_Sort_Order]=i;   //��n��ѭ������λ��
            }
        }
        else if(Prestore_Data[i]==27)  //ѭ������
        {
            IORD_Sort_Order+=1;
            IORD_Loop_Times[IORD_Sort_Order]=2;  //��n��ѭ������
            if(IORD_Sort_Order>1)
            {
                IORD_Loop_Start[IORD_Sort_Order]=IORD_Loop_Stop[IORD_Sort_Order-1]+2;  //��n��ѭ����ʼλ��
            }
            else
            {
                IORD_Loop_Start[IORD_Sort_Order]=0;  //��n��ѭ����ʼλ��
            }
            if(i>0)
            {
                IORD_Loop_Stop[IORD_Sort_Order]=i-1;   //��n��ѭ������λ��
            }
            else
            {
                IORD_Loop_Stop[IORD_Sort_Order]=i;   //��n��ѭ������λ��
            }
        }
        else if(Prestore_Data[i]==28)  //ѭ������
        {
            IORD_Sort_Order+=1;
            IORD_Loop_Times[IORD_Sort_Order]=3;  //��n��ѭ������
            if(IORD_Sort_Order>1)
            {
                IORD_Loop_Start[IORD_Sort_Order]=IORD_Loop_Stop[IORD_Sort_Order-1]+2;  //��n��ѭ����ʼλ��
            }
            else
            {
                IORD_Loop_Start[IORD_Sort_Order]=0;  //��n��ѭ����ʼλ��
            }
            if(i>0)
            {
                IORD_Loop_Stop[IORD_Sort_Order]=i-1;   //��n��ѭ������λ��
            }
            else
            {
                IORD_Loop_Stop[IORD_Sort_Order]=i;   //��n��ѭ������λ��
            }
        }
        else if(Prestore_Data[i]==29)          //ѭ�����
        {
            IORD_Sort_Order+=1;
            IORD_Loop_Times[IORD_Sort_Order]=65530;  //��n��ѭ������
            if(IORD_Sort_Order>1)
            {
                IORD_Loop_Start[IORD_Sort_Order]=IORD_Loop_Stop[IORD_Sort_Order-1]+2;  //��n��ѭ����ʼλ��
            }
            else
            {
                IORD_Loop_Start[IORD_Sort_Order]=0;  //��n��ѭ����ʼλ��
            }
            if(i>0)
            {
                IORD_Loop_Stop[IORD_Sort_Order]=i-1;   //��n��ѭ������λ��
            }
            else
            {
                IORD_Loop_Stop[IORD_Sort_Order]=i;   //��n��ѭ������λ��
            }
        }

    }
    if(IORD_Sort_Order>0)
    {
        return 1;
    }
    return 0;
}
uint8_t Internal_Loop_Judgment()   //8��������Ƿ�����ѭ��ģ��
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

uint8_t IORD_Loop_one(uint16_t Loop_Times,uint8_t Loop_Start,uint8_t Loop_Stop,uint8_t j)   //һ��������ѭ��
{
    for(uint16_t t=0; t<Loop_Times; t++)            //ѭ������
    {
        if(Loop_Times>6100)//��ѭ��
        {
            t=0;
        }
        for(uint8_t y=Loop_Start; y<=Loop_Stop; y++)  //ģ��ѭ����ʼ/����
        {
            if(t<Loop_Times)
            {
                /**����ģ��**/
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
    /*****************����ģ��*****************/
    if(Prestore_Data[i]==1)   //���ı���
    {
        LED_Flashing_Time=0;
        Sendbuff[0]= 24;
        send_data_time();
        Set_LED_Num=7;    //7��
//        Turn_OFF_LED(j);
        while(Set_LED_Num)
        {
            LED_Flashing(j,Set_LED_Num);
            if(KEY_Scan()==1)  //��������
            {
                Set_LED_Num=LED_times;
//                GPIO_RESET(j);
                return 1;
            }
        }
        Set_LED_Num=LED_times;
//        GPIO_RESET(j);
    }
    else if(Prestore_Data[i]==2)   //��������
    {
        LED_Flashing_Time=0;
        Sendbuff[0]= 25;
        send_data_time();
        Set_LED_Num=7;    //7��
        Turn_OFF_LED(j);
        while(Set_LED_Num)
        {
            LED_Flashing(j,Set_LED_Num);
            if(KEY_Scan()==1)  //��������
            {
                Set_LED_Num=LED_times;
                GPIO_RESET(j);
                return 1;
            }
        }
        Set_LED_Num=LED_times;
        GPIO_RESET(j);
    }
    else if(Prestore_Data[i]==3)   //΢Ц����
    {
        LED_Flashing_Time=0;
        Sendbuff[0]= 21;
        send_data_time();
        Set_LED_Num=7;    //7��
//        Turn_OFF_LED(j);
        while(Set_LED_Num)
        {
            LED_Flashing(j,Set_LED_Num);
            if(KEY_Scan()==1)  //��������
            {
                Set_LED_Num=LED_times;
//                GPIO_RESET(j);
                return 1;
            }
        }
        Set_LED_Num=LED_times;
//        GPIO_RESET(j);
    }
    else if(Prestore_Data[i]==4)   //��������
    {
        LED_Flashing_Time=0;
        Sendbuff[0]= 22;//
        send_data_time();
        Set_LED_Num=7;    //7��
        Turn_OFF_LED(j);
        while(Set_LED_Num)
        {
            LED_Flashing(j,Set_LED_Num);
            if(KEY_Scan()==1)  //��������
            {
                Set_LED_Num=LED_times;
                GPIO_RESET(j);
                return 1;
            }
        }
        Set_LED_Num=LED_times;
        GPIO_RESET(j);
    }
    else if(Prestore_Data[i]==5)   //���ı���
    {
        LED_Flashing_Time=0;
        Sendbuff[0]= 23;//
        send_data_time();
        Set_LED_Num=7;    //7��
        Turn_OFF_LED(j);
        while(Set_LED_Num)
        {
            LED_Flashing(j,Set_LED_Num);
            if(KEY_Scan()==1)  //��������
            {
                Set_LED_Num=LED_times;
                GPIO_RESET(j);
                return 1;
            }
        }
        Set_LED_Num=LED_times;
        GPIO_RESET(j);
    }
    /***************����ģ��********************/
    else if(Prestore_Data[i]==11)   //�绰����
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
            if(KEY_Scan()==1)  //��������
            {
                Set_LED_Num=LED_times;
                GPIO_RESET(j);
                return 1;
            }
        }
        Set_LED_Num=LED_times;
        GPIO_RESET(j);
    }
    else if(Prestore_Data[i]==12)   //��������
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
            if(KEY_Scan()==1)  //��������
            {
                Set_LED_Num=LED_times;
                GPIO_RESET(j);
                return 1;
            }
        }
        Set_LED_Num=LED_times;
        GPIO_RESET(j);
    }
    else if(Prestore_Data[i]==13)   //����������
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
            if(KEY_Scan()==1)  //��������
            {
                Set_LED_Num=LED_times;
                GPIO_RESET(j);
                return 1;
            }
        }
        Set_LED_Num=LED_times;
        GPIO_RESET(j);
    }
    else if(Prestore_Data[i]==14)   //��������
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
            if(KEY_Scan()==1)  //��������
            {
                Set_LED_Num=LED_times;
                GPIO_RESET(j);
                return 1;
            }
        }
        Set_LED_Num=LED_times;
        GPIO_RESET(j);
    }
    else if(Prestore_Data[i]==6)   //����¼��
    {
        LED_Flashing_Time=0;
        Sendbuff[0]= 31;
        Send_Data(Sendbuff);
//        HAL_Delay(20);
//        Send_Data(Sendbuff);
        Set_LED_Num=10;    //10��
        Turn_OFF_LED(j);
        while(Set_LED_Num)
        {
            LED_Flashing(j,Set_LED_Num);
            if(KEY_Scan()==1)  //��������
            {
                Set_LED_Num=LED_times;
                GPIO_RESET(j);
                return 1;
            }
        }
        Set_LED_Num=LED_times;
        GPIO_RESET(j);
    }
    else if(Prestore_Data[i]==15)   //������/������
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
            if(KEY_Scan()==1)  //��������
            {
                Set_LED_Num=LED_times;
                GPIO_RESET(j);
                return 1;
            }
        }
        Set_LED_Num=LED_times;
        GPIO_RESET(j);
    }
    /**********�˶���ģ��**********************/
    else if(Prestore_Data[i]==10)   //ֹͣ���
    {
        LED_Flashing_Time=0;
        Set_LED_Num=LED_times;
        Turn_OFF_LED(j);
        while(Set_LED_Num)
        {
            Sendbuff[0]= 35;
            send_data_time();
            LED_Flashing(j,Set_LED_Num);
            if(KEY_Scan()==1)  //��������
            {
                Set_LED_Num=LED_times;
                GPIO_RESET(j);
                return 1;
            }
        }
        Set_LED_Num=LED_times;
        GPIO_RESET(j);
    }
    else	if(Prestore_Data[i]==9)   //�������
    {
        LED_Flashing_Time=0;
        Sendbuff[0]= 34;
        send_data_time();
        Set_LED_Num=LED_times;
        Turn_OFF_LED(j);
        while(Set_LED_Num)
        {
            LED_Flashing(j,Set_LED_Num);
            if(KEY_Scan()==1)  //��������
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
            if(KEY_Scan()==1)  //��������
            {
                Set_LED_Num=LED_times;
                GPIO_RESET(j);
                return 1;
            }
        }
        Set_LED_Num=LED_times;
        GPIO_RESET(j);
    }
    else	if(Prestore_Data[i]==23)   //ֹͣģ��
    {
        LED_Flashing_Time=0;
        Sendbuff[0]= 20;
        send_data_time();
        Set_LED_Num=LED_times;
        Turn_OFF_LED(j);
        while(Set_LED_Num)
        {
            LED_Flashing(j,Set_LED_Num);
            if(KEY_Scan()==1)  //��������
            {
                Set_LED_Num=LED_times;
                GPIO_RESET(j);
                return 1;
            }
        }
        Set_LED_Num=LED_times;
        GPIO_RESET(j);
    }
    else if(Prestore_Data[i]==7)   //����
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
            if(KEY_Scan()==1)  //��������
            {
                Set_LED_Num=LED_times;
                GPIO_RESET(j);
                return 1;
            }
        }
        Set_LED_Num=LED_times;
        GPIO_RESET(j);
    }
    else if(Prestore_Data[i]==8)   //�ص�
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
            if(KEY_Scan()==1)  //��������
            {
                Set_LED_Num=LED_times;
                GPIO_RESET(j);
                return 1;
            }
        }
        Set_LED_Num=LED_times;
        GPIO_RESET(j);
    }
    else if(Prestore_Data[i]==16)   //����һ��
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
            if(KEY_Scan()==1)  //��������
            {
                Set_LED_Num=LED_times;
                GPIO_RESET(j);
                return 1;
            }
        }
        Set_LED_Num=LED_times;
        GPIO_RESET(j);
    }
    else if(Prestore_Data[i]==17)   //��ת90
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
            if(KEY_Scan()==1)  //��������
            {
                Set_LED_Num=LED_times;
                GPIO_RESET(j);
                return 1;
            }
        }
        Set_LED_Num=LED_times;
        GPIO_RESET(j);
    }
    else if(Prestore_Data[i]==19)   //��ת90
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
            if(KEY_Scan()==1)  //��������
            {
                Set_LED_Num=LED_times;
//                GPIO_RESET(j);
                return 1;
            }
        }
        Set_LED_Num=LED_times;
//        GPIO_RESET(j);
    }
    else if(Prestore_Data[i]==21)   //ǰ��һ��
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
            if(KEY_Scan()==1)  //��������
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
    else if(Prestore_Data[i]==25)   //ǰ��2��
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
            if(KEY_Scan()==1)  //��������
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
uint8_t IOWR_Judge()  //��ȡָ���ж�
{


    if((Interface[0]==Write_Interface[0])&&(Interface[1]==Write_Interface[1])&&(Interface[2]==Write_Interface[2])&&(Interface[3]==Write_Interface[3])&&
            (Interface[4]==Write_Interface[4])&&(Interface[5]==Write_Interface[5])&&(Interface[6]==Write_Interface[6]))
    {
        Write_Finish=1;
        return 0;  //�����Դ����־
    }
    for(uint8_t i=0; i<8; i++)
    {
        if((Interface[i]==18)||(Interface[i]==20)||(Interface[i]==22)||(Interface[i]==31)||(Interface[i]==32)||(Interface[i]==33)||(Interface[i]==34)||
                (Interface[i]==35)||(Interface[i]==36)||(Interface[i]==38))//(Interface[i]==26)||(Interface[i]==27)||(Interface[i]==28)||(Interface[i]==29)||
        {
            Write_Finish=1;
            return 0;  //�����Դ����־
        }
    }
    Write_Finish=0;
    return 1;
}


void Starting_Procedure_With_IOWR(void)   //�ж�ȡģ��ĳ���ִ��
{
    for(uint8_t i=0; i<8; i++)
    {
//        Module_Performs(i,j);
    }
    /***��������ָ�Ĭ�ϱ���***/
//    Sendbuff[0]= 55;   //��ֹ��
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
uint8_t Clear_KEY_DOWN() //�������
{
//    Prestore_Times=0;
//    for(uint8_t k=0; k<99; k++) //��������
//    {
//        Prestore_Data[k]=0;
//    }
    if(key_up==0)
    {
        Long_key_up++;
        if(Long_key_up>=80) //��������ʱ��=80*50ms=4s
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
void Clear_Data() //�������
{
    Prestore_Times=0;
    IORD_Sort_Order=0;
    IOWD_Prestore_Times=0;
    Write_Finish=1;
    for(uint8_t i=0; i<7; i++)
    {
        Write_Interface[i]=0;
    }
    for(uint8_t k=0; k<99; k++) //��������
    {
        Prestore_Data[k]=0;
    }
    for(uint8_t k=0; k<49; k++) //��������
    {
        IORD_Loop_Times[k]=0;
        IORD_Loop_Start[k]=0;
        IORD_Loop_Stop[k]=0;
    }
    Clear_KEY=0;//����
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



