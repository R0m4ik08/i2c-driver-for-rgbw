/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
#ifndef PC_DEBUG

UART_HandleTypeDef huart2;

#endif // PC_DEBUG

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/

#ifndef PC_DEBUG

void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);

#endif // PC_DEBUG

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
    /* USER CODE BEGIN 1 */

    /* USER CODE END 1 */

    #ifndef PC_DEBUG

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
    MX_USART2_UART_Init();

    #endif // PC_DEBUG
    /* USER CODE BEGIN 2 */

    i2chw_dev_t i2chw_dev = {
        .addr_width = I2CHW_ADDR_WIDTH_8BIT,
        .bus_num    = I2CHW_BUS_I2C0,
        .dev_addr   = 0x55
    }; 
    i2chw_cfg_t i2chw_cfg = {
        .bus_freq  = I2CHW_1000_KHZ,
        .dir_mode  = I2CHW_SLAVE_MODE
    };

    printf("RGBW driver initialized start:\n");
    if (rgbw_driver_init(&i2chw_dev, &i2chw_cfg) != I2CHW_SUCCESS) {
        printf("Failed to initialize RGBW driver.\n");
        return -1;
    }
    printf("RGBW driver initialized successfully.\n\n");

    // Установка яркости каналов
    printf("Start: Red channel brightness set to 128.\n");
    if (rgbw_driver_set_channel_brightness(&i2chw_dev, RED, 128) == I2CHW_SUCCESS) {
        printf("End: Red channel brightness set to 128.\n\n");
    } else {
        printf("Failed to set red channel brightness.\n\n");
    }

    printf("Start: Green channel brightness set to 64.\n");
    if (rgbw_driver_set_channel_brightness(&i2chw_dev, GREEN, 64) == I2CHW_SUCCESS) {
        printf("End: Green channel brightness set to 64.\n\n");
    } else {
        printf("Failed to set green channel brightness.\n\n");
    }

    printf("Start: Blue channel brightness set to 255.\n");
    if (rgbw_driver_set_channel_brightness(&i2chw_dev, BLUE, 255) == I2CHW_SUCCESS) {
        printf("End: Blue channel brightness set to 255.\n\n");
    } else {
        printf("Failed to set blue channel brightness.\n\n");
    }

    printf("Start: White channel brightness set to 0x20.\n");
    if (rgbw_driver_set_channel_brightness(&i2chw_dev, WHITE, 0x20) == I2CHW_SUCCESS) {
        printf("End: White channel brightness set to 0x20.\n\n");
    } else {
        printf("Failed to set white channel brightness.\n\n");
    }

    // Установка активности каналов
    printf("Start: Red channel activated.\n");
    if (rgbw_driver_set_channel_mode(&i2chw_dev, RED, ALWAYS_ON) == I2CHW_SUCCESS) {
        printf("End: Red channel activated.\n\n");
    } else {
        printf("Failed to activate red channel.\n\n");
    }

    printf("Start: Red channel deactivated.\n");
    if (rgbw_driver_set_channel_mode(&i2chw_dev, RED, ALWAYS_OFF) == I2CHW_SUCCESS) {
        printf("End: Red channel deactivated.\n\n");
    } else {
        printf("Failed to deactivate red channel.\n\n");
    }

    printf("Start: Green channel activated.\n");
    if (rgbw_driver_set_channel_mode(&i2chw_dev, GREEN, ALWAYS_ON) == I2CHW_SUCCESS) {
        printf("End: Green channel activated.\n\n");
    } else {
        printf("Failed to activated green channel.\n\n");
    }

    printf("Start: Blue channel activated.\n");
    if (rgbw_driver_set_channel_mode(&i2chw_dev, BLUE, ALWAYS_ON) == I2CHW_SUCCESS) {
        printf("End: Blue channel activated.\n\n");
    } else {
        printf("Failed to activated blue channel.\n\n");
    }

    printf("Start: White channel activated.\n");
    if (rgbw_driver_set_channel_mode(&i2chw_dev, WHITE, ALWAYS_ON) == I2CHW_SUCCESS) {
        printf("End: White channel activated.\n\n");
    } else {
        printf("Failed to activated white channel.\n\n");
    }

    // Выключение всех каналов
    printf("Start: All channels deactivated.\n");
    if (rgbw_driver_all_channels_activity(&i2chw_dev, ALWAYS_OFF) == I2CHW_SUCCESS) {
        printf("End: All channels deactivated.\n\n");
    } else {
        printf("Failed to deactivate all channels.\n\n");
    }

    printf("Start: RGB color set successfully.\n");
    if(rgbw_driver_set_rgb_color(&i2chw_dev, 0xff, 0x80, 0x40) == I2CHW_SUCCESS){
        printf("End: RGB color set successfully.\n\n");
    }else{
        printf("Failed to set RGB color.\n\n");
    }

    /* USER CODE END 2 */

    /* Infinite loop */
    /* USER CODE BEGIN WHILE */
    while (1)
    {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
    }
    /* USER CODE END 3 */
}


#ifndef PC_DEBUG

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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_I2C1;
  PeriphClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_HSI;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 38400;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
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

#endif // PC_DEBUG

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
