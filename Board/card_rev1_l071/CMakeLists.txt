# F446 CMake Files

message(STATUS "Building L0x1 Directory")

# MCU Properties #
set (MCPU
	"-mcpu=cortex-m0plus"
PARENT_SCOPE)

set (MFPU
	""
PARENT_SCOPE)

set (MFLOAT_ABI
	"-mfloat-abi=soft"
PARENT_SCOPE)

set (RUNTIME_LIBRARY
	"--specs=nano.specs"
PARENT_SCOPE)

set (RUNTIME_LIBRARY_SYSCALLS
	"--specs=nosys.specs"
PARENT_SCOPE)

# MCU Files #
set(L0x1_DIR ${CMAKE_CURRENT_SOURCE_DIR})

set (LINKER_SCRIPT
	${L0x1_DIR}/STM32L071K8UX_FLASH.ld
PARENT_SCOPE)

set (BOARD_SOURCES
	${L0x1_DIR}/Core/Startup/startup_stm32l071k8ux.s
	${L0x1_DIR}/Core/Src/main.c
	${L0x1_DIR}/Core/Src/adc.c
	${L0x1_DIR}/Core/Src/gpio.c
	${L0x1_DIR}/Core/Src/spi.c
	${L0x1_DIR}/Core/Src/i2c.c
	${L0x1_DIR}/Core/Src/stm32l0xx_hal_msp.c
	${L0x1_DIR}/Core/Src/stm32l0xx_it.c
	${L0x1_DIR}/Core/Src/syscalls.c
	${L0x1_DIR}/Core/Src/sysmem.c
	${L0x1_DIR}/Core/Src/system_stm32l0xx.c
	${L0x1_DIR}/Drivers/STM32L0xx_HAL_Driver/Src/stm32l0xx_hal.c
	${L0x1_DIR}/Drivers/STM32L0xx_HAL_Driver/Src/stm32l0xx_hal_adc.c
	${L0x1_DIR}/Drivers/STM32L0xx_HAL_Driver/Src/stm32l0xx_hal_adc_ex.c
	${L0x1_DIR}/Drivers/STM32L0xx_HAL_Driver/Src/stm32l0xx_hal_cortex.c
	${L0x1_DIR}/Drivers/STM32L0xx_HAL_Driver/Src/stm32l0xx_hal_dma.c
	${L0x1_DIR}/Drivers/STM32L0xx_HAL_Driver/Src/stm32l0xx_hal_exti.c
	${L0x1_DIR}/Drivers/STM32L0xx_HAL_Driver/Src/stm32l0xx_hal_flash.c
	${L0x1_DIR}/Drivers/STM32L0xx_HAL_Driver/Src/stm32l0xx_hal_flash_ex.c
	${L0x1_DIR}/Drivers/STM32L0xx_HAL_Driver/Src/stm32l0xx_hal_flash_ramfunc.c
	${L0x1_DIR}/Drivers/STM32L0xx_HAL_Driver/Src/stm32l0xx_hal_gpio.c
	${L0x1_DIR}/Drivers/STM32L0xx_HAL_Driver/Src/stm32l0xx_hal_i2c.c
	${L0x1_DIR}/Drivers/STM32L0xx_HAL_Driver/Src/stm32l0xx_hal_i2c_ex.c
	${L0x1_DIR}/Drivers/STM32L0xx_HAL_Driver/Src/stm32l0xx_hal_spi.c
	${L0x1_DIR}/Drivers/STM32L0xx_HAL_Driver/Src/stm32l0xx_hal_pwr.c
	${L0x1_DIR}/Drivers/STM32L0xx_HAL_Driver/Src/stm32l0xx_hal_pwr_ex.c
	${L0x1_DIR}/Drivers/STM32L0xx_HAL_Driver/Src/stm32l0xx_hal_rcc.c
	${L0x1_DIR}/Drivers/STM32L0xx_HAL_Driver/Src/stm32l0xx_hal_rcc_ex.c
	${L0x1_DIR}/Drivers/STM32L0xx_HAL_Driver/Src/stm32l0xx_hal_tim.c
PARENT_SCOPE)

set (BOARD_INCLUDES
	${L0x1_DIR}/Core/Inc
	${L0x1_DIR}/Drivers/STM32L0xx_HAL_Driver/Inc
	${L0x1_DIR}/Drivers/STM32L0xx_HAL_Driver/Inc/Legacy
	${L0x1_DIR}/Drivers/CMSIS/Device/ST/STM32L0xx/Include
	${L0x1_DIR}/Drivers/CMSIS/Include
PARENT_SCOPE)

set (BOARD_DEFINES
	STM32L071xx
	USE_HAL_DRIVER
	BOARD_L0
	BOARD_CARD
PARENT_SCOPE)

set (SVD_FILE
	${CMAKE_SOURCE_DIR}/Board/SVD/L0/STM32L0x1.svd
PARENT_SCOPE)
