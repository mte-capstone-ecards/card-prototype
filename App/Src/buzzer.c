
#include "buzzer.h"

#include <tim.h>
#include <cmsis_os.h>

#define PIEZZO_TIM	&htim3
#define PIEZZO_CH	TIM_CHANNEL_3

typedef enum
{
	NOTE_C,
	NOTE_D,
	NOTE_E,
	NOTE_F,
	NOTE_G,
	NOTE_A,
	NOTE_B,
	NOTE_COUNT,
} Note;

uint16_t freqs[NOTE_COUNT] = {
	[NOTE_C] = 61555,
	[NOTE_D] = 54485,
	[NOTE_E] = 48539,
	[NOTE_F] = 45815,
	[NOTE_G] = 40816,
	[NOTE_A] = 36364,
	[NOTE_B] = 43397,
};

static void Buzzer_play(Note note, uint8_t octavesUp)
{
	// PWM Clock at 64MHz

	// Stop PWM
	HAL_TIM_PWM_Stop(PIEZZO_TIM, PIEZZO_CH);

	// In Clock: 64MHz
	// Divider: 3
	// Out Clock: 16MHz

	// Set Prescaler
	const uint16_t prescaler = 3;
	__HAL_TIM_SET_PRESCALER(PIEZZO_TIM, prescaler);

	// Set Period
	const uint16_t period = freqs[note] >> octavesUp;
	__HAL_TIM_SET_AUTORELOAD(PIEZZO_TIM, period);

	// Set compare register
	const uint16_t cr3 = period / 2;
	__HAL_TIM_SET_COMPARE(PIEZZO_TIM, PIEZZO_CH, cr3);

	// Start PWM
	HAL_TIM_PWM_Start(PIEZZO_TIM, PIEZZO_CH);
}

static void Buzzer_stopPlaying(void)
{
	HAL_TIM_PWM_Stop(PIEZZO_TIM, PIEZZO_CH);
}

void Buzzer_playBad(void)
{
	Buzzer_play(NOTE_F, 0);
	osDelay(250);
	Buzzer_play(NOTE_E, 0);
	osDelay(500);
	Buzzer_stopPlaying();
}