#include "m24lr.h"

#if FTR_NFCTAG

// #include "m24lr_driver.h"
#include <i2c.h>

#define M24LR_MAX_RETRIES	100U

// TODO: Can we maybe block on read/writes until RF busy is false? Increase odds of read/writes suceeding

#define M24LR_I2C_TIMEOUT       200 /* I2C Time out (ms), this is the maximum time needed by M24LR to complete any command */
#define M24LR_ADDR_DATA_I2C     0xA6
#define M24LR_PAGEWRITE_NBBYTE  4

M24LR_StatusTypeDef M24lr_IO_Init( void )
{
  return M24LR_OK; // All IO initialization already done
}

M24LR_StatusTypeDef M24lr_IO_MemWrite( const uint8_t * const pData, const uint8_t DevAddr, const uint16_t TarAddr, const uint16_t Size )
{
	uint8_t retries = M24LR_MAX_RETRIES;
	uint8_t *pbuffer = (uint8_t *)pData;

	HAL_StatusTypeDef status;
	while (retries > 0)
	{
		status = HAL_I2C_Mem_Write( &M24LR_I2C, DevAddr, TarAddr, I2C_MEMADD_SIZE_16BIT, pbuffer, Size, M24LR_I2C_TIMEOUT );

		if (status == HAL_OK)
			return M24LR_OK;

		retries--;
		HAL_Delay(10);
	}

	switch( status )
	{
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
	uint8_t retries = M24LR_MAX_RETRIES;
	uint8_t *pbuffer = (uint8_t *)pData;

	HAL_StatusTypeDef status;
	while (retries > 0)
	{
		status = HAL_I2C_Mem_Read( &M24LR_I2C, DevAddr, TarAddr, I2C_MEMADD_SIZE_16BIT, pbuffer, Size, M24LR_I2C_TIMEOUT );

		if (status == HAL_OK)
			return M24LR_OK;

		retries--;
		HAL_Delay(10);
	}

	switch( status )
	{
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

/**
  * @brief  Check M24LR availability
  * @param  Trials : number of max tentative tried
  * @retval NFCTAG enum status
  */
 M24LR_StatusTypeDef M24LR_i2c_IsDeviceReady( const uint32_t Trials )
 {
   /* Test i2c with M24LR */
   return M24lr_IO_IsDeviceReady( M24LR_ADDR_DATA_I2C, Trials );
 }

M24LR_StatusTypeDef M24LR_i2c_ReadData( uint8_t * const pData, const uint16_t TarAddr, const uint16_t NbByte )
{
  /* Before calling this function M24LR must be ready, here is a check to detect an issue */
  if( M24LR_i2c_IsDeviceReady( 1 ) != M24LR_OK )
  {
    return M24LR_TIMEOUT;
  }

  return M24lr_IO_MemRead( pData, M24LR_ADDR_DATA_I2C, TarAddr, NbByte );
}

/**
  * @brief  Write N data bytes starting from specified I2C Address
  * @brief  if I2C_Write_Lock bit = 0 or I2C_Password present => ack (modification OK)
  * @brief  if I2C_Write_Lock bit = 1  and no I2C_Password present => No ack (no modification)
  * @param  pData : pointer of the data to write
  * @param  TarAddr : I2C data memory address to write
  * @param  NbByte : number of bytes to write
  * @retval NFCTAG enum status
  */
M24LR_StatusTypeDef M24LR_i2c_WriteData( const uint8_t * const pData, const uint16_t TarAddr, const uint16_t NbByte )
{
  M24LR_StatusTypeDef status;
  uint8_t align_mem_offset;
  uint16_t bytes_to_write = NbByte;
  uint16_t mem_addr = TarAddr;
  const uint8_t *pdata_index = (const uint8_t *)pData;

  /* Before calling this function M24LR must be ready, here is a check to detect an issue */
  if( M24LR_i2c_IsDeviceReady( 1 ) != M24LR_OK )
  {
    return M24LR_TIMEOUT;
  }

  /* M24LR can write a maximum of 4 bytes in EEPROM per i2c communication */
  do
  {
    /* To write data in M24LR, data must be aligned on the same row in memory */
    /* align_mem_offset is used to copy only Bytes that are on the same row  in memory */
    if( bytes_to_write > M24LR_PAGEWRITE_NBBYTE )
    {
      /* DataSize higher than max page write, copy data by page */
      align_mem_offset = M24LR_PAGEWRITE_NBBYTE - (mem_addr % M24LR_PAGEWRITE_NBBYTE);
    }
    else
    {
      /* DataSize lower or equal to max page write, copy only last bytes */
      align_mem_offset = bytes_to_write;
    }
    /* Write align_mem_offset bytes in memory */
    status = M24lr_IO_MemWrite( pdata_index, M24LR_ADDR_DATA_I2C, mem_addr, align_mem_offset );
    /* update index, dest address, size for next write */
    pdata_index += align_mem_offset;
    mem_addr += align_mem_offset;
    bytes_to_write -= align_mem_offset;
    /* Poll until EEPROM is available */
    while( M24LR_i2c_IsDeviceReady( 1 ) != M24LR_OK ) {};
  }
  while( ( bytes_to_write > 0 ) && ( status == M24LR_OK ) );

  return status;
}

#endif
