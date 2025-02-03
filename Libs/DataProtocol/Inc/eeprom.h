#pragma once

#include "base_defs.h"

#if FTR_EEPROM

#include <stdint.h>
#include <assert.h>
#include <stdbool.h>

#if FTR_DATASENDER
# define SENDER_WRITE   volatile
# define READER_WRITE   const volatile

typedef uint32_t eepromWord;
#elif FTR_DATARECEIVER
# define SENDER_WRITE   const volatile
# define READER_WRITE   volatile

typedef uint8_t eepromWord;
#endif

typedef struct {
    uint8_t seqNum;

    struct {
        uint8_t update:1;
    };

    uint8_t dummy[2];
} SenderHeader;

typedef struct {
    uint8_t seqNum;

    struct {
        uint8_t ack:1;
        uint8_t updated:1;
    };

    uint8_t dummy[2];
} receiverHeader;

static_assert(sizeof(SenderHeader) == 4, "Sender header must fit in a block");
static_assert(sizeof(receiverHeader) == 4, "Reader header must fit in a block");

// Eeprom of M24LR
#define EEPROM_NUM_SECTORS  4U
#define EEPROM_SIZE (4 * 1024 / 8) // 4 kBit
#define EEPROM_WORDS (EEPROM_SIZE / 4)
#define EEPROM_DATA_BYTES (EEPROM_SIZE - sizeof(SenderHeader) - sizeof(receiverHeader))
#define EEPROM_DATA_WORDS (EEPROM_DATA_BYTES / 4)

typedef volatile uint32_t Sector[32];

typedef struct
{
    union {
        struct {
            SenderHeader senderHeader;
            receiverHeader receiverHeader;

            SENDER_WRITE union {
                uint8_t     u8[EEPROM_DATA_BYTES];
                uint32_t    u32[EEPROM_DATA_WORDS];
            } data;
        };

        Sector sectors[4];
    };
} Eeprom;
extern volatile Eeprom eeprom;

bool Eeprom_readSector(uint8_t sector);
bool Eeprom_readAll(void);
bool Eeprom_waiting(void);
bool Eeprom_writeNextSeqId(void);
bool Eeprom_readSenderHeader(void);
bool Eeprom_readReceiverHeader(void);
bool Eeprom_partnerStale(void);
bool Eeprom_writeData(uint8_t dataAddr, uint32_t *data, uint16_t len);

#endif
