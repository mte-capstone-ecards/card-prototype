
#include "app.h"
#include "main.h"

void app_main(void)
{

	while (1)
	{
#if defined(BOARD_NUCLEO)

#elif defined(BOARD_CARD)

#endif

#if defined(BOARD_F4)

#elif defined(BOARD_G4)

#endif

#if defined(BOARD_G4)

#endif
		HAL_GPIO_TogglePin(USER_LED_GPIO_Port, USER_LED_Pin);

		HAL_Delay(1000);
	}

}
