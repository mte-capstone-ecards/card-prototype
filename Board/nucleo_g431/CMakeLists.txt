# G431 CMake File
message(STATUS "Building G4 Directory")

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
set(G4_DIR ${CMAKE_CURRENT_SOURCE_DIR})

set (LINKER_SCRIPT
	${G4_DIR}/STM32G431RBTX_FLASH.ld
PARENT_SCOPE)

set (BOARD_SOURCES
	${G4_DIR}/Core/Startup/startup_stm32g431rbtx.s
	${G4_DIR}/Core/Src/main.c
	${G4_DIR}/Core/Src/spi.c
	${G4_DIR}/Core/Src/i2c.c
	${G4_DIR}/Core/Src/gpio.c
	${G4_DIR}/Core/Src/stm32g4xx_hal_msp.c
	${G4_DIR}/Core/Src/stm32g4xx_it.c
	${G4_DIR}/Core/Src/syscalls.c
	${G4_DIR}/Core/Src/sysmem.c
	${G4_DIR}/Core/Src/system_stm32g4xx.c
	${G4_DIR}/Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal.c
	${G4_DIR}/Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_pwr_ex.c
	${G4_DIR}/Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_pwr.c
	${G4_DIR}/Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_cortex.c
	${G4_DIR}/Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_cortex.c
	${G4_DIR}/Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_rcc.c
	${G4_DIR}/Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_rcc_ex.c
	${G4_DIR}/Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_gpio.c
	${G4_DIR}/Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_i2c.c
	${G4_DIR}/Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_i2c_ex.c
	${G4_DIR}/Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_spi.c
	${G4_DIR}/Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_dma.c
PARENT_SCOPE)

set (BOARD_INCLUDES
	${G4_DIR}/Core/Inc
	${G4_DIR}/Drivers/STM32G4xx_HAL_Driver/Inc
	${G4_DIR}/Drivers/CMSIS/Include
	${G4_DIR}/Drivers/CMSIS/Device/ST/STM32G4xx/Include
PARENT_SCOPE)

set (BOARD_DEFINES
	STM32G431xx
PARENT_SCOPE)

set (SVD_FILE
	${CMAKE_SOURCE_DIR}/Board/SVD/G4/STM32G431.svd
PARENT_SCOPE)
