/**
  ******************************************************************************
  * @file    tim.h
  * @brief   This file contains all the function prototypes for
  *          the tim.c file
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __TIM_H__
#define __TIM_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim4;

/* USER CODE BEGIN Private defines */
extern uint16_t LED_Flashing_Time;
extern uint32_t high_time[8];   //高电平时间
/* USER CODE END Private defines */

void MX_TIM1_Init(void);
void MX_TIM4_Init(void);

/* USER CODE BEGIN Prototypes */
void MX_TIM2_Init(void);
void MX_TIM3_Init(void);

void pwm1_input(void);
void pwm2_input(void);
void pwm3_input(void);
void pwm4_input(void);
void pwm5_input(void);
void pwm6_input(void);
void pwm7_input(void);
void pwm8_input(void);
/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __TIM_H__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
