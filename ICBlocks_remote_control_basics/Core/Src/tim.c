/* Includes ------------------------------------------------------------------*/
#include "tim.h"
/* USER CODE BEGIN 0 */
#include "Data_Analysis.h"
uint32_t capture_Buf[8][3] = {0};   //存放计数值
uint8_t capture_Cnt[8]= {0};    //状态标志位
uint32_t high_time[8];   //高电平时间
uint16_t LED_Flashing_Time;
//uint8_t Location;
TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim3;
/* USER CODE END 0 */
TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim4;
/* TIM1 init function */
void MX_TIM1_Init(void)
{
    /* USER CODE BEGIN TIM1_Init 0 */
    /* USER CODE END TIM1_Init 0 */
    TIM_ClockConfigTypeDef sClockSourceConfig = {0};
    TIM_MasterConfigTypeDef sMasterConfig = {0};
    /* USER CODE BEGIN TIM1_Init 1 */
    /* USER CODE END TIM1_Init 1 */
    htim1.Instance = TIM1;
    htim1.Init.Prescaler = 7199;
    htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim1.Init.Period = 999;
    htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    htim1.Init.RepetitionCounter = 0;
    htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
    if (HAL_TIM_Base_Init(&htim1) != HAL_OK)
    {
        Error_Handler();
    }
    sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
    if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK)
    {
        Error_Handler();
    }
    sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
    sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
    if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
    {
        Error_Handler();
    }
    /* USER CODE BEGIN TIM1_Init 2 */
    /* USER CODE END TIM1_Init 2 */
}
/* TIM4 init function */
void MX_TIM4_Init(void)
{
    /* USER CODE BEGIN TIM4_Init 0 */
    /* USER CODE END TIM4_Init 0 */
    TIM_ClockConfigTypeDef sClockSourceConfig = {0};
    TIM_MasterConfigTypeDef sMasterConfig = {0};
    /* USER CODE BEGIN TIM4_Init 1 */
    /* USER CODE END TIM4_Init 1 */
    htim4.Instance = TIM4;
    htim4.Init.Prescaler = 7199;
    htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim4.Init.Period = 499;
    htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    htim4.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
    if (HAL_TIM_Base_Init(&htim4) != HAL_OK)
    {
        Error_Handler();
    }
    sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
    if (HAL_TIM_ConfigClockSource(&htim4, &sClockSourceConfig) != HAL_OK)
    {
        Error_Handler();
    }
    sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
    sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
    if (HAL_TIMEx_MasterConfigSynchronization(&htim4, &sMasterConfig) != HAL_OK)
    {
        Error_Handler();
    }
    /* USER CODE BEGIN TIM4_Init 2 */
    /* USER CODE END TIM4_Init 2 */
}
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* tim_baseHandle)
{
    if(tim_baseHandle->Instance==TIM1)
    {
        /* USER CODE BEGIN TIM1_MspInit 0 */
        /* USER CODE END TIM1_MspInit 0 */
        /* TIM1 clock enable */
        __HAL_RCC_TIM1_CLK_ENABLE();
        /* TIM1 interrupt Init */
        HAL_NVIC_SetPriority(TIM1_UP_IRQn, 0, 1);
        HAL_NVIC_EnableIRQ(TIM1_UP_IRQn);
        /* USER CODE BEGIN TIM1_MspInit 1 */
        /* USER CODE END TIM1_MspInit 1 */
    }
    else if(tim_baseHandle->Instance==TIM4)
    {
        /* USER CODE BEGIN TIM4_MspInit 0 */
        /* USER CODE END TIM4_MspInit 0 */
        /* TIM4 clock enable */
        __HAL_RCC_TIM4_CLK_ENABLE();
        /* TIM4 interrupt Init */
        HAL_NVIC_SetPriority(TIM4_IRQn, 0, 1);
        HAL_NVIC_EnableIRQ(TIM4_IRQn);
        /* USER CODE BEGIN TIM4_MspInit 1 */
        /* USER CODE END TIM4_MspInit 1 */
    }
}
void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef* tim_baseHandle)
{
    if(tim_baseHandle->Instance==TIM1)
    {
        /* USER CODE BEGIN TIM1_MspDeInit 0 */
        /* USER CODE END TIM1_MspDeInit 0 */
        /* Peripheral clock disable */
        __HAL_RCC_TIM1_CLK_DISABLE();
        /* TIM1 interrupt Deinit */
        HAL_NVIC_DisableIRQ(TIM1_UP_IRQn);
        /* USER CODE BEGIN TIM1_MspDeInit 1 */
        /* USER CODE END TIM1_MspDeInit 1 */
    }
    else if(tim_baseHandle->Instance==TIM4)
    {
        /* USER CODE BEGIN TIM4_MspDeInit 0 */

        /* USER CODE END TIM4_MspDeInit 0 */
        /* Peripheral clock disable */
        __HAL_RCC_TIM4_CLK_DISABLE();
        /* TIM4 interrupt Deinit */
        HAL_NVIC_DisableIRQ(TIM4_IRQn);
        /* USER CODE BEGIN TIM4_MspDeInit 1 */
        /* USER CODE END TIM4_MspDeInit 1 */
    }
}
/* USER CODE BEGIN 1 */
/* TIM2 init function */
void MX_TIM2_Init(void)
{
    /* USER CODE BEGIN TIM2_Init 0 */
    /* USER CODE END TIM2_Init 0 */
    TIM_MasterConfigTypeDef sMasterConfig = {0};
    TIM_IC_InitTypeDef sConfigIC = {0};
    /* USER CODE BEGIN TIM2_Init 1 */
    /* USER CODE END TIM2_Init 1 */
    htim2.Instance = TIM2;
    htim2.Init.Prescaler = 71;
    htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim2.Init.Period = 65535;
    htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    if (HAL_TIM_IC_Init(&htim2) != HAL_OK)
    {
        Error_Handler();
    }
    sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
    sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
    if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
    {
        Error_Handler();
    }
    sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_RISING;
    sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
    sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;
    sConfigIC.ICFilter = 0;
    if (HAL_TIM_IC_ConfigChannel(&htim2, &sConfigIC, TIM_CHANNEL_1) != HAL_OK)
    {
        Error_Handler();
    }
    if (HAL_TIM_IC_ConfigChannel(&htim2, &sConfigIC, TIM_CHANNEL_2) != HAL_OK)
    {
        Error_Handler();
    }
    if (HAL_TIM_IC_ConfigChannel(&htim2, &sConfigIC, TIM_CHANNEL_3) != HAL_OK)
    {
        Error_Handler();
    }
    if (HAL_TIM_IC_ConfigChannel(&htim2, &sConfigIC, TIM_CHANNEL_4) != HAL_OK)
    {
        Error_Handler();
    }
    /* USER CODE BEGIN TIM2_Init 2 */
    /* USER CODE END TIM2_Init 2 */
}
/* TIM3 init function */
void MX_TIM3_Init(void)
{
    /* USER CODE BEGIN TIM3_Init 0 */
    /* USER CODE END TIM3_Init 0 */
    TIM_MasterConfigTypeDef sMasterConfig = {0};
    TIM_IC_InitTypeDef sConfigIC = {0};
    /* USER CODE BEGIN TIM3_Init 1 */
    /* USER CODE END TIM3_Init 1 */
    htim3.Instance = TIM3;
    htim3.Init.Prescaler = 71;
    htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim3.Init.Period = 65535;
    htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    if (HAL_TIM_IC_Init(&htim3) != HAL_OK)
    {
        Error_Handler();
    }
    sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
    sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
    if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
    {
        Error_Handler();
    }
    sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_RISING;
    sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
    sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;
    sConfigIC.ICFilter = 0;
    if (HAL_TIM_IC_ConfigChannel(&htim3, &sConfigIC, TIM_CHANNEL_1) != HAL_OK)
    {
        Error_Handler();
    }
    if (HAL_TIM_IC_ConfigChannel(&htim3, &sConfigIC, TIM_CHANNEL_2) != HAL_OK)
    {
        Error_Handler();
    }
    if (HAL_TIM_IC_ConfigChannel(&htim3, &sConfigIC, TIM_CHANNEL_3) != HAL_OK)
    {
        Error_Handler();
    }
    if (HAL_TIM_IC_ConfigChannel(&htim3, &sConfigIC, TIM_CHANNEL_4) != HAL_OK)
    {
        Error_Handler();
    }
    /* USER CODE BEGIN TIM3_Init 2 */
    /* USER CODE END TIM3_Init 2 */
}
void HAL_TIM_IC_MspInit(TIM_HandleTypeDef* tim_icHandle)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    if(tim_icHandle->Instance==TIM2)
    {
        /* USER CODE BEGIN TIM2_MspInit 0 */
        /* USER CODE END TIM2_MspInit 0 */
        /* TIM2 clock enable */
        __HAL_RCC_TIM2_CLK_ENABLE();
        __HAL_RCC_GPIOA_CLK_ENABLE();
        /**TIM2 GPIO Configuration
        PA0-WKUP     ------> TIM2_CH1
        PA1     ------> TIM2_CH2
        PA2     ------> TIM2_CH3
        PA3     ------> TIM2_CH4
        */
        GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3;
        GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
        GPIO_InitStruct.Pull = GPIO_PULLUP;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
        /* TIM2 interrupt Init */
        HAL_NVIC_SetPriority(TIM2_IRQn, 0, 0);
        HAL_NVIC_EnableIRQ(TIM2_IRQn);
        /* USER CODE BEGIN TIM2_MspInit 1 */
        /* USER CODE END TIM2_MspInit 1 */
    }
    else if(tim_icHandle->Instance==TIM3)
    {
        /* USER CODE BEGIN TIM3_MspInit 0 */
        /* USER CODE END TIM3_MspInit 0 */
        /* TIM3 clock enable */
        __HAL_RCC_TIM3_CLK_ENABLE();
        __HAL_RCC_GPIOA_CLK_ENABLE();
        __HAL_RCC_GPIOB_CLK_ENABLE();
        /**TIM3 GPIO Configuration
        PA6     ------> TIM3_CH1
        PA7     ------> TIM3_CH2
        PB0     ------> TIM3_CH3
        PB1     ------> TIM3_CH4
        */
        GPIO_InitStruct.Pin = GPIO_PIN_6|GPIO_PIN_7;
        GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
        GPIO_InitStruct.Pull = GPIO_PULLUP;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
        GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1;
        GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
        GPIO_InitStruct.Pull = GPIO_PULLUP;
        HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
        /* TIM3 interrupt Init */
        HAL_NVIC_SetPriority(TIM3_IRQn, 0, 0);
        HAL_NVIC_EnableIRQ(TIM3_IRQn);
        /* USER CODE BEGIN TIM3_MspInit 1 */
        /* USER CODE END TIM3_MspInit 1 */
    }
}
void HAL_TIM_IC_MspDeInit(TIM_HandleTypeDef* tim_icHandle)
{
    if(tim_icHandle->Instance==TIM2)
    {
        /* USER CODE BEGIN TIM2_MspDeInit 0 */
        /* USER CODE END TIM2_MspDeInit 0 */
        /* Peripheral clock disable */
        __HAL_RCC_TIM2_CLK_DISABLE();
        /**TIM2 GPIO Configuration
        PA0-WKUP     ------> TIM2_CH1
        PA1     ------> TIM2_CH2
        PA2     ------> TIM2_CH3
        PA3     ------> TIM2_CH4
        */
        HAL_GPIO_DeInit(GPIOA, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);
        /* TIM2 interrupt Deinit */
        HAL_NVIC_DisableIRQ(TIM2_IRQn);
        /* USER CODE BEGIN TIM2_MspDeInit 1 */
        /* USER CODE END TIM2_MspDeInit 1 */
    }
    else if(tim_icHandle->Instance==TIM3)
    {
        /* USER CODE BEGIN TIM3_MspDeInit 0 */
        /* USER CODE END TIM3_MspDeInit 0 */
        /* Peripheral clock disable */
        __HAL_RCC_TIM3_CLK_DISABLE();
        /**TIM3 GPIO Configuration
        PA6     ------> TIM3_CH1
        PA7     ------> TIM3_CH2
        PB0     ------> TIM3_CH3
        PB1     ------> TIM3_CH4
        */
        HAL_GPIO_DeInit(GPIOA, GPIO_PIN_6|GPIO_PIN_7);
        HAL_GPIO_DeInit(GPIOB, GPIO_PIN_0|GPIO_PIN_1);
        /* TIM3 interrupt Deinit */
        HAL_NVIC_DisableIRQ(TIM3_IRQn);
        /* USER CODE BEGIN TIM3_MspDeInit 1 */
        /* USER CODE END TIM3_MspDeInit 1 */
    }
}
void pwm1_input()
{
    switch (capture_Cnt[0])
    {
    case 0:
        capture_Cnt[0]++;
        __HAL_TIM_SET_CAPTUREPOLARITY(&htim2, TIM_CHANNEL_1, TIM_INPUTCHANNELPOLARITY_RISING);
        HAL_TIM_IC_Start_IT(&htim2, TIM_CHANNEL_1);	//启动输入捕获
        break;
    case 3:
        high_time[0] = capture_Buf[0][1]- capture_Buf[0][0];    //高电平时间
        capture_Cnt[0] = 0;  //清空标志位
        break;
    }
}
void pwm2_input()
{
    switch (capture_Cnt[1])
    {
    case 0:
        capture_Cnt[1]++;
        __HAL_TIM_SET_CAPTUREPOLARITY(&htim2, TIM_CHANNEL_2, TIM_INPUTCHANNELPOLARITY_RISING);
        HAL_TIM_IC_Start_IT(&htim2, TIM_CHANNEL_2);	//启动输入捕获
        break;
    case 3:
        high_time[1] = capture_Buf[1][1]- capture_Buf[1][0];    //高电平时间
        capture_Cnt[1] = 0;  //清空标志位
        break;
    }
}
void pwm3_input()
{
    switch (capture_Cnt[2])
    {
    case 0:
        capture_Cnt[2]++;
        __HAL_TIM_SET_CAPTUREPOLARITY(&htim2, TIM_CHANNEL_3, TIM_INPUTCHANNELPOLARITY_RISING);
        HAL_TIM_IC_Start_IT(&htim2, TIM_CHANNEL_3);	//启动输入捕获
        break;
    case 3:
        high_time[2] = capture_Buf[2][1]- capture_Buf[2][0];    //高电平时间
        capture_Cnt[2] = 0;  //清空标志位
        break;
    }
}
void pwm4_input()
{
    switch (capture_Cnt[3])
    {
    case 0:
        capture_Cnt[3]++;
        __HAL_TIM_SET_CAPTUREPOLARITY(&htim2, TIM_CHANNEL_4, TIM_INPUTCHANNELPOLARITY_RISING);
        HAL_TIM_IC_Start_IT(&htim2, TIM_CHANNEL_4);	//启动输入捕获
        break;
    case 3:
        high_time[3] = capture_Buf[3][1]- capture_Buf[3][0];    //高电平时间
        capture_Cnt[3] = 0;  //清空标志位
        break;
    }
}
void pwm5_input()
{
    switch (capture_Cnt[4])
    {
    case 0:
        capture_Cnt[4]++;
        __HAL_TIM_SET_CAPTUREPOLARITY(&htim3, TIM_CHANNEL_1, TIM_INPUTCHANNELPOLARITY_RISING);
        HAL_TIM_IC_Start_IT(&htim3, TIM_CHANNEL_1);	//启动输入捕获
        break;
    case 3:
        high_time[4] = capture_Buf[4][1]- capture_Buf[4][0];    //高电平时间
        capture_Cnt[4] = 0;  //清空标志位
        break;
    }
}
void pwm6_input()
{
    switch (capture_Cnt[5])
    {
    case 0:
        capture_Cnt[5]++;
        __HAL_TIM_SET_CAPTUREPOLARITY(&htim3, TIM_CHANNEL_2, TIM_INPUTCHANNELPOLARITY_RISING);
        HAL_TIM_IC_Start_IT(&htim3, TIM_CHANNEL_2);	//启动输入捕获
        break;
    case 3:
        high_time[5] = capture_Buf[5][1]- capture_Buf[5][0];    //高电平时间
        capture_Cnt[5] = 0;  //清空标志位
        break;
    }
}
void pwm7_input()
{
    switch (capture_Cnt[6])
    {
    case 0:
        capture_Cnt[6]++;
        __HAL_TIM_SET_CAPTUREPOLARITY(&htim3, TIM_CHANNEL_3, TIM_INPUTCHANNELPOLARITY_RISING);
        HAL_TIM_IC_Start_IT(&htim3, TIM_CHANNEL_3);	//启动输入捕获
        break;
    case 3:
        high_time[6] = capture_Buf[6][1]- capture_Buf[6][0];    //高电平时间
        capture_Cnt[6] = 0;  //清空标志位
        break;
    }
}
void pwm8_input()
{
    switch (capture_Cnt[7])
    {
    case 0:
        capture_Cnt[7]++;
        __HAL_TIM_SET_CAPTUREPOLARITY(&htim3, TIM_CHANNEL_4, TIM_INPUTCHANNELPOLARITY_RISING);
        HAL_TIM_IC_Start_IT(&htim3, TIM_CHANNEL_4);	//启动输入捕获
        break;
    case 3:
        high_time[7] = capture_Buf[7][1]- capture_Buf[7][0];    //高电平时间
        capture_Cnt[7] = 0;  //清空标志位
        break;
    }
}
/*****************输入捕获回调函数*******************************/
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
    switch(Location)
    {
    case 1:
        if(TIM2 == htim->Instance)
        {
            switch(capture_Cnt[0])
            {
            case 1:
                capture_Buf[0][0] = HAL_TIM_ReadCapturedValue(&htim2,TIM_CHANNEL_1);//获取当前的捕获值.
                __HAL_TIM_SET_CAPTUREPOLARITY(&htim2,TIM_CHANNEL_1,TIM_ICPOLARITY_FALLING);  //设置为下降沿捕获
                capture_Cnt[0]++;
                break;
            case 2:
                capture_Buf[0][1] = HAL_TIM_ReadCapturedValue(&htim2,TIM_CHANNEL_1);//获取当前的捕获值.
                HAL_TIM_IC_Stop_IT(&htim2,TIM_CHANNEL_1); //停止捕获   或者: __HAL_TIM_DISABLE(&htim5);
                capture_Cnt[0]++;
            }
        }
        break;
    case 2:
        if(TIM2 == htim->Instance)
        {
            switch(capture_Cnt[1])
            {
            case 1:
                capture_Buf[1][0] = HAL_TIM_ReadCapturedValue(&htim2,TIM_CHANNEL_2);//获取当前的捕获值.
                __HAL_TIM_SET_CAPTUREPOLARITY(&htim2,TIM_CHANNEL_2,TIM_ICPOLARITY_FALLING);  //设置为下降沿捕获
                capture_Cnt[1]++;
                break;
            case 2:
                capture_Buf[1][1] = HAL_TIM_ReadCapturedValue(&htim2,TIM_CHANNEL_2);//获取当前的捕获值.
                HAL_TIM_IC_Stop_IT(&htim2,TIM_CHANNEL_2); //停止捕获   或者: __HAL_TIM_DISABLE(&htim5);
                capture_Cnt[1]++;
            }
        }
        break;
    case 3:
        if(TIM2 == htim->Instance)
        {
            switch(capture_Cnt[2])
            {
            case 1:
                capture_Buf[2][0] = HAL_TIM_ReadCapturedValue(&htim2,TIM_CHANNEL_3);//获取当前的捕获值.
                __HAL_TIM_SET_CAPTUREPOLARITY(&htim2,TIM_CHANNEL_3,TIM_ICPOLARITY_FALLING);  //设置为下降沿捕获
                capture_Cnt[2]++;
                break;
            case 2:
                capture_Buf[2][1] = HAL_TIM_ReadCapturedValue(&htim2,TIM_CHANNEL_3);//获取当前的捕获值.
                HAL_TIM_IC_Stop_IT(&htim2,TIM_CHANNEL_3); //停止捕获   或者: __HAL_TIM_DISABLE(&htim5);
                capture_Cnt[2]++;
            }
        }
        break;
    case 4:
        if(TIM2 == htim->Instance)
        {
            switch(capture_Cnt[3])
            {
            case 1:
                capture_Buf[3][0] = HAL_TIM_ReadCapturedValue(&htim2,TIM_CHANNEL_4);//获取当前的捕获值.
                __HAL_TIM_SET_CAPTUREPOLARITY(&htim2,TIM_CHANNEL_4,TIM_ICPOLARITY_FALLING);  //设置为下降沿捕获
                capture_Cnt[3]++;
                break;
            case 2:
                capture_Buf[3][1] = HAL_TIM_ReadCapturedValue(&htim2,TIM_CHANNEL_4);//获取当前的捕获值.
                HAL_TIM_IC_Stop_IT(&htim2,TIM_CHANNEL_4); //停止捕获   或者: __HAL_TIM_DISABLE(&htim5);
                capture_Cnt[3]++;
            }
        }
        break;
    case 5:
        if(TIM3 == htim->Instance)
        {
            switch(capture_Cnt[4])
            {
            case 1:
                capture_Buf[4][0] = HAL_TIM_ReadCapturedValue(&htim3,TIM_CHANNEL_1);//获取当前的捕获值.
                __HAL_TIM_SET_CAPTUREPOLARITY(&htim3,TIM_CHANNEL_1,TIM_ICPOLARITY_FALLING);  //设置为下降沿捕获
                capture_Cnt[4]++;
                break;
            case 2:
                capture_Buf[4][1] = HAL_TIM_ReadCapturedValue(&htim3,TIM_CHANNEL_1);//获取当前的捕获值.
                HAL_TIM_IC_Stop_IT(&htim3,TIM_CHANNEL_1); //停止捕获   或者: __HAL_TIM_DISABLE(&htim5);
                capture_Cnt[4]++;
            }
        }
        break;
    case 6:
        if(TIM3 == htim->Instance)
        {
            switch(capture_Cnt[5])
            {
            case 1:
                capture_Buf[5][0] = HAL_TIM_ReadCapturedValue(&htim3,TIM_CHANNEL_2);//获取当前的捕获值.
                __HAL_TIM_SET_CAPTUREPOLARITY(&htim3,TIM_CHANNEL_2,TIM_ICPOLARITY_FALLING);  //设置为下降沿捕获
                capture_Cnt[5]++;
                break;
            case 2:
                capture_Buf[5][1] = HAL_TIM_ReadCapturedValue(&htim3,TIM_CHANNEL_2);//获取当前的捕获值.
                HAL_TIM_IC_Stop_IT(&htim3,TIM_CHANNEL_2); //停止捕获   或者: __HAL_TIM_DISABLE(&htim5);
                capture_Cnt[5]++;
            }
        }
        break;
    case 7:
        if(TIM3 == htim->Instance)
        {
            switch(capture_Cnt[6])
            {
            case 1:
                capture_Buf[6][0] = HAL_TIM_ReadCapturedValue(&htim3,TIM_CHANNEL_3);//获取当前的捕获值.
                __HAL_TIM_SET_CAPTUREPOLARITY(&htim3,TIM_CHANNEL_3,TIM_ICPOLARITY_FALLING);  //设置为下降沿捕获
                capture_Cnt[6]++;
                break;
            case 2:
                capture_Buf[6][1] = HAL_TIM_ReadCapturedValue(&htim3,TIM_CHANNEL_3);//获取当前的捕获值.
                HAL_TIM_IC_Stop_IT(&htim3,TIM_CHANNEL_3); //停止捕获   或者: __HAL_TIM_DISABLE(&htim5);
                capture_Cnt[6]++;
            }
        }
        break;
    case 8:
        if(TIM3 == htim->Instance)
        {
            switch(capture_Cnt[7])
            {
            case 1:
                capture_Buf[7][0] = HAL_TIM_ReadCapturedValue(&htim3,TIM_CHANNEL_4);//获取当前的捕获值.
                __HAL_TIM_SET_CAPTUREPOLARITY(&htim3,TIM_CHANNEL_4,TIM_ICPOLARITY_FALLING);  //设置为下降沿捕获
                capture_Cnt[7]++;
                break;
            case 2:
                capture_Buf[7][1] = HAL_TIM_ReadCapturedValue(&htim3,TIM_CHANNEL_4);//获取当前的捕获值.
                HAL_TIM_IC_Stop_IT(&htim3,TIM_CHANNEL_4); //停止捕获   或者: __HAL_TIM_DISABLE(&htim5);
                capture_Cnt[7]++;
            }
        }
        break;
    default:
        break;
    }
}
/* USER CODE END 1 */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
