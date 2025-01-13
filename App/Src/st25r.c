
#include "st25r.h"

#include <spi.h>

#include <stdbool.h>
#include <stdint.h>


/*****************************************/
/*                Defines                */
/*****************************************/
#if defined(BOARD_F4)
# define ST25R_SPI  hspi1
#else
# error "No ST25R SPI defined"
#endif

#define MODE_MASK(val)  (val << 6U)
#define WRITE_MODE      MODE_MASK(0b00)
#define READ_MODE       MODE_MASK(0b01)
#define FIFO_MODE       MODE_MASK(0b10)
#define CMD_MODE        MODE_MASK(0b11)

/*****************************************/
/*           Private Functions           */
/*****************************************/

static void ST25R_writeBytes(uint8_t addr, uint8_t *data, uint8_t len)
{
    HAL_GPIO_WritePin(ST25_CS_GPIO_Port, ST25_CS_Pin, GPIO_PIN_RESET);

    // Write pattern
    uint8_t pattern = WRITE_MODE | addr;
    HAL_SPI_Transmit(&ST25R_SPI, &pattern, 1U, HAL_MAX_DELAY);

    // Write the data
    HAL_SPI_Transmit(&ST25R_SPI, data, len, HAL_MAX_DELAY);

    HAL_GPIO_WritePin(ST25_CS_GPIO_Port, ST25_CS_Pin, GPIO_PIN_SET);
}

static uint8_t ST25R_readRegister(uint8_t addr)
{
    HAL_GPIO_WritePin(ST25_CS_GPIO_Port, ST25_CS_Pin, GPIO_PIN_RESET);

    // Write pattern
    uint8_t pattern = READ_MODE | addr;
    HAL_SPI_Transmit(&ST25R_SPI, &pattern, 1U, HAL_MAX_DELAY);

    // Read the data
    uint8_t data;
    HAL_SPI_Receive(&ST25R_SPI, &data, 1U, HAL_MAX_DELAY);

    HAL_GPIO_WritePin(ST25_CS_GPIO_Port, ST25_CS_Pin, GPIO_PIN_SET);

    return data;
}

static void ST25R_directCommand(uint8_t cmd, bool interrupt)
{
    HAL_GPIO_WritePin(ST25_CS_GPIO_Port, ST25_CS_Pin, GPIO_PIN_RESET);

    // Write pattern
    uint8_t pattern = CMD_MODE | cmd;
    HAL_SPI_Transmit(&ST25R_SPI, &pattern, 1U, HAL_MAX_DELAY);

    HAL_GPIO_WritePin(ST25_CS_GPIO_Port, ST25_CS_Pin, GPIO_PIN_SET);

    if (interrupt)
    {
        // Wait for the IRQ line
        while( HAL_GPIO_ReadPin(ST25_INTR_GPIO_Port, ST25_INTR_Pin) == GPIO_PIN_RESET );

        // We should clear the IRQ reason for direct command
        // Direct Command IRQ: Reg 0x18, Bit 7

    }
}

static void ST25R_powerUpSequence(void)
{
    // 1. Configure IO regs

    // 2. Configure Regulators

    // 3. Calibrate Antenna

    // 4. Calibrate modulation depth (If needed)

    // 5. We are ready to operate!
}

/*****************************************/
/*           Public Functions            */
/*****************************************/

void st25r_main()
{
    HAL_Delay(1000);

    ST25R_powerUpSequence();

	while (1)
	{
		HAL_GPIO_TogglePin(USER_LED_GPIO_Port, USER_LED_Pin);
		HAL_Delay(1000);
	}
}

