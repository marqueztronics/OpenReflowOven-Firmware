/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
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
#include <stdio.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
// Define to use SWO trace
//#define DEBUG_PRINT
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc1;
ADC_HandleTypeDef hadc2;

SPI_HandleTypeDef hspi2;
SPI_HandleTypeDef hspi3;

TIM_HandleTypeDef htim1;

/* USER CODE BEGIN PV */
uint32_t ambient_temp, oven_temp;	// Variables to store sensed temperatures

enum states
{
	IDLE,
	PREHEAT,
	REFLOW,
	COOLDOWN
};

enum states state = IDLE;

// PID loop constants
float k_p = 1.2;		// Proportional constant
float k_i = 0.002;		// Integral constant
float k_d = 0;			// Derivative constant
uint8_t interval = 1;	// Interval in seconds
int32_t integral;		// Integral term
int32_t derivative;		// Derivative term
int16_t error_prev;		// Required to compute derivative

// System variables (All temperatures in ºC, times in seconds)
uint8_t time_counter_enable;	// Start count: 1, stop count: 0
uint16_t seconds_count;			// seconds counter, needed to control different reflow profile's phases
uint8_t zero_crossing_count;	// Zero crossings counter, needed to control oven's output
uint8_t oven_duty;				// Oven's "duty cycle": How many cycles will oven's element be enabled. Value between 0 and 100
uint8_t soak_setpoint = 180;	// Set point temperature for soak phase
uint8_t reflow_setpoint = 250;	// Set point temperature for reflow phase
uint16_t soak_duration = 200;	// Soak phase duration
uint16_t reflow_duration = 105;	// Reflow phase duration
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_ADC1_Init(void);
static void MX_ADC2_Init(void);
static void MX_SPI2_Init(void);
static void MX_SPI3_Init(void);
static void MX_TIM1_Init(void);
/* USER CODE BEGIN PFP */
uint8_t PID_Loop(uint16_t setpoint);
void RGB_LED_Color(uint8_t color);
void control_oven(int8_t oven_duty_cycle);
void oven_task(uint16_t setpoint);
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
  MX_ADC1_Init();
  MX_ADC2_Init();
  MX_SPI2_Init();
  MX_SPI3_Init();
  MX_TIM1_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  // State machine
	  switch(state)
	  {
	  case IDLE:
		  HAL_GPIO_WritePin(GPIOA, SSR_Pin, GPIO_PIN_RESET);	// Turn off SSR
		  seconds_count = 0;
		  integral = 0;
		  oven_task(0);
		  RGB_LED_Color(RGB_LED_RED);	// Red color indicates oven is off
		  break;
	  case PREHEAT:
		  oven_task(soak_setpoint);
		  if (seconds_count > soak_duration)
			  state++;
		  RGB_LED_Color(RGB_LED_YELLOW);	// Yellow color indicates oven is in Preheat
		  break;
	  case REFLOW:
		  oven_task(reflow_setpoint);
		  if (seconds_count > soak_duration + reflow_duration)
			  state++;
		  RGB_LED_Color(RGB_LED_GREEN);	// Green color indicates oven is in Reflow
		  break;
	  case COOLDOWN:
		  // Beep sound for 1 sec
		  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
		  HAL_Delay(1000);
		  HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);
		  state = IDLE;
		  time_counter_enable = 0;
		  break;
	  }

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
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

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */

  /** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV2;
  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
  hadc1.Init.ScanConvMode = DISABLE;
  hadc1.Init.ContinuousConvMode = DISABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 1;
  hadc1.Init.DMAContinuousRequests = DISABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_0;
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */

  /* USER CODE END ADC1_Init 2 */

}

/**
  * @brief ADC2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC2_Init(void)
{

  /* USER CODE BEGIN ADC2_Init 0 */

  /* USER CODE END ADC2_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC2_Init 1 */

  /* USER CODE END ADC2_Init 1 */

  /** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
  */
  hadc2.Instance = ADC2;
  hadc2.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV2;
  hadc2.Init.Resolution = ADC_RESOLUTION_12B;
  hadc2.Init.ScanConvMode = DISABLE;
  hadc2.Init.ContinuousConvMode = DISABLE;
  hadc2.Init.DiscontinuousConvMode = DISABLE;
  hadc2.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc2.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc2.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc2.Init.NbrOfConversion = 1;
  hadc2.Init.DMAContinuousRequests = DISABLE;
  hadc2.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  if (HAL_ADC_Init(&hadc2) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_1;
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
  if (HAL_ADC_ConfigChannel(&hadc2, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC2_Init 2 */

  /* USER CODE END ADC2_Init 2 */

}

/**
  * @brief SPI2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI2_Init(void)
{

  /* USER CODE BEGIN SPI2_Init 0 */

  /* USER CODE END SPI2_Init 0 */

  /* USER CODE BEGIN SPI2_Init 1 */

  /* USER CODE END SPI2_Init 1 */
  /* SPI2 parameter configuration*/
  hspi2.Instance = SPI2;
  hspi2.Init.Mode = SPI_MODE_MASTER;
  hspi2.Init.Direction = SPI_DIRECTION_2LINES;
  hspi2.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi2.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi2.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi2.Init.NSS = SPI_NSS_SOFT;
  hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
  hspi2.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi2.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi2.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi2.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI2_Init 2 */

  /* USER CODE END SPI2_Init 2 */

}

/**
  * @brief SPI3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI3_Init(void)
{

  /* USER CODE BEGIN SPI3_Init 0 */

  /* USER CODE END SPI3_Init 0 */

  /* USER CODE BEGIN SPI3_Init 1 */

  /* USER CODE END SPI3_Init 1 */
  /* SPI3 parameter configuration*/
  hspi3.Instance = SPI3;
  hspi3.Init.Mode = SPI_MODE_MASTER;
  hspi3.Init.Direction = SPI_DIRECTION_2LINES;
  hspi3.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi3.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi3.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi3.Init.NSS = SPI_NSS_SOFT;
  hspi3.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
  hspi3.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi3.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi3.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi3.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI3_Init 2 */

  /* USER CODE END SPI3_Init 2 */

}

/**
  * @brief TIM1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM1_Init(void)
{

  /* USER CODE BEGIN TIM1_Init 0 */

  /* USER CODE END TIM1_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};
  TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig = {0};

  /* USER CODE BEGIN TIM1_Init 1 */

  /* USER CODE END TIM1_Init 1 */
  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 16-1;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 210;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 105;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
  sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
  if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  sBreakDeadTimeConfig.OffStateRunMode = TIM_OSSR_DISABLE;
  sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_DISABLE;
  sBreakDeadTimeConfig.LockLevel = TIM_LOCKLEVEL_OFF;
  sBreakDeadTimeConfig.DeadTime = 0;
  sBreakDeadTimeConfig.BreakState = TIM_BREAK_DISABLE;
  sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_HIGH;
  sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;
  if (HAL_TIMEx_ConfigBreakDeadTime(&htim1, &sBreakDeadTimeConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM1_Init 2 */

  /* USER CODE END TIM1_Init 2 */
  HAL_TIM_MspPostInit(&htim1);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, DISP_CS_Pin|DISP_T_CS_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, DISP_RESET_Pin|DISP_DC_Pin|SD_CS_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, REG_LED_B_Pin|REG_LED_R_Pin|REG_LED_G_Pin|SSR_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : BTN1_Pin BTN2_Pin BTN3_Pin BTN4_Pin
                           ZERO_CROSS_Pin */
  GPIO_InitStruct.Pin = BTN1_Pin|BTN2_Pin|BTN3_Pin|BTN4_Pin
                          |ZERO_CROSS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : DISP_CS_Pin DISP_T_CS_Pin */
  GPIO_InitStruct.Pin = DISP_CS_Pin|DISP_T_CS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : DISP_RESET_Pin DISP_DC_Pin SD_CS_Pin */
  GPIO_InitStruct.Pin = DISP_RESET_Pin|DISP_DC_Pin|SD_CS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : REG_LED_B_Pin REG_LED_R_Pin REG_LED_G_Pin SSR_Pin */
  GPIO_InitStruct.Pin = REG_LED_B_Pin|REG_LED_R_Pin|REG_LED_G_Pin|SSR_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI2_IRQn, 0, 15);
  HAL_NVIC_EnableIRQ(EXTI2_IRQn);

  HAL_NVIC_SetPriority(EXTI3_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI3_IRQn);

  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
#ifdef DEBUG_PRINT
// To get printf() on SWO
int _write(int file, char *ptr, int len)
{
  (void)file;
  int DataIdx;

  for (DataIdx = 0; DataIdx < len; DataIdx++)
  {
	  ITM_SendChar(*ptr++);
  }
  return len;
}
#endif

/* RGB_LED_Color
 * @brief: Displays a color on the on-board RGB-LED
 *
 * @param: color value, can be any value in @RGB_LED_COLORS
 *
 * @return: None
 */
void RGB_LED_Color(uint8_t color)
{
	switch (color)
	{
	case RGB_LED_OFF:
		HAL_GPIO_WritePin(GPIOA, REG_LED_R_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOA, REG_LED_G_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOA, REG_LED_B_Pin, GPIO_PIN_RESET);
		break;
	case RGB_LED_RED:
		HAL_GPIO_WritePin(GPIOA, REG_LED_R_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOA, REG_LED_G_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOA, REG_LED_B_Pin, GPIO_PIN_RESET);
		break;
	case RGB_LED_GREEN:
		HAL_GPIO_WritePin(GPIOA, REG_LED_R_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOA, REG_LED_G_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOA, REG_LED_B_Pin, GPIO_PIN_RESET);
		break;
	case RGB_LED_BLUE:
		HAL_GPIO_WritePin(GPIOA, REG_LED_R_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOA, REG_LED_G_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOA, REG_LED_B_Pin, GPIO_PIN_SET);
		break;
	case RGB_LED_CYAN:
		HAL_GPIO_WritePin(GPIOA, REG_LED_R_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOA, REG_LED_G_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOA, REG_LED_B_Pin, GPIO_PIN_SET);
		break;
	case RGB_LED_PURPLE:
		HAL_GPIO_WritePin(GPIOA, REG_LED_R_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOA, REG_LED_G_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOA, REG_LED_B_Pin, GPIO_PIN_SET);
		break;
	case RGB_LED_YELLOW:
		HAL_GPIO_WritePin(GPIOA, REG_LED_R_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOA, REG_LED_G_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOA, REG_LED_B_Pin, GPIO_PIN_RESET);
		break;
	case RGB_LED_WHITE:
		HAL_GPIO_WritePin(GPIOA, REG_LED_R_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOA, REG_LED_G_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOA, REG_LED_B_Pin, GPIO_PIN_SET);
		break;
	}
}

uint8_t PID_Loop(uint16_t setpoint)
{
	int16_t error, output;

	// Calculate the PID terms
	error = setpoint - oven_temp;
	integral = integral + (error * interval);
	derivative = (error - error_prev) / interval;

	output = (k_p * error) + (k_i * integral) + (k_d * derivative);

	//printf("temp: %lu	error: %d	integral: %ld, deritavtive: %ld ", oven_temp, error, integral, derivative);

	if (output > 100)
		output = 100;
	else if (output < 0)
		output = 0;
	//printf("output: %d\n", output);

	// Save value for next iteration
	error_prev = error;

	return (uint8_t)output;
}

void control_oven(int8_t oven_duty_cycle)
{
	if (oven_duty_cycle > 0)
	{
		HAL_GPIO_WritePin(GPIOA, SSR_Pin, GPIO_PIN_SET);	// Turn on SSR
	}
	else
	{
		HAL_GPIO_WritePin(GPIOA, SSR_Pin, GPIO_PIN_RESET);	// Turn off SSR
	}
	oven_duty = oven_duty_cycle;
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
	if (hadc->Instance == ADC1)
	{
		// Thermocouple Measurement complete
		oven_temp = (0.0668*HAL_ADC_GetValue(hadc) -3) + ambient_temp;
	}
	else if (hadc->Instance == ADC2)
	{
		// Thermistor Measurement complete
		ambient_temp = (uint32_t)(((42.57 / 4095) * HAL_ADC_GetValue(hadc)) + 8.708);
#ifdef DEBUG_PRINT
		//printf("Ambient Temperature %lu C\n", ambient_temp);
#endif
	}
}

void oven_task(uint16_t setpoint)
{
	  if (zero_crossing_count == oven_duty)
	  {
		  HAL_GPIO_WritePin(GPIOA, SSR_Pin, GPIO_PIN_RESET);	// Turn off SSR
	  }

	  if (zero_crossing_count >= 99)
	  {
		  zero_crossing_count = 0;	// Reset counter
		  if(time_counter_enable)
		  {
			  seconds_count++;		// Increase seconds counter
		  }
		  HAL_ADC_Start_IT(&hadc2); // Start a conversion on ADC1 (Thermistor)
		  HAL_ADC_Start_IT(&hadc1); // Start a conversion on ADC2 (Thermocouple)

#ifdef DEBUG_PRINT
		  printf("%lu\n", oven_temp);
#endif

		  if (setpoint > 0)
		  {
			  control_oven(PID_Loop(setpoint)); // Regulate Oven's output
		  }

		  else
		  {
			  control_oven(0);
		  }
	  }
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	// Zero cross detection interrupt
	if (GPIO_Pin == ZERO_CROSS_Pin)
	{
		zero_crossing_count++;
	}

	// Start Button Detection Interrupt
	else if (GPIO_Pin == BTN1_Pin)
	{
		state = PREHEAT;	// change state machine to pre-heat so reflow starts
		time_counter_enable = 1;
	}

	// Stop Button Detection Interrupt
	else if (GPIO_Pin == BTN2_Pin)
	{
		state = IDLE;
		time_counter_enable = 0;
	}
}
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
