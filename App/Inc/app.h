#pragma once

#include "base_defs.h"

void app_main(void);

#if OS_FREERTOS
void App_HeartbeatTask(void *);
#endif
