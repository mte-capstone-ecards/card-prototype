
#include "app.h"
#include "main.h"

#include "eink.h"
#include "st25r.h"
#include "m24lr_driver.h"

#if OS_FREERTOS
# include "cmsis_os.h"
#endif

void app_main(void)
{
#if BOARD_G4
	HAL_Delay(1000);

	while (1)
	{
		HAL_GPIO_TogglePin(USER_LED_GPIO_Port, USER_LED_Pin);
		HAL_Delay(1000);
	}
#elif TYPE_CARD
	eink_demo();
#elif TYPE_CONTROLLER
	ST25R_main();
#else
	while (1)
	{
		HAL_GPIO_TogglePin(USER_LED_GPIO_Port, USER_LED_Pin);
		HAL_Delay(1000);
	}
#endif
}

#if OS_FREERTOS
void App_HeartbeatTask(void *args)
{
	static uint8_t read0[] = ST25R_CMD_READ_SINGLE_BLOCK(0x00);

	static uint8_t write0[] = ST25R_CMD_WRITE_SINGLE_BLOCK(0x00, 0xDEADBEEF);
	// static uint8_t write0[] = ST25R_CMD_WRITE_SINGLE_BLOCK(0x00, 0xBEEFDEAD);

	static uint8_t read1[] = ST25R_CMD_READ_SINGLE_BLOCK(0x01);

	static uint8_t write1[] = ST25R_CMD_WRITE_SINGLE_BLOCK(0x01, 0x00112233);

	static ST25R_command cmd_read0 = {
		.payload = read0,
		.payloadSize = sizeof(read0),
	};

	static ST25R_command cmd_read1 = {
		.payload = read1,
		.payloadSize = sizeof(read1),
	};

	static ST25R_command cmd_write0 = {
		.payload = write0,
		.payloadSize = sizeof(write0),
	};

	static ST25R_command cmd_write1 = {
		.payload = write1,
		.payloadSize = sizeof(write1),
	};


	extern osMessageQueueId_t nfcCommandQueueHandle;
	// osMessageQueuePut(nfcCommandQueueHandle, &cmd_read1, 	0, 10);
	// osMessageQueuePut(nfcCommandQueueHandle, &cmd_write1, 	0, 10);
	// osMessageQueuePut(nfcCommandQueueHandle, &cmd_read1, 	0, 10);

	for (;;)
	{
		HAL_GPIO_TogglePin(USER_LED_GPIO_Port, USER_LED_Pin);

		if (osMessageQueueGetCount(nfcCommandQueueHandle) == 0)
		{
			osMessageQueuePut(nfcCommandQueueHandle, &cmd_read0, 	0, 10);
			// osMessageQueuePut(nfcCommandQueueHandle, &cmd_write0, 	0, 10);
			// osMessageQueuePut(nfcCommandQueueHandle, &cmd_read0, 	0, 10);
		}

		osDelay(500);
	}
}
#endif
