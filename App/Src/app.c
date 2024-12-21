
#include "app.h"
#include "main.h"

#define EINK defined(BOARD_G4)

#if EINK
# include "eink.h"
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

static void writeCmd(uint8_t cmd)
{
	HAL_GPIO_WritePin(EINK_DC_GPIO_Port, EINK_DC_Pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi2, &cmd, 1, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(EINK_DC_GPIO_Port, EINK_DC_Pin, GPIO_PIN_SET);
	// HAL_Delay(2);
}

static void writeData(uint8_t data)
{
	HAL_SPI_Transmit(&hspi2, &data, 1, HAL_MAX_DELAY);
	// HAL_Delay(2);
}

// static uint8_t readData(void)
// {
// 	uint8_t data;
// 	HAL_SPI_Receive(&hspi2, &data, 1, HAL_MAX_DELAY);
// 	HAL_Delay(2);
// 	return data;
// }

static void waitBusy(void)
{
	while (HAL_GPIO_ReadPin(EINK_BUSY_GPIO_Port, EINK_BUSY_Pin) == GPIO_PIN_SET);
	return;
}

static void hwReset(void)
{
	HAL_GPIO_WritePin(EINK_RESET_GPIO_Port, EINK_RESET_Pin, GPIO_PIN_RESET);
	HAL_Delay(200);
	HAL_GPIO_WritePin(EINK_RESET_GPIO_Port, EINK_RESET_Pin, GPIO_PIN_SET);
	HAL_Delay(200);

	waitBusy();
}

static void swReset(void)
{
	writeCmd(0x12);
	waitBusy();
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
	eink_main();
#endif


	while (1)
	{
#if defined(BOARD_NUCLEO)
		HAL_GPIO_TogglePin(USER_LED_GPIO_Port, USER_LED_Pin);

		HAL_Delay(1000);
#endif

#if EINK

		writeCmd(0x1A);
		writeData(0xDE);
		writeData(0xC0);
		waitBusy();

        // deriver output control
        writeCmd(0x01);
        writeData(0x27);
        writeData(0x01);
        writeData(0x01);

        // data entry mode
        writeCmd(0x11);
        writeData(0x01);

        // set ram-x addr start/end pos
        writeCmd(0x44);
        writeData(0x00);
        writeData(0x0F);

        // set ram-y addr start/end pos
        writeCmd(0x45);
        writeData(0x27);
        writeData(0x01);
        writeData(0x00);
        writeData(0x00);

        // border waveform
        writeCmd(0x3c);
        writeData(0x05);

        // display update control
        writeCmd(0x21);
    	// 0x6X bypasses red ram
        writeData(0x00);
        writeData(0x80);

        // set ram-x addr cnt to 0
        writeCmd(0x4e);
        writeData(0x00);
        // set ram-y addr cnt to 0x127
        writeCmd(0x4F);
        writeData(0x27);
        writeData(0x01);

		// End initialization
		HAL_Delay(100);

		// Update Memory
		writeCmd(0x24);
		for (int i = 0; i < (128 * 296 / 8); i++)
		{
			writeData(0x00);
		}

		writeCmd(0x26);
		for (int i = 0; i < (128 * 296 / 8); i++)
		{
			writeData(0x00);
		}

		// Display update control
		writeCmd(0x22);
		writeData(0xF7);

		writeCmd(0x20);
		waitBusy();

		HAL_Delay(4000);

#endif
	}

}
