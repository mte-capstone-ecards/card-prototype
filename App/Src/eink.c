
#include "eink.h"

#include <main.h>
#include <spi.h>

#include <stdbool.h>
#include <string.h>

typedef struct {
    bool input;
    GPIO_PinState initState;
    GPIO_PinState activeState;
    GPIO_TypeDef *HAL_port;
    uint16_t HAL_pin;
} Pin_s;

typedef enum {
    PIN_BUSY,
    PIN_DC,
    PIN_RESET,
    PIN_CS,

    PIN_COUNT
} Pin_e;

Pin_s Pins[PIN_COUNT] = {
    [PIN_BUSY]  = { .input = true,                               .activeState = GPIO_PIN_RESET, .HAL_port = EINK_BUSY_GPIO_Port,   .HAL_pin = EINK_BUSY_Pin   },
    [PIN_DC]    = { .input = false, .initState = GPIO_PIN_SET,   .activeState = GPIO_PIN_SET  , .HAL_port = EINK_DC_GPIO_Port,     .HAL_pin = EINK_DC_Pin     },
    [PIN_RESET] = { .input = false, .initState = GPIO_PIN_SET,   .activeState = GPIO_PIN_RESET, .HAL_port = EINK_RESET_GPIO_Port,  .HAL_pin = EINK_RESET_Pin  },
    [PIN_CS]    = { .input = false, .initState = GPIO_PIN_SET,   .activeState = GPIO_PIN_RESET, .HAL_port = EINK_SPI_CS_GPIO_Port, .HAL_pin = EINK_SPI_CS_Pin },
};

/*****************************************/
/*             HAL Functions             */
/*****************************************/
void eink_pinAssert(Pin_e pinNum)
{
    HAL_GPIO_WritePin(Pins[pinNum].HAL_port, Pins[pinNum].HAL_pin, Pins[pinNum].activeState);
}

void eink_pinDeassert(Pin_e pinNum)
{
    if (Pins[pinNum].activeState == GPIO_PIN_SET)
        HAL_GPIO_WritePin(Pins[pinNum].HAL_port, Pins[pinNum].HAL_pin, GPIO_PIN_RESET);
    else
        HAL_GPIO_WritePin(Pins[pinNum].HAL_port, Pins[pinNum].HAL_pin, GPIO_PIN_SET);
}

bool eink_pinRead(Pin_e pinNum)
{
    return HAL_GPIO_ReadPin(Pins[pinNum].HAL_port, Pins[pinNum].HAL_pin) == Pins[pinNum].activeState;
}

void eink_pinInit(void)
{
    for (uint8_t pinNum = 0; pinNum < PIN_COUNT; pinNum++)
    {
        Pin_s *pin = &Pins[pinNum];
        if (pin->input == false)
        {
            HAL_GPIO_WritePin(pin->HAL_port, pin->HAL_pin, pin->initState);
        }
    }
}

/*****************************************/
/*         EInk Driver Functions         */
/*****************************************/

void eink_sendIndexData(uint8_t index, uint8_t *data, uint16_t len)
{
    eink_pinDeassert(PIN_DC);
    eink_pinAssert(PIN_CS);

    HAL_SPI_Transmit(&hspi2, &index, 1U, HAL_MAX_DELAY);

    eink_pinDeassert(PIN_CS);
    eink_pinAssert(PIN_DC);
    eink_pinDeassert(PIN_CS);

    for (uint16_t i = 0; i < len; i++)
    {
        HAL_SPI_Transmit(&hspi2, &data[i], 1U, HAL_MAX_DELAY);
    }

    eink_pinAssert(PIN_CS);
}

void eink_softReset()
{
    uint8_t softReset = 0x0e;
    eink_sendIndexData(0x00, &softReset, 1);
    while (eink_pinRead(PIN_BUSY));
}

void eink_displayRefresh()
{
    uint8_t displayRefresh = {0x00};
    eink_sendIndexData(0x12, &displayRefresh, 1U);
    while (eink_pinRead(PIN_BUSY));
}

void eink_reset()
{
    const uint8_t delays[] = {1, 5, 10, 5, 1};

    HAL_Delay(delays[0]);
    eink_pinDeassert(PIN_RESET);
    HAL_Delay(delays[1]);
    eink_pinAssert(PIN_RESET);
    HAL_Delay(delays[2]);
    eink_pinDeassert(PIN_RESET);
    HAL_Delay(delays[3]);
    eink_pinDeassert(PIN_CS);
    HAL_Delay(delays[4]);
}

void eink_DCDCpowerOn()
{
    eink_sendIndexData(0x04, 0x00, 1U);
    while (eink_pinRead(PIN_BUSY));
}

/*****************************************/
/*           EInk App Funtions           */
/*****************************************/

typedef struct {
    uint16_t screenSizeV;
    uint16_t screenSizeH;
    uint16_t screenDiagonal;
    uint16_t refreshTime;
} EINK_s;


// 370 Display
#define EINK_SCREEN_SIZE_V 416
#define EINK_SCREEN_SIZE_H 240

#define EINK_IMAGE_DATA_SIZE (((uint32_t) (EINK_SCREEN_SIZE_V)) * ((uint32_t) (EINK_SCREEN_SIZE_H / 8)))

void eink_powerUp()
{
    eink_pinInit();

    HAL_Delay(5);

    eink_reset();
    eink_softReset();

    uint8_t initData[] = {0x19, 0x02, 0xcf, 0x8d};
    eink_sendIndexData(0xe5, &initData[0], 1); // Input temperature 25degC
    eink_sendIndexData(0xe0, &initData[1], 1); // Active temperature
    eink_sendIndexData(0x00, &initData[2], 2); // PSR0, PSR1
}

void eink_powerDown()
{
    // TODO:
}

void eink_globalUpdate(uint8_t *dataBW, uint8_t *dataBWR)
{
    // Send first frame
    eink_sendIndexData(0x10, dataBW, EINK_IMAGE_DATA_SIZE);

    // Send second frame
    eink_sendIndexData(0x13, dataBWR, EINK_IMAGE_DATA_SIZE);

    eink_DCDCpowerOn();
    eink_displayRefresh();

}

void eink_main()
{

    // Turn on the display
    eink_powerUp();

    // Create display data
    uint8_t dataBW[EINK_IMAGE_DATA_SIZE] = { 0U };
    uint8_t dataBWR[EINK_IMAGE_DATA_SIZE] = { 0U };

    memset(dataBW,  0x00, EINK_IMAGE_DATA_SIZE);
    memset(dataBWR, 0x00, EINK_IMAGE_DATA_SIZE);

    eink_globalUpdate(dataBW, dataBWR);

    while(1)
    {
        HAL_Delay(500);
        HAL_GPIO_TogglePin(USER_LED_GPIO_Port, USER_LED_Pin);
    }
}
