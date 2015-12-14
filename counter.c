#include "counter.h"
#include "board.h"
#include "bitband.h"

volatile int cap = 0;

void setupCounter(void)
{
	Chip_IOCON_PinMux(LPC_IOCON, 1, 18, 2, 3);
	Chip_GPIO_SetPinDIRInput(LPC_GPIO, 1, 18);

	Chip_TIMER_Init(LPC_TIMER1);
	Chip_TIMER_Reset(LPC_TIMER1);
	Chip_TIMER_CaptureRisingEdgeEnable(LPC_TIMER1, 0);
	Chip_TIMER_CaptureEnableInt(LPC_TIMER1, 0);
	Chip_TIMER_Enable(LPC_TIMER1);
	NVIC_ClearPendingIRQ(TIMER1_IRQn);
	NVIC_EnableIRQ(TIMER1_IRQn);
}

void TIMER1_IRQHandler(void)
{
	if (Chip_TIMER_CapturePending(LPC_TIMER1, 0))
	{
		Chip_TIMER_ClearCapture(LPC_TIMER1, 0);
		BITBAND_SRAM_GetBit(FIO0GET_ADDRESS, LED_PIN) ? BITBAND_SRAM_SetBit(FIO0CLR_ADDRESS, LED_PIN) : BITBAND_SRAM_SetBit(FIO0SET_ADDRESS, LED_PIN);
		cap++;
	}
}