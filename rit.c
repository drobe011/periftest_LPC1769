#include "board.h"
#include "rit.h"

void setupRITimer(void)
{
	Chip_RIT_Init(LPC_RITIMER);
	Chip_RIT_SetTimerInterval(LPC_RITIMER, 20);
	Chip_RIT_TimerDebugEnable(LPC_RITIMER);
	Chip_RIT_Enable(LPC_RITIMER);
	NVIC_ClearPendingIRQ(RITIMER_IRQn);
	NVIC_EnableIRQ(RITIMER_IRQn);
}

void RIT_IRQHandler(void)
{
	switch (flashDir)
	{
		case 0:
			if (duty > 1) duty--;
			if (duty == 1) flashDir = 1;
			break;
		case 1:
			if (duty < 100) duty++;
			if (duty == 100) flashDir = 0;
	}
	Chip_PWM_SetMatch(LPC_PWM1, 2, duty);
	Chip_PWM_LatchEnable(LPC_PWM1, 0, 1);
	Chip_PWM_LatchEnable(LPC_PWM1, 2, 1);
	Chip_RIT_ClearInt(LPC_RITIMER);
}