
#pragma once

#define CARD (defined(BOARD_G4) | defined(BOARD_L0))
#define CONTROLLER (defined(BOARD_F4))

void app_main(void);
