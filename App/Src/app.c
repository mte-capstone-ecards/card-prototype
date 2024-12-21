
#include "app.h"
#include "main.h"

#define EINK defined(BOARD_G4)

void app_main(void)
{
#if EINK
	#include "eink.h"
	eink_main();
#else

	while (1);
#endif
}