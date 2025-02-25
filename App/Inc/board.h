#pragma once

/*

File for defining and validating of HW resources as needed.

*/

#include "base_defs.h"
#include "main.h"

#if BOARD_L0
# include "stm32l0xx_hal.h"
#elif BOARD_G4
# include "stm32g431xx_hal.h"
#elif BOARD_F4
# include "stm32f4xx_hal.h"
#elif BOARD_G0
# include "stm32g0xx_hal.h"
#else
# error "No hal file defined"
#endif

#if FTR_NFCTAG
# if BOARD_L0
#  define M24LR_I2C	hi2c1
# elif TYPE_CARD
#  error "Must define M24LR for card"
# endif
#endif

#if FTR_NFCREADER | FTR_SIMEEPROM
# if BOARD_F4
#  define ST25R_SPI  hspi1

#  define ST25R_SS_PIN          ST25_CS_Pin         /* GPIO pin used for ST25R SPI SS                */
#  define ST25R_SS_PORT         ST25_CS_GPIO_Port   /* GPIO port used for ST25R SPI SS port          */

#  define ST25R_INT_PIN         ST25_INTR_Pin            /* GPIO pin used for ST25R External Interrupt    */
#  define ST25R_INT_PORT        ST25_INTR_GPIO_Port      /* GPIO port used for ST25R External Interrupt   */

#  define ST25R_INT_NUM         ST25_INTR_EXTI_IRQn
# elif BOARD_G0
#  define ST25R_I2C hi2c3

#  define ST25R_INT_PIN         NFC_A_BUSY_Pin            /* GPIO pin used for ST25R External Interrupt    */
#  define ST25R_INT_PORT        NFC_A_BUSY_GPIO_Port      /* GPIO port used for ST25R External Interrupt   */

#  define ST25R_INT_NUM         NFC_A_BUSY_EXTI_IRQn
# else
#  error "Must define HW peripherals for NFC reader"
# endif
#endif

#if FTR_EINK
# if BOARD_G4
#  define EINK_SPI hspi2
# elif BOARD_L0
#  define EINK_SPI hspi1
# elif BOARD_G0
#  define EINK_SPI hspi3
# else
#  error "No SPI peripheral defined for eink"
# endif
#endif

#if BOARD_G0
# define USER_LED_GPIO_Port LED_DEBUG_G_GPIO_Port
# define USER_LED_Pin       LED_DEBUG_G_Pin
#endif


