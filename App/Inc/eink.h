#pragma once

#include "app.h"

#if TYPE_CARD


#include <stdint.h>

/** SPI Peripheral **/
#if defined(BOARD_G4)
# define EINK_SPI hspi2
#elif defined(BOARD_L0)
# define EINK_SPI hspi1
#else
# error "No SPI peripheral defined for eink"
#endif

/* 3.70 Inch EPD */
#define EINK_SCREEN_SIZE_V 416
#define EINK_SCREEN_SIZE_H 240

#define ROW_BYTES ((uint32_t) (EINK_SCREEN_SIZE_H / 8))

#define EINK_IMAGE_DATA_SIZE (((uint32_t) (EINK_SCREEN_SIZE_V)) * ROW_BYTES)

typedef uint8_t EPDBuf[EINK_SCREEN_SIZE_V][ROW_BYTES];

// Driver Functions
void eink_fullUpdate(EPDBuf buf);
void eink_powerUp(void);

// Library Functions
void eink_clearBuf(EPDBuf buf);
void eink_setPixel(EPDBuf buf, uint16_t x, uint16_t y);
void eink_setRow(EPDBuf buf, uint16_t y, uint8_t val);
void eink_demo(void);

#endif
