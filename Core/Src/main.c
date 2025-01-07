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
#include "fatfs.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <string.h>
#include "../../Drivers/tft/ili9341.h"
#include "../../Drivers/touchpad/xpt2046.h"
#include "../../Drivers/lvgl/lvgl.h"
#include "reflow_oven_gui.h"
#include "lwjson/lwjson.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
//#define DEBUG_PRINT	/* Define to use SWO trace */
#define DISPLAY_WIDTH 320
#define DISPLAY_HEIGHT 240
#define WIDTH (DISPLAY_WIDTH)
#define HEIGHT (DISPLAY_HEIGHT)
#define BYTE_PER_PIXEL (LV_COLOR_FORMAT_GET_SIZE(LV_COLOR_FORMAT_RGB565)) /*will be 2 for RGB565 */
#define BUFF_SIZE (DISPLAY_WIDTH * DISPLAY_HEIGHT / 10 * BYTE_PER_PIXEL)
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc1;
ADC_HandleTypeDef hadc2;

SPI_HandleTypeDef hspi1;
SPI_HandleTypeDef hspi2;
SPI_HandleTypeDef hspi3;

TIM_HandleTypeDef htim1;

/* USER CODE BEGIN PV */
uint32_t ambient_temp, oven_temp;	// Variables to store sensed temperatures

enum states state = IDLE;

// PID loop constants
float k_p_preheat = 0.85;		// Proportional constant
float k_i_preheat = 0.001;		// Integral constant
float k_d_preheat = 0;			// Derivative constant
float k_p_reflow = 2.25;		// Proportional constant
float k_i_reflow = 0;			// Integral constant
float k_d_reflow = 0;			// Derivative constant
uint8_t interval = 1;	// Interval in seconds
int32_t integral;		// Integral term
int32_t derivative;		// Derivative term
int16_t error_prev;		// Required to compute derivative

// System variables (All temperatures in ºC, times in seconds)
uint8_t time_counter_enable;	// Start count: 1, stop count: 0
uint16_t seconds_count;			// seconds counter, needed to control different reflow profile's phases
uint8_t zero_crossing_count;	// Zero crossings counter, needed to control oven's output
uint8_t oven_duty;				// Oven's "duty cycle": How many cycles will oven's element be enabled. Value between 0 and 100
char profile_names[5][20];			// Profile names array
uint8_t preheat_setpoints[5];		// Set point temperature array for soak phase
uint16_t reflow_setpoints[5];	// Set point temperature array for reflow phase
uint16_t preheat_durations[5];		// Soak phase duration array
uint16_t reflow_durations[5];	// Reflow phase duration array
uint8_t profile_number;			// Reflow profile index
uint8_t total_profiles;			// Holds the total of profiles decoded in JSON

// Buffers for LVGL
static uint8_t buf_1[BUFF_SIZE];
static uint8_t buf_2[BUFF_SIZE];

// Extern LVGL objects
extern lv_obj_t* profile_label;
extern lv_obj_t* phase_label;
extern lv_obj_t* temp_label;
extern lv_obj_t* time_label;

// SD CARD related variable

FATFS fs;
FATFS *pfs;
FIL fil;
FRESULT fres;
DWORD fre_clust;
uint32_t totalSpace, freeSpace;
char buffer[500];

// LwJSON instance and tokens
static lwjson_token_t tokens[20];
static lwjson_t lwjson;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_ADC1_Init(void);
static void MX_ADC2_Init(void);
static void MX_SPI2_Init(void);
static void MX_SPI3_Init(void);
static void MX_TIM1_Init(void);
static void MX_SPI1_Init(void);
/* USER CODE BEGIN PFP */
uint8_t PID_Loop(uint16_t setpoint, float k_p, float k_i, float k_d);
void RGB_LED_Color(uint8_t color);
void control_oven(int8_t oven_duty_cycle);
void oven_task(uint16_t setpoint);
void my_flush_cb(lv_display_t * display, const lv_area_t * area, uint8_t * px_map);
void my_touchpad_read(lv_indev_t * indev_drv, lv_indev_data_t * data);
void json_get_tokens(char *buf);
void print_child_tokens(lwjson_token_t* t, int indent_depth);
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
  MX_SPI1_Init();
  MX_FATFS_Init();
  /* USER CODE BEGIN 2 */
  /* Wait for SD module reset */
    	HAL_Delay(500);

  	if(f_mount(&fs, "", 0) != FR_OK)
  		Error_Handler();

  	/* Open file to read */
  	if(f_open(&fil, "profiles.json", FA_READ) != FR_OK)
  		Error_Handler();

  	f_read(&fil, buffer, sizeof(buffer), NULL);

  	/* Close file */
  	if(f_close(&fil) != FR_OK)
  		Error_Handler();

  	/* Unmount SDCARD */
  	if(f_mount(NULL, "", 1) != FR_OK)
  		Error_Handler();

  	json_get_tokens(buffer);

  // Initialize ILI9341 driver
  ILI9341_Init();
  ILI9341_Set_Rotation(SCREEN_HORIZONTAL_2);

  //Initialise LVGL UI library
  lv_init();

  lv_display_t * disp = lv_display_create(HEIGHT, WIDTH); /*Basic initialization with horizontal and vertical resolution in pixels*/
  lv_display_set_flush_cb(disp, my_flush_cb); /*Set a flush callback to draw to the display*/
  lv_display_set_buffers(disp, buf_1, buf_2, sizeof(buf_1), LV_DISPLAY_RENDER_MODE_PARTIAL); /*Set an initialized buffer*/

  lv_display_set_rotation(disp, LV_DISPLAY_ROTATION_90);

  lv_indev_t * indev = lv_indev_create();           /*Create an input device*/
  lv_indev_set_type(indev, LV_INDEV_TYPE_POINTER);  /*Touch pad is a pointer-like device*/
  lv_indev_set_read_cb(indev, my_touchpad_read);    /*Set your driver function*/

  reflow_oven_ui();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  lv_timer_handler();

	  // State machine
	  switch(state)
	  {
	  case IDLE:
		  lv_label_set_text(phase_label, "Phase: IDLE");
		  HAL_GPIO_WritePin(SSR_GPIO_Port, SSR_Pin, GPIO_PIN_RESET);	// Turn off SSR
		  seconds_count = 0;
		  integral = 0;
		  oven_task(0);
		  break;
	  case PREHEAT:
		  lv_label_set_text(phase_label, "Phase: PREHEAT");
		  time_counter_enable = 1;
		  oven_task(preheat_setpoints[profile_number]);
		  if (seconds_count > preheat_durations[profile_number])
			  state++;
		  break;
	  case REFLOW:
		  lv_label_set_text(phase_label, "Phase: REFLOW");
		  oven_task(reflow_setpoints[profile_number]);
		  if (seconds_count > preheat_durations[profile_number] + reflow_durations[profile_number])
		  {
			  // Beep sound for 1 sec
			  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
			  lv_delay_ms(1000);
			  HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);
			  state++;
		  }
		  break;
	  case COOLDOWN:
		  lv_label_set_text(phase_label, "Phase: COOLDOWN");
		  if (oven_temp <= 50 || seconds_count >= 480)
		  {
			  state = IDLE;
			  time_counter_enable = 0;
		  }
		  oven_task(0);
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
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 180;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Activate the Over-Drive mode
  */
  if (HAL_PWREx_EnableOverDrive() != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
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
  hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV8;
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
  hadc2.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV8;
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
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_16;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

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
  hspi3.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_128;
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
  htim1.Init.Prescaler = 180-1;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 208;
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
  sConfigOC.Pulse = 104;
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
  HAL_GPIO_WritePin(SD_CS_GPIO_Port, SD_CS_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, DISP_CS_Pin|DISP_T_CS_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, DISP_RESET_Pin|DISP_DC_Pin|SSR_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : SD_CS_Pin */
  GPIO_InitStruct.Pin = SD_CS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(SD_CS_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : DISP_CS_Pin DISP_T_CS_Pin */
  GPIO_InitStruct.Pin = DISP_CS_Pin|DISP_T_CS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : DISP_RESET_Pin DISP_DC_Pin SSR_Pin */
  GPIO_InitStruct.Pin = DISP_RESET_Pin|DISP_DC_Pin|SSR_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : DISP_T_IRQ_Pin */
  GPIO_InitStruct.Pin = DISP_T_IRQ_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(DISP_T_IRQ_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : ZERO_CROSS_Pin */
  GPIO_InitStruct.Pin = ZERO_CROSS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(ZERO_CROSS_GPIO_Port, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI9_5_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

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

uint8_t decode_profiles_json(lwjson_token_t* t)
{
	lwjson_token_t* profiles_tkn;	// Token variable to handle each profile in the JSON array
	lwjson_token_t* preheat_tkn;	// Token variable to handle preheat's object elements
	lwjson_token_t* reflow_tkn;		// Token variable to handle reflow's object elements
	int profile_index = 0;			// JSON file holds an array of profiles, this index keeps track of each profile
	for (const lwjson_token_t* tkn = lwjson_get_first_child(t); tkn != NULL; tkn = tkn->next) {

		// Token "profile_name"
		profiles_tkn = lwjson_get_first_child(tkn);
		strncpy(profile_names[profile_index], profiles_tkn->u.str.token_value, profiles_tkn->u.str.token_value_len);

		// Token "preheat_stage"
		profiles_tkn = profiles_tkn->next;

		// This is an object, so get its children
		// Token "preheat_duration"
		preheat_tkn = lwjson_get_first_child(profiles_tkn);
		preheat_durations[profile_index] = preheat_tkn->u.num_int;

		// Token "preheat_setpoint"
		preheat_tkn = preheat_tkn->next;
		preheat_setpoints[profile_index] = preheat_tkn->u.num_int;

		// Token "reflow_stage"
		profiles_tkn = profiles_tkn->next;

		// This is an object, so get its children
		// Token "reflow_duration"
		reflow_tkn = lwjson_get_first_child(profiles_tkn);
		reflow_durations[profile_index] = reflow_tkn->u.num_int;

		// Token "reflow_setpoint"
		reflow_tkn = reflow_tkn->next;
		reflow_setpoints[profile_index] = reflow_tkn->u.num_int;

		profile_index++;	// Increase index so next iteration of the for loop decodes the next profile in the array
	}

	return profile_index;	// Return the number of reflow profiles decoded from JSON
}

void print_child_tokens(lwjson_token_t* t, int indent_depth)
{
    /* Now print all keys in the object */
    for (const lwjson_token_t* tkn = lwjson_get_first_child(t); tkn != NULL; tkn = tkn->next) {
    	for (int i = 0; i < indent_depth; i++)
    		printf("\t");
        printf("Token: %.*s,", (int)tkn->token_name_len, tkn->token_name);
        if (tkn->type == LWJSON_TYPE_NUM_INT){
        	int temp = (int)tkn->u.num_int;	// Collect token value in int format
        	printf(" Value: %d\n", temp);
        }
        else if (tkn->type == LWJSON_TYPE_STRING){
        	const char* str_val;
        	str_val = tkn->u.str.token_value; // Collect token value in string format
        	printf(" Value: %.*s\n", tkn->u.str.token_value_len, str_val);
        }
        else if (tkn->type == LWJSON_TYPE_ARRAY || tkn->type == LWJSON_TYPE_OBJECT) {
            printf(" token is array or object, printing children:\n");
            //print child tokens recursively
            print_child_tokens(tkn, (indent_depth + 1));
        }
    }
}

void json_get_tokens(char *buf) {
    /* Initialize and pass statically allocated tokens */
    lwjson_init(&lwjson, tokens, LWJSON_ARRAYSIZE(tokens));

    /* Try to parse input string */
    if (lwjson_parse(&lwjson, buf) == lwjsonOK) {
        lwjson_token_t* t;
        printf("JSON parsed..\r\n");

        /* Get very first token as top object */
        t = lwjson_get_first_token(&lwjson);
        if (t->type == LWJSON_TYPE_ARRAY) {
            printf("JSON starts with array..\n");
        } else if (t->type == LWJSON_TYPE_OBJECT) {
            printf("JSON starts with object..\n");
        } else {
            printf("This should never happen..\n");
        }

        //print_child_tokens(t, 0);
        total_profiles = decode_profiles_json(lwjson_get_first_child(t));

        /* Call this when not used anymore */
        lwjson_free(&lwjson);
    }
}

uint8_t PID_Loop(uint16_t setpoint, float k_p, float k_i, float k_d)
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
		HAL_GPIO_WritePin(SSR_GPIO_Port, SSR_Pin, GPIO_PIN_SET);	// Turn on SSR
	}
	else
	{
		HAL_GPIO_WritePin(SSR_GPIO_Port, SSR_Pin, GPIO_PIN_RESET);	// Turn off SSR
	}
	oven_duty = oven_duty_cycle;
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
	if (hadc->Instance == ADC1)
	{
		// Thermocouple Measurement complete
		oven_temp = (0.069*HAL_ADC_GetValue(hadc) - 5.168) + ambient_temp;

#ifdef DEBUG_PRINT
		printf("Oven Temp ADC Val: %lu C\n", HAL_ADC_GetValue(hadc));
#endif
	}
	else if (hadc->Instance == ADC2)
	{
		// Thermistor Measurement complete
		ambient_temp = (0.017*HAL_ADC_GetValue(hadc) - 15.057);
	}
}

void oven_task(uint16_t setpoint)
{
	  lv_label_set_text_fmt(profile_label, "Profile: %s", profile_names[profile_number]);
	  if (zero_crossing_count >= oven_duty)
	  {
		  HAL_GPIO_WritePin(SSR_GPIO_Port, SSR_Pin, GPIO_PIN_RESET);	// Turn off SSR
	  }

	  if (zero_crossing_count >= 99)
	  {
		  zero_crossing_count = 0;	// Reset counter
		  if(time_counter_enable)
		  {
			  seconds_count++;		// Increase seconds counter
			  lv_label_set_text_fmt(time_label, "%d s", seconds_count);
		  }
		  HAL_ADC_Start_IT(&hadc2); // Start a conversion on ADC1 (Thermistor)
		  HAL_ADC_Start_IT(&hadc1); // Start a conversion on ADC2 (Thermocouple)

		  if (state != IDLE)
			  update_chart(seconds_count, oven_temp);

		  lv_label_set_text_fmt(temp_label, "%lu C", oven_temp);

#ifdef DEBUG_PRINT
		  printf("%lu\n", oven_temp);
#endif

		  if (setpoint > 0)
		  {
			  if (state == PREHEAT)
				  control_oven(PID_Loop(setpoint, k_p_preheat, k_i_preheat, k_d_preheat)); // Regulate Oven's output

			  else
				  control_oven(PID_Loop(setpoint, k_p_reflow, k_i_reflow, k_d_reflow)); // Regulate Oven's output
		  }

		  else
		  {
			  control_oven(0);
		  }
	  }
}

void my_flush_cb(lv_display_t * display, const lv_area_t * area, uint8_t * px_map)
{
    /*The most simple case (but also the slowest) to put all pixels to the screen one-by-one
     *`put_px` is just an example, it needs to be implemented by you.*/
    uint16_t * buf16 = (uint16_t *)px_map; /*Let's say it's a 16 bit (RGB565) display*/
    uint16_t width = (area->x2 - area->x1) + 1;
    uint16_t height = (area->y2 - area->y1) + 1;
    ILI9341_Draw_Bitmap(area->x1, area->y1, width, height, buf16);

    /* IMPORTANT!!!
     * Inform LVGL that you are ready with the flushing and buf is not used anymore*/
    lv_display_flush_ready(display);
}

void my_touchpad_read(lv_indev_t * indev_drv, lv_indev_data_t * data)
{
    /*`touchpad_is_pressed` and `touchpad_get_xy` needs to be implemented by you*/
    if(XPT2046_TouchPressed()) {
      data->state = LV_INDEV_STATE_PRESSED;
      XPT2046_TouchGetCoordinates((uint16_t*)&data->point.x, (uint16_t*)&data->point.y);
    } else {
      data->state = LV_INDEV_STATE_RELEASED;
    }
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	// Zero cross detection interrupt
	if (GPIO_Pin == ZERO_CROSS_Pin)
	{
		zero_crossing_count++;
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
