/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __ADC_H__
#define __ADC_H__
#ifdef __cplusplus
extern "C" {
#endif
/* Includes ------------------------------------------------------------------*/
#include "main.h"
/* USER CODE BEGIN Includes */
/* USER CODE END Includes */
extern ADC_HandleTypeDef hadc1;
/* USER CODE BEGIN Private defines */
extern uint16_t  ADC_ValBuf[320];
extern uint16_t ADC_Value[16];
/* USER CODE END Private defines */
void MX_ADC1_Init(void);
/* USER CODE BEGIN Prototypes */
void Get_ADC_Averagevalue(void);
/* USER CODE END Prototypes */
#ifdef __cplusplus
}
#endif
#endif /* __ADC_H__ */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
