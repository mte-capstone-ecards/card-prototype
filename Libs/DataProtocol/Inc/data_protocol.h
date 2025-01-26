#pragma once

#include "base_defs.h"

#include <stdint.h>
#include <stdbool.h>
#include <cassert.h>

#define DATA_CRC_SIZE       2

typedef struct
{
    uint16_t dataLen;
    uint8_t crc[DATA_CRC_SIZE];
} DataHeader;

#define DATA_PACKET_SIZE    256 // Size of total packet in bytes
#define DATA_HEADER_SIZE    (sizeof(DataHeader))

static_assert(DATA_HEADER_SIZE % 4 == 0, "Data header of packet must be a multiple of 4 bytes");

#define DATA_PAYLOAD_SIZE   ((DATA_PACKET_SIZE - DATA_HEADER_SIZE) / 4) // Size of payload buffer in bytes

typedef uint32_t DataPayload[DATA_PAYLOAD_SIZE];

typedef struct
{
    DataHeader header;
    DataPayload payload;
} DataPacket;

#if FTR_DATASENDER
void Data_generateCRC(DataPacket *packet);
#endif

#if FTR_DATARECEIVER
bool Data_validateCRC(DataPacket *packet);
#endif