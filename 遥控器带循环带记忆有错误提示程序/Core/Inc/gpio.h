/**
  ******************************************************************************
  * @file    gpio.h
  * @brief   This file contains all the function prototypes for
  *          the gpio.c file
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
#ifndef __GPIO_H__
#define __GPIO_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

void MX_GPIO_Init(void);

/* USER CODE BEGIN Prototypes */
void ID_GPIO_OUTPUT_Init(uint8_t ID);
void ID_GPIO_ADC_Init(uint8_t ID);
void ID_GPIO_PWM_Init(uint8_t ID);
void ID1_GPIO_OUTPUT_Init(void);
void ID2_GPIO_OUTPUT_Init(void);
void ID3_GPIO_OUTPUT_Init(void);
void ID4_GPIO_OUTPUT_Init(void);
void ID5_GPIO_OUTPUT_Init(void);
void ID6_GPIO_OUTPUT_Init(void);
void ID7_GPIO_OUTPUT_Init(void);
void ID8_GPIO_OUTPUT_Init(void);

void ID1_GPIO_ADC_Init(void);
void ID2_GPIO_ADC_Init(void);
void ID3_GPIO_ADC_Init(void);
void ID4_GPIO_ADC_Init(void);
void ID5_GPIO_ADC_Init(void);
void ID6_GPIO_ADC_Init(void);
void ID7_GPIO_ADC_Init(void);
void ID8_GPIO_ADC_Init(void);

void ID1_GPIO_PWM_Init(void);
void ID2_GPIO_PWM_Init(void);
void ID3_GPIO_PWM_Init(void);
void ID4_GPIO_PWM_Init(void);
void ID5_GPIO_PWM_Init(void);
void ID6_GPIO_PWM_Init(void);
void ID7_GPIO_PWM_Init(void);
void ID8_GPIO_PWM_Init(void);

void GPIO_SET(uint8_t i);
void GPIO_RESET(uint8_t i);

/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif
#endif /*__ GPIO_H__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
