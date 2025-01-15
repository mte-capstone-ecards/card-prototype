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
	"-mfloat-abi=hard"
PARENT_SCOPE)

set (RUNTIME_LIBRARY
	"--specs=nano.specs"
PARENT_SCOPE)

set (RUNTIME_LIBRARY_SYSCALLS
	"--specs=nosys.specs"
PARENT_SCOPE)

# MCU Files #
set(F4_DIR 		${CMAKE_CURRENT_SOURCE_DIR})
set(RTOS_DIR 	${F4_DIR}/Middlewares/Third_Party/FreeRTOS/Source)

set (LINKER_SCRIPT
	${F4_DIR}/STM32F446RETX_FLASH.ld
PARENT_SCOPE)

set (BOARD_SOURCES
	${F4_DIR}/Core/Startup/startup_stm32f446retx.s
	${F4_DIR}/Core/Src/main.c
	${F4_DIR}/Core/Src/gpio.c
	${F4_DIR}/Core/Src/spi.c
	${F4_DIR}/Core/Src/stm32f4xx_hal_msp.c
	${F4_DIR}/Core/Src/stm32f4xx_hal_timebase_tim.c
	${F4_DIR}/Core/Src/stm32f4xx_it.c
	${F4_DIR}/Core/Src/syscalls.c
	${F4_DIR}/Core/Src/sysmem.c
	${F4_DIR}/Core/Src/system_stm32f4xx.c
	${F4_DIR}/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal.c
	${F4_DIR}/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_cortex.c
	${F4_DIR}/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc.c
	${F4_DIR}/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_gpio.c
	${F4_DIR}/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_spi.c
	${F4_DIR}/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dma.c
	${F4_DIR}/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pwr_ex.c
	${F4_DIR}/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_tim.c
	${F4_DIR}/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_tim_ex.c


	${RTOS_DIR}/CMSIS_RTOS_V2/cmsis_os2.c
	${RTOS_DIR}/portable/GCC/ARM_CM4F/port.c
	${RTOS_DIR}/portable/MemMang/heap_4.c
	${RTOS_DIR}/croutine.c
	${RTOS_DIR}/event_groups.c
	${RTOS_DIR}/list.c
	${RTOS_DIR}/queue.c
	${RTOS_DIR}/stream_buffer.c
	${RTOS_DIR}/tasks.c
	${RTOS_DIR}/timers.c
PARENT_SCOPE)

set (BOARD_INCLUDES
	${F4_DIR}/Core/Inc
	${F4_DIR}/Drivers/STM32F4xx_HAL_Driver/Inc
	${F4_DIR}/Drivers/CMSIS/Include
	${F4_DIR}/Drivers/CMSIS/Device/ST/STM32F4xx/Include

	${RTOS_DIR}/include
	${RTOS_DIR}/CMSIS_RTOS_V2
	${RTOS_DIR}/portable/GCC/ARM_CM4F
PARENT_SCOPE)

set (BOARD_DEFINES
	STM32F446xx
	USE_HAL_DRIVER
	BOARD_F4
	BOARD_CONTROLLER
	OS_FREERTOS
PARENT_SCOPE)

set (SVD_FILE
	${CMAKE_SOURCE_DIR}/Board/SVD/F4/STM32F446.svd
PARENT_SCOPE)
