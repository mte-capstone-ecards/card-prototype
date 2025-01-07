
#include "app.h"
#include "main.h"

void app_main(void)
{
#if EINK
	#include "eink.h"
	eink_demo();
#elif defined(BOARD_L0)
	while (1)
	{
		HAL_GPIO_TogglePin(USER_LED_GPIO_Port, USER_LED_Pin);
		HAL_Delay(500);
	}
#endif
}