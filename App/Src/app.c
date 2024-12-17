
#include "app.h"
#include "main.h"

#define EINK defined(BOARD_G4)

#if EINK
# include "spi.h"
#endif

#if EINK

// All in NS - TODO: Implement seperate values for READ/WRITE (Table 12-1)
#define T_CSSU 		100
#define T_CSHLD 	50
#define T_CSHIGH 	250

// TODO: Actually implement this
#define DELAY_NS(delay) HAL_Delay(1)

// static void waitBusy(void)
// {
// 	while (HAL_GPIO_ReadPin(EINK_BUSY_GPIO_Port, EINK_BUSY_Pin) == GPIO_PIN_SET);

// 	return;
// }

static void hwReset(void)
{
	HAL_GPIO_WritePin(EINK_RESET_GPIO_Port, EINK_RESET_Pin, GPIO_PIN_RESET);
}

static void writeCmd(uint8_t cmd)
{
	HAL_GPIO_WritePin(EINK_DC_GPIO_Port, EINK_DC_Pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi2, &cmd, 1, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(EINK_DC_GPIO_Port, EINK_DC_Pin, GPIO_PIN_SET);
}

static void writeData(uint8_t data)
{
	HAL_SPI_Transmit(&hspi2, &data, 1, HAL_MAX_DELAY);
}

static uint8_t  readData(void)
{

}

// static void writeData(uint8_t cmd, uint8_t *data, uint8_t dataLen)
// {
// 	HAL_GPIO_WritePin(EINK_DC_GPIO_Port, EINK_DC_Pin, GPIO_PIN_RESET);
// 	HAL_GPIO_WritePin(EINK_SPI_CS_GPIO_Port, EINK_SPI_CS_Pin, GPIO_PIN_RESET);

// 	DELAY_NS(T_CSSU);

// 	// Write cmd
// 	HAL_SPI_Transmit(&hspi2, &cmd, 1U, HAL_MAX_DELAY);

// 	if (dataLen > 0 && data != NULL)
// 	{
// 		HAL_GPIO_WritePin(EINK_DC_GPIO_Port, EINK_DC_Pin, GPIO_PIN_SET);
// 		HAL_SPI_Transmit(&hspi2, data, dataLen, HAL_MAX_DELAY);
// 	}

// 	// Deassert CS
// 	DELAY_NS(T_CSHLD);
// 	HAL_GPIO_WritePin(EINK_SPI_CS_GPIO_Port, EINK_SPI_CS_Pin, GPIO_PIN_SET);
// 	DELAY_NS(T_CSHIGH);
// }

// static void readData(uint8_t cmd, volatile uint8_t *data, uint8_t dataLen)
// {
// 	HAL_GPIO_WritePin(EINK_DC_GPIO_Port, EINK_DC_Pin, GPIO_PIN_RESET);
// 	HAL_GPIO_WritePin(EINK_SPI_CS_GPIO_Port, EINK_SPI_CS_Pin, GPIO_PIN_RESET);

// 	DELAY_NS(T_CSSU);

// 	HAL_SPI_Transmit(&hspi2, &cmd, 1U, HAL_MAX_DELAY);

// 	// Read back temperature data
// 	HAL_GPIO_WritePin(EINK_DC_GPIO_Port, EINK_DC_Pin, GPIO_PIN_SET);
// 	HAL_SPI_Receive(&hspi2, (uint8_t *) data, dataLen, HAL_MAX_DELAY);

// 	// Deassert CS
// 	DELAY_NS(T_CSHLD);
// 	HAL_GPIO_WritePin(EINK_SPI_CS_GPIO_Port, EINK_SPI_CS_Pin, GPIO_PIN_SET);
// 	DELAY_NS(T_CSHIGH);
// }

#endif

void app_main(void)
{

#if EINK
	HAL_GPIO_WritePin(EINK_SPI_CS_GPIO_Port, EINK_SPI_CS_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(EINK_DC_GPIO_Port, EINK_DC_Pin, GPIO_PIN_SET);
#endif

	while (1)
	{
#if defined(BOARD_NUCLEO)
		HAL_GPIO_TogglePin(USER_LED_GPIO_Port, USER_LED_Pin);

		HAL_Delay(1000);
#endif

#if EINK
		// HAL_Delay(10);

		// // // Read Temp
		// // volatile uint8_t readBuf[2] = { 0U };
		// // readData(0x1B, readBuf, 2);

		// // HAL_Delay(100);

		// // // Write temperature write command
		// // uint8_t writeBuf[2] = { 0xDE, 0xC0 };
		// // writeData(0x1A, writeBuf, 2);

		// // HAL_Delay(100);

		// // // Read temp
		// // volatile uint8_t readBuf2[2] = { 0U };
		// // readData(0x1B, readBuf2, 2);

		// // HAL_Delay(5);

#endif
	}

}
