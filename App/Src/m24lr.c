#include "m24lr.h"

#include "m24lr_driver.h"
#include <i2c.h>

#define M24LR_I2C	hi2c1

M24LR_StatusTypeDef M24lr_IO_Init( void )
{
  return M24LR_OK; // All IO initialization already done
}

M24LR_StatusTypeDef M24lr_IO_MemWrite( const uint8_t * const pData, const uint8_t DevAddr, const uint16_t TarAddr, const uint16_t Size )
{
	uint8_t *pbuffer = (uint8_t *)pData;

	const HAL_StatusTypeDef status = HAL_I2C_Mem_Write( &M24LR_I2C, DevAddr, TarAddr, I2C_MEMADD_SIZE_16BIT, pbuffer, Size, M24LR_I2C_TIMEOUT );
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

M24LR_StatusTypeDef M24lr_IO_MemRead( uint8_t * const pData, const uint8_t DevAddr, const uint16_t TarAddr, const uint16_t Size )
{
	uint8_t *pbuffer = (uint8_t *)pData;

	const HAL_StatusTypeDef status = HAL_I2C_Mem_Read( &M24LR_I2C, DevAddr, TarAddr, I2C_MEMADD_SIZE_16BIT, pbuffer, Size, M24LR_I2C_TIMEOUT );

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

M24LR_StatusTypeDef M24lr_IO_IsDeviceReady( const uint8_t DevAddr, const uint32_t Trials )
{
	const HAL_StatusTypeDef status = HAL_I2C_IsDeviceReady( &M24LR_I2C, DevAddr, Trials, M24LR_I2C_TIMEOUT );

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
