#include "m24lr.h"

#include "m24lr_driver.h"

#include <i2c.h>
#include <stm32g431xx.h>

// Device Address Definitions
#define M24LR_ADDR(MEM_SPACE)   (((0b1010011U | (MEM_SPACE)) << 1U))

// User Memory Definitions
#define M24LR_GET_BIT(data, bit) ( (data) & (1U << (bit)) )
#define M24LR_SET_BIT(data, bit) ( (data) |= 1U << (bit) )

// System Space Addresses
#define IC_REF_ADDR (2332U)

#define M24LR_I2C	hi2c1

#define M24LR_I2C_IsActiveFlag(reg, flag) (((M24LR_I2C->reg & flag) == (flag)) ? 1UL : 0UL)

// Utility Functions
#define MSB(addr) 	((addr) >> 8U & 0xFFU)
#define LSB(addr) 	((addr) & 0xFFU)


static void completeByteTransfer(uint8_t byte)
{
	M24LR_I2C->TXDR = byte;
	while( !M24LR_I2C_IsActiveFlag(ISR, I2C_ISR_TXE) );
}

void M24LR_randomAddressRead(uint8_t memorySpace, uint16_t dataAddr, uint8_t nbytes, uint8_t *buf)
{
	uint8_t deviceAddress = M24LR_ADDR(memorySpace);

	// Initiate Address Write; 2 Address Bytes
	// Key note: No auto-end to allow for restart condition
	M24LR_I2C->CR2 =
		1U << 26U | // PEC Enable
		0U << 25U | // No Auto end
		0U << 24U | // No Reload
		2U << 16U | // 2 Bytes Transmitted
		0U << 15U | // Slave Mode Feature (nACK)
		0U << 14U | // No Stop Bit Generation
		1U << 13U | // Start Bit Generation
		0U << 12U | // Ignore 10 Bit Address Feature
		0U << 11U | // Set into 7 Bit Address mode
		0U << 10U | // Write
		deviceAddress
	;

	completeByteTransfer(MSB(dataAddr));
	completeByteTransfer(LSB(dataAddr));
	while ( !M24LR_I2C_IsActiveFlag(ISR, I2C_ISR_TC) );

	// Initiate Data Read at Address; NBytes being read
	M24LR_I2C->CR2 =
		1U 		<< 26U | // PEC Enable
		1U 		<< 25U | // Auto end
		0U 		<< 24U | // No reload
		nbytes 	<< 16U | // Transmit all the bytes
		0U 		<< 15U | // Slave Mode Feature (nACK)
		0U 		<< 14U | // No Stop Bit Generation
		1U 		<< 13U | // Start Bit Generation
		0U 		<< 12U | // Ignore 10 Bit Address Feature
		0U 		<< 11U | // Set into 7 Bit Address mode
		1U 		<< 10U | // Read
		deviceAddress
	;

	for (int index = 0U; index < nbytes; index++)
	{
	  while( !M24LR_I2C_IsActiveFlag(ISR, I2C_ISR_RXNE) );
	  buf[index] = M24LR_I2C->RXDR;
	}

	while( !M24LR_I2C_IsActiveFlag(ISR, I2C_ISR_STOPF) );
}

M24LR_StatusTypeDef NFCTAG_IO_MemWrite( const uint8_t * const pData, const uint8_t DevAddr, const uint16_t TarAddr, const uint16_t Size )
{
	uint8_t *pbuffer = (uint8_t *)pData;

	const HAL_StatusTypeDef status = HAL_I2C_Mem_Write( &M24LR_I2C, DevAddr, TarAddr, I2C_MEMADD_SIZE_16BIT, pbuffer, Size, HAL_MAX_DELAY );
	switch( status )
	{
		case HAL_OK:
			return M24LR_OK;
		case HAL_ERROR:
			return M24LR_ERROR;
		case HAL_BUSY:
			return M24LR_BUSY;
		case HAL_TIMEOUT:
			return M24LR_TIMEOUT;

		default:
			return M24LR_TIMEOUT;
	}
}

void M24LR_byteWrite(uint8_t memorySpace, uint16_t dataAddr, uint8_t byte)
{
	uint8_t deviceAddress = M24LR_ADDR(memorySpace);

	// Initiate Data Write at Address; 3 bytes need to be written (2 Addr + 1 Byte)
	M24LR_I2C->CR2 =
		1U 	<< 26U | // PEC Enable
		1U 	<< 25U | // Auto end
		0U 	<< 24U | // No reload
		3U 	<< 16U | // Transmit all the bytes
		0U 	<< 15U | // Slave Mode Feature (nACK)
		0U  << 14U | // No Stop Bit Generation
		1U 	<< 13U | // Start Bit Generation
		0U 	<< 12U | // Ignore 10 Bit Address Feature
		0U 	<< 11U | // Set into 7 Bit Address mode
		0U 	<< 10U | // Write
		deviceAddress
	;

	completeByteTransfer(MSB(dataAddr));
	completeByteTransfer(LSB(dataAddr));
	completeByteTransfer(byte);
	while( !M24LR_I2C_IsActiveFlag(ISR, I2C_ISR_STOPF) );

	while ( M24LR_isBusy() );
}

void M24LR_pageWrite(uint8_t memorySpace, uint16_t pageAlignedAddr, uint8_t *pageBuf)
{
	uint8_t deviceAddress = M24LR_ADDR(memorySpace);

	// Initiate Data Write at Address; 6 bytes need to be written (2 Addr + 4 Byte)
	M24LR_I2C->CR2 =
		1U 	<< 26U | // PEC Enable
		1U 	<< 25U | // Auto end
		0U 	<< 24U | // No reload
		6U 	<< 16U | // Transmit all the bytes
		0U 	<< 15U | // Slave Mode Feature (nACK)
		0U  << 14U | // No Stop Bit Generation
		1U 	<< 13U | // Start Bit Generation
		0U 	<< 12U | // Ignore 10 Bit Address Feature
		0U 	<< 11U | // Set into 7 Bit Address mode
		0U 	<< 10U | // Write
		deviceAddress
	;

	completeByteTransfer(MSB(pageAlignedAddr));
	completeByteTransfer(LSB(pageAlignedAddr));
	for (uint8_t pageIndex = 0U; pageIndex < 4U; pageIndex++)
	{
		completeByteTransfer(pageBuf[pageIndex]);
	}
	while( !M24LR_I2C_IsActiveFlag(ISR, I2C_ISR_STOPF) );

	while ( M24LR_isBusy() );
}

bool M24LR_isBusy(void)
{
	return HAL_GPIO_ReadPin(M24LR_BUSY_GPIO_Port, M24LR_BUSY_Pin) == GPIO_PIN_RESET;
}
