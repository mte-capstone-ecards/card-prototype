# F446 CMake Files

message(STATUS "Building F4 Directory")

# MCU Properties #
set (MCPU
	"-mcpu=cortex-M4"
PARENT_SCOPE)

set (MFPU
	"-mfpu=fpv4-sp-d16"
PARENT_SCOPE)

set (MFLOAT_ABI
	""
PARENT_SCOPE)

set (RUNTIME_LIBRARY
	"--specs=nano.specs"
PARENT_SCOPE)

set (RUNTIME_LIBRARY_SYSCALLS
	"--specs=nosys.specs"
PARENT_SCOPE)

# MCU Files #
set(F4_DIR ${CMAKE_CURRENT_SOURCE_DIR})

set (LINKER_SCRIPT
	${F4_DIR}/STM32F446RETX_FLASH.ld
PARENT_SCOPE)

set (BOARD_SOURCES
	${F4_DIR}/Core/Startup/startup_stm32f446retx.s
	${F4_DIR}/Core/Src/main.c
	${F4_DIR}/Core/Src/gpio.c
	${F4_DIR}/Core/Src/usart.c
	${F4_DIR}/Core/Src/stm32f4xx_hal_msp.c
	${F4_DIR}/Core/Src/stm32f4xx_it.c
	${F4_DIR}/Core/Src/syscalls.c
	${F4_DIR}/Core/Src/sysmem.c
	${F4_DIR}/Core/Src/system_stm32f4xx.c
	${F4_DIR}/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal.c
	${F4_DIR}/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_cortex.c
	${F4_DIR}/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc.c
	${F4_DIR}/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_uart.c
	${F4_DIR}/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_gpio.c
	${F4_DIR}/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dma.c
PARENT_SCOPE)

set (BOARD_INCLUDES
	${F4_DIR}/Core/Inc
	${F4_DIR}/Drivers/STM32F4xx_HAL_Driver/Inc
	${F4_DIR}/Drivers/CMSIS/Include
	${F4_DIR}/Drivers/CMSIS/Device/ST/STM32F4xx/Include
PARENT_SCOPE)

set (BOARD_DEFINES
	STM32F446xx
	USE_HAL_DRIVER
	BOARD_F4
	BOARD_NUCLEO
PARENT_SCOPE)

set (SVD_FILE
	${CMAKE_SOURCE_DIR}/Board/SVD/G4/STM32G431.svd
PARENT_SCOPE)
