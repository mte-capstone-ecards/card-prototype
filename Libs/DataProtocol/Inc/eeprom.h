#pragma once

#include "base_defs.h"

#include <stdint.h>
#include <assert.h>
#include <stdbool.h>

#if FTR_DATASENDER
# define SENDER_WRITE   volatile
# define READER_WRITE   const volatile

typedef uint32_t eepromWord;
#elif FTR_DATAREADER
# define SENDER_WRITE   const volatile
# define READER_WRITE   volatile

typedef uint8_t eepromWord;
#endif

typedef struct
{
    SENDER_WRITE struct {
        uint8_t seqNum;
        struct {
            uint8_t dummy:8;
        } cmd;
    } senderCmdBytes;

    READER_WRITE struct {
        uint8_t seqNum;
        struct {
            uint8_t dummy:8;
        } cmd;
    } readerCfg;
} EepromHeader;

static_assert(sizeof(EepromHeader) % 4 == 0, "Eeprom header must be 4 byte aligned");

// Eeprom of M24LR
#define EEPROM_SIZE (4 * 1024) // 4 kB
#define EEPROM_WORDS (EEPROM_SIZE / 4)
#define EEPROM_DATA_BYTES (EEPROM_SIZE - sizeof(EepromHeader))
#define EEPROM_DATA_WORDS (EEPROM_DATA_BYTES / 4)

typedef struct
{
    EepromHeader header;

    SENDER_WRITE union {
        uint8_t     u8[EEPROM_DATA_BYTES];
        uint32_t    u32[EEPROM_DATA_WORDS];
    } data;
} Eeprom;
extern volatile Eeprom eeprom;

bool Eeprom_writeData(uint16_t addr, eepromWord data);
bool Eeprom_readByte(uint16_t addr, uint32_t *readLoc);
bool Eeprom_readBytes(uint16_t addr, uint8_t len, uint32_t *readLoc);
