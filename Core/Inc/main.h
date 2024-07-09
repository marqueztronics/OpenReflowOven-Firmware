/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define OVEN_TEMP_Pin GPIO_PIN_0
#define OVEN_TEMP_GPIO_Port GPIOA
#define AMBIENT_TEMP_Pin GPIO_PIN_1
#define AMBIENT_TEMP_GPIO_Port GPIOA
#define BTN1_Pin GPIO_PIN_2
#define BTN1_GPIO_Port GPIOA
#define BTN1_EXTI_IRQn EXTI2_IRQn
#define BTN2_Pin GPIO_PIN_3
#define BTN2_GPIO_Port GPIOA
#define BTN2_EXTI_IRQn EXTI3_IRQn
#define BTN3_Pin GPIO_PIN_6
#define BTN3_GPIO_Port GPIOA
#define BTN4_Pin GPIO_PIN_7
#define BTN4_GPIO_Port GPIOA
#define DISP_CS_Pin GPIO_PIN_5
#define DISP_CS_GPIO_Port GPIOC
#define DISP_RESET_Pin GPIO_PIN_0
#define DISP_RESET_GPIO_Port GPIOB
#define DISP_DC_Pin GPIO_PIN_1
#define DISP_DC_GPIO_Port GPIOB
#define DISP_SCK_Pin GPIO_PIN_13
#define DISP_SCK_GPIO_Port GPIOB
#define DISP_MISO_Pin GPIO_PIN_14
#define DISP_MISO_GPIO_Port GPIOB
#define DISP_MOSI_Pin GPIO_PIN_15
#define DISP_MOSI_GPIO_Port GPIOB
#define DISP_T_CS_Pin GPIO_PIN_7
#define DISP_T_CS_GPIO_Port GPIOC
#define BUZZER_Pin GPIO_PIN_8
#define BUZZER_GPIO_Port GPIOA
#define REG_LED_B_Pin GPIO_PIN_9
#define REG_LED_B_GPIO_Port GPIOA
#define REG_LED_R_Pin GPIO_PIN_10
#define REG_LED_R_GPIO_Port GPIOA
#define REG_LED_G_Pin GPIO_PIN_11
#define REG_LED_G_GPIO_Port GPIOA
#define ZERO_CROSS_Pin GPIO_PIN_12
#define ZERO_CROSS_GPIO_Port GPIOA
#define ZERO_CROSS_EXTI_IRQn EXTI15_10_IRQn
#define TMS_Pin GPIO_PIN_13
#define TMS_GPIO_Port GPIOA
#define TCK_Pin GPIO_PIN_14
#define TCK_GPIO_Port GPIOA
#define SSR_Pin GPIO_PIN_15
#define SSR_GPIO_Port GPIOA
#define SD_SCK_Pin GPIO_PIN_10
#define SD_SCK_GPIO_Port GPIOC
#define SD_MOSI_Pin GPIO_PIN_12
#define SD_MOSI_GPIO_Port GPIOC
#define SWO_Pin GPIO_PIN_3
#define SWO_GPIO_Port GPIOB
#define SD_MISO_Pin GPIO_PIN_4
#define SD_MISO_GPIO_Port GPIOB
#define SD_CS_Pin GPIO_PIN_6
#define SD_CS_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */
// @@RGB_LED_COLORS
#define RGB_LED_OFF 0
#define RGB_LED_RED 1
#define RGB_LED_GREEN 2
#define RGB_LED_BLUE 3
#define RGB_LED_PURPLE 4
#define RGB_LED_YELLOW 5
#define RGB_LED_CYAN 6
#define RGB_LED_WHITE 7

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
