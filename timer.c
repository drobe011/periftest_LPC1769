//#include "timer.h"
#include "board.h"

void setupTimer(void)
{
	Chip_IOCON_PinMux(LPC_IOCON, 1, 28, 2, 3);
	Chip_GPIO_SetPinDIROutput(LPC_GPIO, 1, 28);

	Chip_TIMER_Init(LPC_TIMER0);
	Chip_TIMER_Reset(LPC_TIMER0);
	Chip_TIMER_PrescaleSet(LPC_TIMER0, 500);
	Chip_TIMER_SetMatch(LPC_TIMER0, 0, 16000);
	Chip_TIMER_ResetOnMatchEnable(LPC_TIMER0, 0);
	//	Chip_TIMER_MatchEnableInt(LPC_TIMER0, 0);
	Chip_TIMER_ExtMatchControlSet(LPC_TIMER0, RESET, TIMER_EXTMATCH_TOGGLE, 0);
	Chip_TIMER_Enable(LPC_TIMER0);
	//	NVIC_ClearPendingIRQ(TIMER0_IRQn);
	//	NVIC_EnableIRQ(TIMER0_IRQn);
}

//void TIMER0_IRQHandler(void)
//{
//	if (Chip_TIMER_MatchPending(LPC_TIMER0, 0))
//	{
//		Chip_TIMER_ClearMatch(LPC_TIMER0, 0);
//	}
//}