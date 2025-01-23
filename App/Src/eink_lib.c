
#include "eink.h"
#include <string.h>

#if FTR_EINK

void eink_clearBuf(EPDBuf buf)
{
    memset(buf, 0x00, sizeof(EPDBuf));
}

void eink_setPixel(EPDBuf buf, uint16_t x, uint16_t y)
{
    if (x > EINK_SCREEN_SIZE_H)
        return;
    if (y > EINK_SCREEN_SIZE_V)
        return;

    buf[y][x / 8] |= 1U << (x % 8);
}

void eink_setRow(EPDBuf buf, uint16_t y, uint8_t val)
{
    memset(buf[y], val, ROW_BYTES);
}

void eink_demo()
{
    eink_powerUp();

    EPDBuf buf;

    eink_clearBuf(buf);

    uint16_t gridSize = 1;
    for (uint16_t y = 0; y < EINK_SCREEN_SIZE_V; y++)
    {
        uint16_t doubleSize = 2 * gridSize;
        for (uint16_t x = 0; x < EINK_SCREEN_SIZE_H; x++)
        {
            if ((x % doubleSize < gridSize) ^ (y % doubleSize < gridSize))
                eink_setPixel(buf, x, y);
            // if (img[x + y * EINK_SCREEN_SIZE_H])
            //     eink_setPixel(buf, x, y);

        }

        if (y > 40 * gridSize)
            gridSize++;
    }

    eink_fullUpdate(buf);
    return;
}
#endif
