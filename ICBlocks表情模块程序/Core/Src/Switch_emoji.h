#ifndef __Switch_emoji_H
#define __Switch_emoji_H
#include "stm32f1xx_hal.h"



extern unsigned char BMP1[],BMP2[],BMP3[],BMP4[],BMP5[],BMP6[],BMP7[],BMP8[],BMP9[],BMP10[],
       BMP11[],BMP12[],BMP13[],BMP14[],BMP15[],BMP16[],BMP17[],BMP18[],BMP19[];

extern uint8_t  KEY_DOWN,KEY_DOWN1;
extern uint16_t time_up;
extern int8_t emoji_num,num_data;
void Read_KEY(void);


void num_display(void);
void emoji_display(void);



#endif
