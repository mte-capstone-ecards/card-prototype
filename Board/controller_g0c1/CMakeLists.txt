# G0C1 CMake Files

message(STATUS "Building G0C1 Directory")

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
set(G0C1_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(RTOS_DIR ${G0C1_DIR}/Middlewares/Third_Party/FreeRTOS/Source)

set (LINKER_SCRIPT
	${G0C1_DIR}/STM32G0C1RETX_FLASH.ld
PARENT_SCOPE)

set (BOARD_SOURCES
    ${G0C1_DIR}/Core/Src/adc.c
    ${G0C1_DIR}/Core/Src/gpio.c
    ${G0C1_DIR}/Core/Src/i2c.c
    ${G0C1_DIR}/Core/Src/main.c
    ${G0C1_DIR}/Core/Src/spi.c
    ${G0C1_DIR}/Core/Src/stm32g0xx_hal_msp.c
    ${G0C1_DIR}/Core/Src/stm32g0xx_hal_timebase_tim.c
    ${G0C1_DIR}/Core/Src/stm32g0xx_it.c
    ${G0C1_DIR}/Core/Src/syscalls.c
    ${G0C1_DIR}/Core/Src/sysmem.c
    ${G0C1_DIR}/Core/Src/system_stm32g0xx.c
    ${G0C1_DIR}/Core/Src/tim.c
    ${G0C1_DIR}/Core/Src/usart.c

    ${G0C1_DIR}/Drivers/STM32G0xx_HAL_Driver/Src/stm32g0xx_hal.c
    ${G0C1_DIR}/Drivers/STM32G0xx_HAL_Driver/Src/stm32g0xx_hal_adc.c
    ${G0C1_DIR}/Drivers/STM32G0xx_HAL_Driver/Src/stm32g0xx_hal_adc_ex.c
    ${G0C1_DIR}/Drivers/STM32G0xx_HAL_Driver/Src/stm32g0xx_hal_cortex.c
    ${G0C1_DIR}/Drivers/STM32G0xx_HAL_Driver/Src/stm32g0xx_hal_dma.c
    ${G0C1_DIR}/Drivers/STM32G0xx_HAL_Driver/Src/stm32g0xx_hal_dma_ex.c
    ${G0C1_DIR}/Drivers/STM32G0xx_HAL_Driver/Src/stm32g0xx_hal_exti.c
    ${G0C1_DIR}/Drivers/STM32G0xx_HAL_Driver/Src/stm32g0xx_hal_flash.c
    ${G0C1_DIR}/Drivers/STM32G0xx_HAL_Driver/Src/stm32g0xx_hal_flash_ex.c
    ${G0C1_DIR}/Drivers/STM32G0xx_HAL_Driver/Src/stm32g0xx_hal_gpio.c
    ${G0C1_DIR}/Drivers/STM32G0xx_HAL_Driver/Src/stm32g0xx_hal_i2c.c
    ${G0C1_DIR}/Drivers/STM32G0xx_HAL_Driver/Src/stm32g0xx_hal_i2c_ex.c
    ${G0C1_DIR}/Drivers/STM32G0xx_HAL_Driver/Src/stm32g0xx_hal_pwr.c
    ${G0C1_DIR}/Drivers/STM32G0xx_HAL_Driver/Src/stm32g0xx_hal_pwr_ex.c
    ${G0C1_DIR}/Drivers/STM32G0xx_HAL_Driver/Src/stm32g0xx_hal_rcc.c
    ${G0C1_DIR}/Drivers/STM32G0xx_HAL_Driver/Src/stm32g0xx_hal_rcc_ex.c
    ${G0C1_DIR}/Drivers/STM32G0xx_HAL_Driver/Src/stm32g0xx_hal_spi.c
    ${G0C1_DIR}/Drivers/STM32G0xx_HAL_Driver/Src/stm32g0xx_hal_spi_ex.c
    ${G0C1_DIR}/Drivers/STM32G0xx_HAL_Driver/Src/stm32g0xx_hal_tim.c
    ${G0C1_DIR}/Drivers/STM32G0xx_HAL_Driver/Src/stm32g0xx_hal_tim_ex.c
    ${G0C1_DIR}/Drivers/STM32G0xx_HAL_Driver/Src/stm32g0xx_hal_uart.c
    ${G0C1_DIR}/Drivers/STM32G0xx_HAL_Driver/Src/stm32g0xx_hal_uart_ex.c
    ${G0C1_DIR}/Drivers/STM32G0xx_HAL_Driver/Src/stm32g0xx_ll_adc.c
    ${G0C1_DIR}/Drivers/STM32G0xx_HAL_Driver/Src/stm32g0xx_ll_rcc.c

    ${G0C1_DIR}/Core/Startup/startup_stm32g0c1retx.s

    ${RTOS_DIR}/portable/MemMang/heap_4.c
    ${RTOS_DIR}/portable/GCC/ARM_CM0/port.c
    ${RTOS_DIR}/CMSIS_RTOS_V2/cmsis_os2.c
    ${RTOS_DIR}/croutine.c
    ${RTOS_DIR}/event_groups.c
    ${RTOS_DIR}/list.c
    ${RTOS_DIR}/queue.c
    ${RTOS_DIR}/stream_buffer.c
    ${RTOS_DIR}/tasks.c
    ${RTOS_DIR}/timers.c
PARENT_SCOPE)

set (BOARD_INCLUDES
    ${G0C1_DIR}/Core/Inc
    ${G0C1_DIR}/Drivers/STM32G0xx_HAL_Driver/Inc
    ${G0C1_DIR}/Drivers/STM32G0xx_HAL_Driver/Inc/Legacy
    ${G0C1_DIR}/Drivers/CMSIS/Device/ST/STM32G0xx/Include
    ${G0C1_DIR}/Drivers/CMSIS/Include
    ${RTOS_DIR}/include
    ${RTOS_DIR}/CMSIS_RTOS_V2
    ${RTOS_DIR}/portable/GCC/ARM_CM0
PARENT_SCOPE)

set (BOARD_DEFINES
    USE_HAL_DRIVER
    STM32G0C1xx
    CMSIS_device_header=<stm32g0xx.h>

	BOARD_G0
	BOARD_CONTROLLER
	OS_FREERTOS
PARENT_SCOPE)

set (SVD_FILE
	${CMAKE_SOURCE_DIR}/Board/SVD/G0/STM32G0C1.svd
PARENT_SCOPE)
