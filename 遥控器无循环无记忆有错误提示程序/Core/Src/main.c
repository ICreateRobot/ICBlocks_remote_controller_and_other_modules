/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "dma.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "stm32f1xx_it.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "Data_Analysis.h"
#include "Bluetooth.h"
/* USER CODE END Includes */
/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
uint16_t LED_Time;
char SendCom1[16] = "AT+HOSTEN1\r\n";  //定义数据发送数组
char SendCom2[13] = "AT+NAME25\r\n";  //定义数据发送数组
//Sendbuff[0]= 0;
uint8_t Turn_OFF=0;
/* USER CODE END PTD */
/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */
/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
/* USER CODE END PM */
/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */
/* USER CODE END PV */
/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void TEST_PRO(void)
{
    Interface_module();
    for(uint8_t i=0; i<8; i++)
    {
        if(Interface[i]==99)
        {
            GPIO_SET(i);
            HAL_Delay(88);
            GPIO_RESET(i);
            HAL_Delay(88);
        }
    }
}
/* USER CODE END PFP */
/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if(htim->Instance == TIM4)
    {
        Get_ADC_Averagevalue();
        LED_Time++;
    }
    if(htim->Instance == TIM1)
    {
        LED_Flashing_Time++;
//        Send_Data(Sendbuff1);
        Boot_detection();
        /*************************/
        Light_times++;
    }
}
/* USER CODE END 0 */
/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
    /* USER CODE BEGIN 1 */
    /* USER CODE END 1 */
    /* MCU Configuration--------------------------------------------------------*/
    /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
    HAL_Init();
    /* USER CODE BEGIN Init */
    /* USER CODE END Init */
    /* Configure the system clock */
    SystemClock_Config();
    /* USER CODE BEGIN SysInit */
    /* USER CODE END SysInit */
    /* Initialize all configured peripherals */
    MX_GPIO_Init();
    MX_DMA_Init();
    MX_ADC1_Init();
    MX_TIM4_Init();
    MX_USART1_UART_Init();
    MX_TIM1_Init();
    /* USER CODE BEGIN 2 */
    HAL_ADC_Start_DMA(&hadc1,(uint32_t*)&ADC_ValBuf,320);
    MX_TIM2_Init();
    MX_TIM3_Init();
    HAL_TIM_Base_Start_IT(&htim4);
    HAL_TIM_Base_Start_IT(&htim1);
    /****************************/
    __HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE);  //开启空闲中断
    HAL_UART_Receive_DMA(&huart1,rx_buffer,7);  //开启DMA接收中断
    /************************/
    HAL_Delay(200);
    Interface_module();
    Module_Init();
    /* Infinite loop */
    /* USER CODE BEGIN WHILE */
    while (1)
    {
        if((HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_13)==1))
        {
            HAL_Delay(20);
            if((HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_13)==1))
            {
                HAL_GPIO_WritePin(GPIOC,GPIO_PIN_10,GPIO_PIN_RESET);
                Sendbuff[0]= 10;     //开始标志
                Send_Data(Sendbuff);
                Turn_OFF=1;
            }
        }
        else
        {
            Turn_OFF=0;
        }
        /**********测试遥控器程序*********/
        //  TEST_PRO();
        /***********/
        if(Turn_OFF==0)   //遥控器充电不可以使用
        {
            Programming_Determine();
            /*******************************/
            if(Programming_Error==0)
            {
                if(KEY_Scan()==1)
                {
                    Sendbuff[0]= 33; //关灯
                    Send_Data(Sendbuff);
                    HAL_Delay(20);
                    Send_Data(Sendbuff);
                    HAL_Delay(20);
                    Interface_module();
                    Module_Init();    //只初始化一次函数
                    for(uint8_t i=0; i<8; i++)
                    {
                        GPIO_SET(i);
                    }
                    HAL_Delay(300);
                    Special_Combination_Module();
                }
            }
        }
    }
    /* USER CODE END 3 */
}
/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
    RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};
    /** Initializes the RCC Oscillators according to the specified parameters
    * in the RCC_OscInitTypeDef structure.
    */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        Error_Handler();
    }
    /** Initializes the CPU, AHB and APB buses clocks
    */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                                  |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
    {
        Error_Handler();
    }
    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
    PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV8;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
    {
        Error_Handler();
    }
}
/* USER CODE BEGIN 4 */
/* USER CODE END 4 */
/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
    /* USER CODE BEGIN Error_Handler_Debug */
    /* User can add his own implementation to report the HAL error return state */
    __disable_irq();
    while (1)
    {
    }
    /* USER CODE END Error_Handler_Debug */
}
#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
    /* USER CODE BEGIN 6 */
    /* User can add his own implementation to report the file name and line number,
       ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
    /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
