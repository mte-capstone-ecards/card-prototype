#pragma once

#include "base_defs.h"
#include "board.h"


#if OS_FREERTOS
#include "watchdog.h"

void Controller_hearbeatTask(void *);
#else
void card_main(void);
#endif
