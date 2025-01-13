
#include "app.h"
#include "main.h"

#include "eink.h"
#include "st25r.h"

void app_main(void)
{
#if CARD
	eink_demo();
#elif CONTROLLER
	st25r_main();
#else
	while (1)
	{
		HAL_GPIO_TogglePin(USER_LED_GPIO_Port, USER_LED_Pin);
		HAL_Delay(1000);
	}
#endif
}
