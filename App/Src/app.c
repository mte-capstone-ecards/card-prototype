
#include "app.h"
#include "main.h"


#if OS_FREERTOS
# include "cmsis_os.h"
# include "st25r.h"

# include "data_protocol.h"
# include "eeprom.h"
# include "sender.h"

# include "button.h"
# include "led.h"

#endif

#if OS_BAREMETAL

#elif OS_FREERTOS
extern uint32_t period;
uint32_t period = 20;

#define NUM_WORDS ((416 * (240 / 8)) / 4)
uint32_t words[NUM_WORDS];

void Controller_hearbeatTask(void *args)
{
	while(1)
	{
		osDelay(500);
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
	Button_EXTIFallingCallback(GPIO_Pin);
}
#endif
