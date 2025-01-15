#pragma once

#include "app.h"

#if TYPE_CARD

#include <stdint.h>
#include <stdbool.h>

#define M24LR_USER_SPACE		(0U << 2U)
#define M24LR_SYSTEM_SPACE	    (1U << 2U)

// M24 Functions
void M24LR_init(void);

void M24LR_randomAddressRead(uint8_t memorySpace, uint16_t dataAddr, uint8_t nbytes, uint8_t *buf);
void M24LR_byteWrite(uint8_t memorySpace, uint16_t dataAddr, uint8_t byte);
void M24LR_pageWrite(uint8_t memorySpace, uint16_t pageAlignedAddr, uint8_t *pageBuf);

bool M24LR_isBusy(void);

#endif
