
#include "app.h"
#include "main.h"

void app_main(void)
{

	while (1)
	{
#if defined(BOARD_NUCLEO)
		HAL_GPIO_TogglePin(USER_LED_GPIO_Port, USER_LED_Pin);

		HAL_Delay(1000);
#endif
	}

}
