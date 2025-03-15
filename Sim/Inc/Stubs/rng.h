#pragma once

#include <stdlib.h>
#include <stdbool.h>

#define HAL_OK true
#define HAL_RNG_GenerateRandomNumber(a, b)  (*b = rand(), true)
