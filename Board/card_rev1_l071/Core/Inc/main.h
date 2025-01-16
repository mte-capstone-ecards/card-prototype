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
#include "stm32l0xx_hal.h"

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
#define HARVEST_ADC_Pin GPIO_PIN_0
#define HARVEST_ADC_GPIO_Port GPIOA
#define VDD_ADC_Pin GPIO_PIN_1
#define VDD_ADC_GPIO_Port GPIOA
#define EINK_DC_Pin GPIO_PIN_4
#define EINK_DC_GPIO_Port GPIOA
#define EINK_SPI_SCL_Pin GPIO_PIN_5
#define EINK_SPI_SCL_GPIO_Port GPIOA
#define EINK_SPI_CS_Pin GPIO_PIN_6
#define EINK_SPI_CS_GPIO_Port GPIOA
#define EINK_SPI_MOSI_Pin GPIO_PIN_7
#define EINK_SPI_MOSI_GPIO_Port GPIOA
#define EINK_BUSY_Pin GPIO_PIN_8
#define EINK_BUSY_GPIO_Port GPIOA
#define EINK_RESET_Pin GPIO_PIN_9
#define EINK_RESET_GPIO_Port GPIOA
#define M24LR_SCL_Pin GPIO_PIN_6
#define M24LR_SCL_GPIO_Port GPIOB
#define M24LR_SDA_Pin GPIO_PIN_7
#define M24LR_SDA_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
