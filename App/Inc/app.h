#pragma once

#include "base_defs.h"
#include "board.h"

#if OS_FREERTOS
void App_HeartbeatTask(void *);
#else
void app_main(void);
#endif
