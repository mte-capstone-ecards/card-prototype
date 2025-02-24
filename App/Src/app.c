
#include "app.h"
#include "main.h"


#if OS_FREERTOS
# include "cmsis_os.h"
#include "gui.h"
#include "st25r.h"

#include "data_protocol.h"
#include "eeprom.h"
#include "sender.h"
#endif

#if OS_BAREMETAL

#elif OS_FREERTOS
extern uint32_t period;
uint32_t period = 20;

#define NUM_WORDS ((416 * (240 / 8)) / 4)
uint32_t words[NUM_WORDS];

void Controller_hearbeatTask(void *args)
{
	GUI_init();

	while(1)
	{
		HAL_GPIO_TogglePin(LED_DEBUG_R_GPIO_Port, LED_DEBUG_R_Pin);
		HAL_GPIO_TogglePin(LED_DEBUG_G_GPIO_Port, LED_DEBUG_G_Pin);
		HAL_GPIO_TogglePin(LED_DEBUG_B_GPIO_Port, LED_DEBUG_B_Pin);
		HAL_Delay(500);
	}

#if FTR_DATASENDER
	SenderDataSpec senderData;


	senderData.startBit = 0;
	senderData.data = words;
	senderData.numWords = NUM_WORDS;

	for (uint32_t i = 0; i < NUM_WORDS; i++)
	{
		words[i] = i;
	}

#endif

	static uint32_t counter = 0;

	for (;;)
	{
#if FTR_DATASENDER
		extern osMessageQueueId_t dataSenderQueueHandle;
		osMessageQueuePut(dataSenderQueueHandle, &senderData, 0, 10);
#endif

		osDelay(period);
	}
}
#endif
