/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32g0xx_hal.h"

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
#define USER_BUTTON2_Pin GPIO_PIN_11
#define USER_BUTTON2_GPIO_Port GPIOC
#define LED_DEBUG_G_Pin GPIO_PIN_13
#define LED_DEBUG_G_GPIO_Port GPIOC
#define LED_DEBUG_R_Pin GPIO_PIN_14
#define LED_DEBUG_R_GPIO_Port GPIOC
#define LED_DEBUG_B_Pin GPIO_PIN_15
#define LED_DEBUG_B_GPIO_Port GPIOC
#define NFC_A_SCL_Pin GPIO_PIN_0
#define NFC_A_SCL_GPIO_Port GPIOC
#define NFC_A_SDA_Pin GPIO_PIN_1
#define NFC_A_SDA_GPIO_Port GPIOC
#define SD_CARD_MISO_Pin GPIO_PIN_2
#define SD_CARD_MISO_GPIO_Port GPIOC
#define SD_CARD_MOSI_Pin GPIO_PIN_3
#define SD_CARD_MOSI_GPIO_Port GPIOC
#define SD_CARD_SCK_Pin GPIO_PIN_0
#define SD_CARD_SCK_GPIO_Port GPIOA
#define SD_CARD_CS_Pin GPIO_PIN_1
#define SD_CARD_CS_GPIO_Port GPIOA
#define NFC_I_SENSE_N_Pin GPIO_PIN_2
#define NFC_I_SENSE_N_GPIO_Port GPIOA
#define NFC_I_SENSE_P_Pin GPIO_PIN_3
#define NFC_I_SENSE_P_GPIO_Port GPIOA
#define LCD_SCK_Pin GPIO_PIN_5
#define LCD_SCK_GPIO_Port GPIOA
#define LCD_MISO_Pin GPIO_PIN_6
#define LCD_MISO_GPIO_Port GPIOA
#define LCD_MOSI_Pin GPIO_PIN_7
#define LCD_MOSI_GPIO_Port GPIOA
#define LCD_CS_Pin GPIO_PIN_4
#define LCD_CS_GPIO_Port GPIOC
#define LCD_DC_Pin GPIO_PIN_5
#define LCD_DC_GPIO_Port GPIOC
#define BUZZER_OUT_Pin GPIO_PIN_0
#define BUZZER_OUT_GPIO_Port GPIOB
#define LCD_BRIGHTNESS_Pin GPIO_PIN_1
#define LCD_BRIGHTNESS_GPIO_Port GPIOB
#define USB_RX_Pin GPIO_PIN_10
#define USB_RX_GPIO_Port GPIOB
#define USB_TX_Pin GPIO_PIN_11
#define USB_TX_GPIO_Port GPIOB
#define NFC_B_SCL_Pin GPIO_PIN_13
#define NFC_B_SCL_GPIO_Port GPIOB
#define NFC_B_SDA_Pin GPIO_PIN_14
#define NFC_B_SDA_GPIO_Port GPIOB
#define USER_BUTTON1_Pin GPIO_PIN_15
#define USER_BUTTON1_GPIO_Port GPIOB
#define USER_BUTTON7_Pin GPIO_PIN_8
#define USER_BUTTON7_GPIO_Port GPIOA
#define USER_BUTTON6_Pin GPIO_PIN_9
#define USER_BUTTON6_GPIO_Port GPIOA
#define USER_BUTTON5_Pin GPIO_PIN_6
#define USER_BUTTON5_GPIO_Port GPIOC
#define USER_BUTTON4_Pin GPIO_PIN_7
#define USER_BUTTON4_GPIO_Port GPIOC
#define LED_PWR_Pin GPIO_PIN_10
#define LED_PWR_GPIO_Port GPIOA
#define LED_DISPLAY_R_Pin GPIO_PIN_11
#define LED_DISPLAY_R_GPIO_Port GPIOA
#define LED_DISPLAY_B_Pin GPIO_PIN_12
#define LED_DISPLAY_B_GPIO_Port GPIOA
#define NFC_A_BUSY_Pin GPIO_PIN_2
#define NFC_A_BUSY_GPIO_Port GPIOD
#define NFC_A_BUSY_EXTI_IRQn EXTI2_3_IRQn
#define EINK_SPI_CS_Pin GPIO_PIN_3
#define EINK_SPI_CS_GPIO_Port GPIOD
#define EINK_DC_Pin GPIO_PIN_4
#define EINK_DC_GPIO_Port GPIOD
#define EINK_RESET_Pin GPIO_PIN_5
#define EINK_RESET_GPIO_Port GPIOD
#define EINK_BUSY_Pin GPIO_PIN_6
#define EINK_BUSY_GPIO_Port GPIOD
#define EINK_SCK_Pin GPIO_PIN_3
#define EINK_SCK_GPIO_Port GPIOB
#define EINK_MISO_Pin GPIO_PIN_4
#define EINK_MISO_GPIO_Port GPIOB
#define EINK_MOSI_Pin GPIO_PIN_5
#define EINK_MOSI_GPIO_Port GPIOB
#define SD_CARD_CONNDET_Pin GPIO_PIN_8
#define SD_CARD_CONNDET_GPIO_Port GPIOB
#define USER_BUTTON3_Pin GPIO_PIN_10
#define USER_BUTTON3_GPIO_Port GPIOC

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
