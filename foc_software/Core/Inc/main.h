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
#include "stm32g4xx_hal.h"

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

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define TIM_1_8_PERIOD_CLOCKS 3500
#define TIM_APB1_CLOCK_HZ 168000000
#define TIM_APB1_DEADTIME_CLOCKS 40
#define TIM_APB1_PERIOD_CLOCKS 4096
#define TIM_1_8_RCR 0
#define TIM_1_8_DEADTIME_CLOCKS 20
#define TIM_1_8_CLOCK_HZ 168000000
#define LED1_Pin GPIO_PIN_13
#define LED1_GPIO_Port GPIOC
#define LED2_Pin GPIO_PIN_14
#define LED2_GPIO_Port GPIOC
#define LED3_Pin GPIO_PIN_15
#define LED3_GPIO_Port GPIOC
#define DEBUG_SIG_Pin GPIO_PIN_3
#define DEBUG_SIG_GPIO_Port GPIOC
#define ENCODER_SPI_CS_Pin GPIO_PIN_2
#define ENCODER_SPI_CS_GPIO_Port GPIOD
#define ENCODER_SPI_SCK_Pin GPIO_PIN_3
#define ENCODER_SPI_SCK_GPIO_Port GPIOB
#define ENCODER_SPI_MISO_Pin GPIO_PIN_4
#define ENCODER_SPI_MISO_GPIO_Port GPIOB
#define ENCODER_SPI_MOSI_Pin GPIO_PIN_5
#define ENCODER_SPI_MOSI_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
