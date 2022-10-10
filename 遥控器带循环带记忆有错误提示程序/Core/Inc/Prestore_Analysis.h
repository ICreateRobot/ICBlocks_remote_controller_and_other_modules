#ifndef __Prestore_Analysis_H
#define __Prestore_Analysis_H

#include "stm32f1xx_hal.h"

extern uint8_t Prestore_Data[99]; //预存数据
extern uint8_t Prestore_Times;       //预存数据轮次
extern uint8_t Data_Flag;
extern uint8_t IORD_Prestore_Times;    //程序执行8个模块执行到第几次
extern uint8_t Clear_KEY;
extern uint8_t IORD_Sort_Order;            //第几个循环块

void IOWR(void);  //预存指令模块写入
void IORD(uint8_t j);  //读取指令
void IORD_Running(uint8_t j);  //读取指令运行
void IORD_Analyse(uint8_t j);  //读取指令分析
uint8_t Loop_Number(void);  //循环个数

uint8_t IOWR_Judge(void);  //读取指令判断
uint8_t Module_Performs(uint8_t i,uint8_t j);
uint8_t Module_Judge(void);
void Starting_Procedure_With_IOWR(void);   //有读取模块的程序执行
uint8_t IORD_Loop_one(uint16_t Loop_Times,uint8_t Loop_Start,uint8_t Loop_Stop,uint8_t j);   //一个完整的循环
uint8_t Internal_Loop_Judgment(void);   //8个块里边是否有内循环模块
void Clear_Data(void); //清除数据
uint8_t Clear_KEY_DOWN(void); //清除数据


#endif


