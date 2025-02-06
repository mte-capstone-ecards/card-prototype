
#include "app.h"
#include "main.h"

#include "eink.h"
#include "st25r.h"

#if OS_FREERTOS
# include "cmsis_os.h"
#endif

#if OS_BAREMETAL
// static void m24lr_i2c_demo()
// {
// 	M24lr_i2c_Drv.Init();

// 	M24LR_StatusTypeDef status = M24lr_i2c_Drv.IsReady(1);
// 	if (status != M24LR_OK)
// 	{
// 		while(1);
// 	}

// 	volatile M24LR_EH_MODE_STATUS mode;
// 	volatile M24LR_EH_CFG_VOUT cfg;

// 	M24lr_i2c_ExtDrv.Read_EH_mode((M24LR_EH_MODE_STATUS *)&mode);
// 	M24lr_i2c_ExtDrv.Enable_EH_mode();
// 	M24lr_i2c_ExtDrv.Read_EH_mode((M24LR_EH_MODE_STATUS *)&mode);

// 	M24lr_i2c_ExtDrv.WriteEH_Cfg(M24LR_EH_Cfg_3MA);
// 	M24lr_i2c_ExtDrv.ReadEH_Cfg((M24LR_EH_CFG_VOUT *)&cfg);
// 	M24lr_i2c_ExtDrv.WriteEH_Cfg(M24LR_EH_Cfg_6MA);
// 	M24lr_i2c_ExtDrv.ReadEH_Cfg((M24LR_EH_CFG_VOUT *)&cfg);
// }

static void sdcard_demo()
{
	// Code





	// Trap
	while (1);
}

#elif OS_FREERTOS

#include "data_protocol.h"
#include "eeprom.h"
#include "sender.h"

extern uint32_t period;
uint32_t period = 500;

#define NUM_WORDS ((416 * (240 / 8)) / 4)
uint32_t words[NUM_WORDS];

void Controller_hearbeatTask(void *args)
{
#if FTR_DATASENDER
	SenderDataSpec senderData;


	senderData.startBit = 0;
	senderData.data = words;
	senderData.numWords = NUM_WORDS;

	for (uint32_t i = 0; i < NUM_WORDS; i++)
	{
		words[i] = (1U << i) - 1U;
	}

#endif

	for (;;)
	{
		HAL_GPIO_TogglePin(USER_LED_GPIO_Port, USER_LED_Pin);

#if FTR_DATASENDER
		extern osMessageQueueId_t dataSenderQueueHandle;
		osMessageQueuePut(dataSenderQueueHandle, &senderData, 0, 10);
#endif

		osDelay(period);
	}
}
#endif
