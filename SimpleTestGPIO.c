/*
 ===============================================================================
 Name        : SimpleTestGPIO.c
 Author      : $(author)
 Version     :
 Copyright   : $(copyright)
 Description : main definition
 ===============================================================================
 */

#include "SimpleTestGPIO.h"
//#undef DEBUG_ENABLE

#ifdef DEBUG_ENABLE
#include <cross_studio_io.h>
#endif
	
int main(void)
{
	SystemCoreClockUpdate();
	Board_Init();
	Board_I2C_Init(I2C1);
	SysTick_Config(SystemCoreClock / 1000);

	RTC_TIME_T FullTime;
	I2C_XFER_T xfer;
	uint8_t rcvchar = EOF;

	setupTimer();
	setupCounter();
	setupRTC(&FullTime);
	setupEEPROM(&xfer);
	setupPWM(KHZ(1), duty);
	setupRITimer();

	#ifdef DEBUG_ENABLE
	debug_printf("\n\n\r..Ready..\n\r");
	#endif
	
	while (1)
	{
		#ifdef DEBUG_ENABLE
		rcvchar = debug_getch();
		if (rcvchar != EOF)
		{
			switch (rcvchar)
			{
				case 't':
					showTime(&FullTime);
					break;
				case 'c':
					debug_printf("\n\rCaps: %d\n\r", cap);
					break;
				case 'e':
					getProm(&xfer);
					break;
			}
		}
		#endif
	}
}

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

void setupPWM(uint32_t period, uint32_t onTime)
{
	Chip_IOCON_PinMux(LPC_IOCON, 1, 20, 2, 2); //pwm1.2
	Chip_GPIO_SetPinDIROutput(LPC_GPIO, 1, 20);

	Chip_PWM_Init(LPC_PWM1);
	Chip_PWM_Reset(LPC_PWM1);
	Chip_PWM_PrescaleSet(LPC_PWM1, 299); // PCLK (Systemcoreclock/4) / 299 = 100kHz
	Chip_PWM_ResetOnMatchEnable(LPC_PWM1, 0);
	Chip_PWM_SetMatch(LPC_PWM1, 0, period); // PR / period (100) = 1kHz
	Chip_PWM_SetMatch(LPC_PWM1, 2, onTime);
	Chip_PWM_LatchEnable(LPC_PWM1, 0, 1);
	Chip_PWM_LatchEnable(LPC_PWM1, 2, 1);
	Chip_PWM_SetControlMode(LPC_PWM1, 2, PWM_SINGLE_EDGE_CONTROL_MODE, PWM_OUT_ENABLED);
	Chip_PWM_Enable(LPC_PWM1);
}

void setupRTC(RTC_TIME_T *thisTime)
{
	if ((LPC_RTC->RTC_AUX & _BIT(RTC_OSCF)) || (FORCE_RTC_SET == 1))
	{
		thisTime->time[RTC_TIMETYPE_SECOND] = (uint32_t)mS;
		thisTime->time[RTC_TIMETYPE_MINUTE] = (uint32_t)mMN;
		thisTime->time[RTC_TIMETYPE_HOUR] = (uint32_t)mHR;
		thisTime->time[RTC_TIMETYPE_DAYOFMONTH] = (uint32_t)mDY;
		thisTime->time[RTC_TIMETYPE_DAYOFWEEK] = (uint32_t)mWD;
		thisTime->time[RTC_TIMETYPE_DAYOFYEAR] = (uint32_t)mDOY;
		thisTime->time[RTC_TIMETYPE_MONTH] = (uint32_t)mMO;
		thisTime->time[RTC_TIMETYPE_YEAR] = (uint32_t)mYR;

		Chip_RTC_SetFullTime(LPC_RTC, thisTime);
		LPC_RTC->RTC_AUX |= _BIT(RTC_OSCF);
		Chip_RTC_Enable(LPC_RTC, 1);
	}
}

void setupEEPROM(I2C_XFER_T *xfr)
{
	xfr->rxSz = 4;
	xfr->txSz = 6;
	xfr->slaveAddr = 0b1010000;
	xfr->slaveAddr &= 0xFF;

	Chip_I2C_Init(I2C1);
	Chip_I2C_SetClockRate(I2C1, 100000);
	Chip_I2C_SetMasterEventHandler(I2C1, Chip_I2C_EventHandlerPolling);

	//Chip_I2C_MasterSend(I2C1, xfer.slaveAddr, "dave", 4);
}

void setupRITimer(void)
{
	Chip_RIT_Init(LPC_RITIMER);
	Chip_RIT_SetTimerInterval(LPC_RITIMER, 10);
	Chip_RIT_Enable(LPC_RITIMER);
	NVIC_ClearPendingIRQ(RITIMER_IRQn);
	NVIC_EnableIRQ(RITIMER_IRQn);
}

void SysTick_Handler(void)
{
	ticker++;
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

//void TIMER0_IRQHandler(void)
//{
//	if (Chip_TIMER_MatchPending(LPC_TIMER0, 0))
//	{
//		Chip_TIMER_ClearMatch(LPC_TIMER0, 0);
//	}
//}

void TIMER1_IRQHandler(void)
{
	if (Chip_TIMER_CapturePending(LPC_TIMER1, 0))
	{
		Chip_TIMER_ClearCapture(LPC_TIMER1, 0);
		BITBAND_SRAM_GetBit(FIO0GET_ADDRESS, LED_PIN) ? BITBAND_SRAM_SetBit(FIO0CLR_ADDRESS, LED_PIN) : BITBAND_SRAM_SetBit(FIO0SET_ADDRESS, LED_PIN);
		cap++;
	}
}

void showTime(RTC_TIME_T *pTime)
{
	NVIC_ClearPendingIRQ(RITIMER_IRQn);
	NVIC_DisableIRQ(RITIMER_IRQn);
	Chip_RTC_GetFullTime(LPC_RTC, pTime);
	#ifdef DEBUG_ENABLE
	debug_printf("\r\nTime: %.2d:%.2d:%.2d",
		   pTime->time[RTC_TIMETYPE_HOUR], pTime->time[RTC_TIMETYPE_MINUTE],
		   pTime->time[RTC_TIMETYPE_SECOND]);
	debug_printf(" %.2d/%.2d/%.4d\r\n", pTime->time[RTC_TIMETYPE_MONTH],
		   pTime->time[RTC_TIMETYPE_DAYOFMONTH],
		   pTime->time[RTC_TIMETYPE_YEAR]);
	#endif
	NVIC_ClearPendingIRQ(RITIMER_IRQn);
	NVIC_EnableIRQ(RITIMER_IRQn);
}

void getProm(I2C_XFER_T *xfr)
{
	const uint8_t R_SIZE = 4;
	uint8_t rxbuffer[5];
	uint8_t txbuffer[] = {0, 0};
	xfr->rxSz = R_SIZE;
	xfr->txSz = 2;
	xfr->rxBuff = rxbuffer;
	xfr->txBuff = txbuffer;
	
	#ifdef DEBUG_ENABLE
	debug_printf("\n\r%d:", Chip_I2C_MasterTransfer(I2C1, xfr));
	rxbuffer[R_SIZE] = '\0';

	debug_printf(" EEPROM: %s\n\r", rxbuffer);
	#endif
}

//int __putchar(int ch)
//{
//	while ((LPC_UART3->LSR & UART_LSR_THRE) == 0) {}
//	LPC_UART3->THR = ch;
//	//Board_UARTPutChar(ch);
//	return (1);
//	//debug_putchar(ch);
//}