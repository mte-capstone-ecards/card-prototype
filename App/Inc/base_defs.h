#pragma once

/* BOARDS */
#ifndef BOARD_G4
# define BOARD_G4 0
#endif

#ifndef BOARD_L0
# define BOARD_L0 0
#endif

#ifndef BOARD_F4
# define BOARD_F4 0
#endif

#ifndef BOARD_G0
# define BOARD_G0 0
#endif

/* TYPES */
#ifndef TYPE_CARD
# define TYPE_CARD 0
#endif

#ifndef TYPE_CONTROLLER
# define TYPE_CONTROLLER 0
#endif

/* OS */
#ifndef OS_FREERTOS
# define OS_FREERTOS 0
#endif

#define OS_BAREMETAL (!OS_FREERTOS)

/* Features */
#define FTR_SIMEEPROM       (0U)
#define FTR_EINK            (TYPE_CARD | BOARD_G0)
#define FTR_NFCTAG          (TYPE_CARD & !FTR_SIMEEPROM)
#define FTR_NFCREADER       (TYPE_CONTROLLER & !FTR_SIMEEPROM)
#define FTR_LED             (BOARD_G0)
#define FTR_BUTTON          (BOARD_G0)
#define FTR_EEPROM          ((TYPE_CARD | TYPE_CONTROLLER))
#define FTR_DATASENDER      (TYPE_CONTROLLER)
#define FTR_DATARECEIVER    (TYPE_CARD)
#define FTR_GUI             (BOARD_G0)
