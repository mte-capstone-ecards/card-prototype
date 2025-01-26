#pragma once

#include "base_defs.h"

#include <stdint.h>
#include <stdbool.h>

typedef struct
{
    uint8_t packetId;
} DataHeader;

#define DATA_PACKET_SIZE    256 // Size of total packet in bytes
#define DATA_HEADER_SIZE    (sizeof(DataHeader))
#define DATA_CRC_SIZE       2
#define DATA_PAYLOAD_SIZE   (DATA_PACKET_SIZE - DATA_HEADER_SIZE) // Size of payload buffer in bytes

typedef uint8_t DataPayload[DATA_PAYLOAD_SIZE];

typedef struct
{
    DataHeader header;
    DataPayload payload;
    uint8_t crc[DATA_CRC_SIZE];
} DataPacket;

#if FTR_DATASENDER
void Data_generateCRC(DataPacket *packet);
#endif

#if FTR_DATARECEIVER
bool Data_validateCRC(DataPacket *packet);
#endif