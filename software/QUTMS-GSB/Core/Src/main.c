/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "can.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
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

/* USER CODE BEGIN PV */


/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
#define Suspension_ADC ADC_CHANNEL_1
#define GearBox_ADC ADC_CHANNEL_2
#define Radiator_ADC ADC_CHANNEL_3
#define TEM30C ((uint16_t*) ((uint32_t) 0x1FFFF7B8))
#define TEMP110C ((uint16_t*) ((uint32_t) 0x1FFFF7C2))


void config_ext_channel_ADC(uint16_t channel, bool val)
{
  ADC_ChannelConfTypeDef sConfig;
  sConfig.Channel = channel;
  sConfig.SamplingTime = ADC_SAMPLETIME_71CYCLES_5;

  if(true == val)
  {
    sConfig.Rank = ADC_RANK_CHANNEL_NUMBER;
  }
  else
  {
    sConfig.Rank = ADC_RANK_NONE;
  }

  HAL_ADC_ConfigChannel(&hadc, &sConfig);
}

uint16_t r_single_ext_channel_ADC(uint16_t channel)
{
  uint16_t digital_result;
  config_ext_channel_ADC(channel, true);
  HAL_ADCEx_Calibration_Start(&hadc);

  HAL_ADC_Start(&hadc);
  HAL_ADC_PollForConversion(&hadc, 1000);
  digital_result = HAL_ADC_GetValue(&hadc);
  HAL_ADC_Stop(&hadc);

  config_ext_channel_ADC(channel, false);

  return digital_result;
}

uint16_t GET_Suspension(){
	uint16_t suspension_raw = r_single_ext_channel_ADC(Suspension_ADC);
	return suspension_raw;
}


uint16_t GET_Gearbox(){
	uint16_t Gearbox_raw = r_single_ext_channel_ADC(GearBox_ADC);
	return Gearbox_raw;
}

float temprature(uint32_t ts_data){
	float temp;
	float slope = ((110.0 - 30.0)/((*TEMP110C) - (*TEM30C)));
	//temp = ((3.3/3.3) * ts_data) / 1000;
	temp = (slope * (ts_data - (*TEM30C))) + 30;
	return temp;
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
  MX_USART1_UART_Init();
  MX_ADC_Init();
  MX_CAN_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

  char msg[100];
  uint16_t raw = 0;
  uint16_t max_suspension = 4055;//taken data form the suspension
  uint16_t min_suspension = 2330;

  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  raw = r_single_ext_channel_ADC(Suspension_ADC);
	  uint16_t raw_percent =(float)(raw-min_suspension)/(max_suspension-min_suspension) * 100;
	  sprintf(msg, "suspension: %d          percent: %d\r\n", raw, raw_percent);
	  		  HAL_UART_Transmit(&huart1, (uint8_t*) msg, strlen((char*) msg),
	  		  				HAL_MAX_DELAY);
	  HAL_Delay(500);

	  raw = r_single_ext_channel_ADC(GearBox_ADC);
	  uint16_t temperature_c = temprature(raw);
	  sprintf(msg, "gear box: %d          C: %d\r\n", raw, temperature_c);
		  		  HAL_UART_Transmit(&huart1,  (uint8_t*) msg, strlen((char*) msg),
		  		  				HAL_MAX_DELAY);
	 HAL_Delay(500);

	 raw = r_single_ext_channel_ADC(Radiator_ADC);
	 	  temperature_c = temprature(raw);
	 	  sprintf(msg, "gear box: %d          C: %d\r\n", raw, temperature_c);
	 		  		  HAL_UART_Transmit(&huart1,  (uint8_t*) msg, strlen((char*) msg),
	 		  		  				HAL_MAX_DELAY);
	 	 HAL_Delay(500);
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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_HSI14;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSI14State = RCC_HSI14_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.HSI14CalibrationValue = 16;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1;
  PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK1;
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
