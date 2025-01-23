
#include "app.h"
#include "main.h"

#include "eink.h"
#include "st25r.h"

#if TYPE_CARD
#include "m24lr_driver.h"
#endif

#if OS_FREERTOS
# include "cmsis_os.h"
#endif

#if OS_BAREMETAL
static void m24lr_i2c_demo()
{
	M24lr_i2c_Drv.Init();

	M24LR_StatusTypeDef status = M24lr_i2c_Drv.IsReady(1);
	if (status != M24LR_OK)
	{
		while(1);
	}

	volatile M24LR_EH_MODE_STATUS mode;
	volatile M24LR_EH_CFG_VOUT cfg;

	M24lr_i2c_ExtDrv.Read_EH_mode((M24LR_EH_MODE_STATUS *)&mode);
	M24lr_i2c_ExtDrv.Enable_EH_mode();
	M24lr_i2c_ExtDrv.Read_EH_mode((M24LR_EH_MODE_STATUS *)&mode);

	M24lr_i2c_ExtDrv.WriteEH_Cfg(M24LR_EH_Cfg_3MA);
	M24lr_i2c_ExtDrv.ReadEH_Cfg((M24LR_EH_CFG_VOUT *)&cfg);
	M24lr_i2c_ExtDrv.WriteEH_Cfg(M24LR_EH_Cfg_6MA);
	M24lr_i2c_ExtDrv.ReadEH_Cfg((M24LR_EH_CFG_VOUT *)&cfg);
}

static void sdcard_demo()
{
	// Code





	// Trap
	while (1);
}

void app_main(void)
{
#if TYPE_CARD
	m24lr_i2c_demo();
	// sdcard_demo();
#elif TYPE_CONTROLLER
	ST25R_main();
#endif

	while (1)
	{

	}
}

#elif OS_FREERTOS

void App_HeartbeatTask(void *args)
{
	for (;;)
	{

		osDelay(500);
	}
}
#endif
