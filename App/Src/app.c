
#include "app.h"
#include "main.h"


#if OS_FREERTOS
# include "cmsis_os.h"
# include "st25r.h"

# include "data_protocol.h"
# include "eeprom.h"
# include "sender.h"

# include "led.h"

# if FTR_GUI
#  include "gui.h"
# endif
#endif

#if OS_BAREMETAL

#elif OS_FREERTOS
extern uint32_t period;
uint32_t period = 20;

#define NUM_WORDS ((416 * (240 / 8)) / 4)
uint32_t words[NUM_WORDS];

void Controller_hearbeatTask(void *args)
{
#if FTR_GUI
	GUI_init();
#endif

#if FTR_LED
	LED_enableHz(LED_DEBUG_B, 1);
#endif

	while(1)
	{
		osDelay(500);
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

#if BOARD(CONTROLLER, 0)
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	ST25R_EXTICallback(GPIO_Pin);
}
#endif

#if BOARD(CONTROLLER, 1)
void HAL_GPIO_EXTI_Rising_Callback(uint16_t GPIO_Pin)
{
	Button_EXTIRisingCallback(GPIO_Pin);
	ST25R_EXTICallback(GPIO_Pin);
}

void HAL_GPIO_EXTI_Falling_Callback(uint16_t GPIO_Pin)
{
	Button_EXTIRisingCallback(GPIO_Pin);
}
#endif
