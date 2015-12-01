/*
 ===============================================================================
 Name        : SimpleTestGPIO.c
 Author      : $(author)
 Version     :
 Copyright   : $(copyright)
 Description : main definition
 ===============================================================================
 */

#include "board.h"
//#include <cross_studio_io.h>
#include "mDT.h"
#include <stdlib.h>
#include <string.h>
#include "bitband.h"

#define FORCE_RTC_SET 0
#define FIOSET_OFFSET 0x18
#define FIOPIN_OFFSET 0x14
#define FIOCLR_OFFSET 0x1C
#define FIO0SET_ADDRESS LPC_GPIO0_BASE + FIOSET_OFFSET // 0x2009C018 //C01A  // FIO0SET2
#define FIO0GET_ADDRESS LPC_GPIO0_BASE + FIOPIN_OFFSET
#define FIO0CLR_ADDRESS LPC_GPIO0_BASE + FIOCLR_OFFSET
#define LED_PIN 22					   //6 //pin22

volatile int ticker = 0;
volatile int cap = 0;

void showTime(RTC_TIME_T *pTime);
void getProm();

static I2C_XFER_T xfer;
uint8_t rxbuffer[10];
uint8_t txbuffer[10];

int main(void)
{

	SystemCoreClockUpdate();
	Board_Init();
	Board_I2C_Init(I2C1);

	Chip_IOCON_PinMux(LPC_IOCON, 1, 28, 2, 3);
	Chip_GPIO_SetPinDIROutput(LPC_GPIO, 1, 28);

	Chip_IOCON_PinMux(LPC_IOCON, 1, 18, 2, 3);
	Chip_GPIO_SetPinDIRInput(LPC_GPIO, 1, 18);

	SysTick_Config(SystemCoreClock / 1000);

	Chip_TIMER_Init(LPC_TIMER0);
	Chip_TIMER_Reset(LPC_TIMER0);
	Chip_TIMER_PrescaleSet(LPC_TIMER0, 500);
	Chip_TIMER_SetMatch(LPC_TIMER0, 0, 16000);
	Chip_TIMER_ResetOnMatchEnable(LPC_TIMER0, 0);
	Chip_TIMER_MatchEnableInt(LPC_TIMER0, 0);
	LPC_TIMER0->EMR |= (1 << 4);
	LPC_TIMER0->EMR |= (1 << 5);
	Chip_TIMER_Enable(LPC_TIMER0);
	NVIC_ClearPendingIRQ(TIMER0_IRQn);
	NVIC_EnableIRQ(TIMER0_IRQn);

	Chip_TIMER_Init(LPC_TIMER1);
	Chip_TIMER_Reset(LPC_TIMER1);
	Chip_TIMER_CaptureRisingEdgeEnable(LPC_TIMER1, 0);
	Chip_TIMER_CaptureEnableInt(LPC_TIMER1, 0);
	Chip_TIMER_Enable(LPC_TIMER1);
	NVIC_ClearPendingIRQ(TIMER1_IRQn);
	NVIC_EnableIRQ(TIMER1_IRQn);

	xfer.rxBuff = rxbuffer;
	xfer.rxSz = 4;
	xfer.txBuff = txbuffer;
	xfer.txSz = 6;
	xfer.slaveAddr = 0b1010000;
	xfer.slaveAddr &= 0xFF;

	Chip_I2C_Init(I2C1);
	Chip_I2C_SetClockRate(I2C1, 100000);
	Chip_I2C_SetMasterEventHandler(I2C1, Chip_I2C_EventHandlerPolling);

	//Chip_I2C_MasterSend(I2C1, xfer.slaveAddr, "dave", 4);

	RTC_TIME_T FullTime;
	if ((LPC_RTC->RTC_AUX & (1 << 4)) || (FORCE_RTC_SET == 1))
	{
		FullTime.time[RTC_TIMETYPE_SECOND] = (uint32_t)mS;
		FullTime.time[RTC_TIMETYPE_MINUTE] = (uint32_t)mMN;
		FullTime.time[RTC_TIMETYPE_HOUR] = (uint32_t)mHR;
		FullTime.time[RTC_TIMETYPE_DAYOFMONTH] = (uint32_t)mDY;
		FullTime.time[RTC_TIMETYPE_DAYOFWEEK] = (uint32_t)mWD;
		FullTime.time[RTC_TIMETYPE_DAYOFYEAR] = (uint32_t)mDOY;
		FullTime.time[RTC_TIMETYPE_MONTH] = (uint32_t)mMO;
		FullTime.time[RTC_TIMETYPE_YEAR] = (uint32_t)mYR;

		Chip_RTC_SetFullTime(LPC_RTC, &FullTime);
		LPC_RTC->RTC_AUX |= (1 << 4);
		Chip_RTC_Enable(LPC_RTC, 1);
	}

	uint32_t tickertime = ticker;
	uint8_t rcvchar = EOF;
	//Chip_UART_SendBlocking(LPC_UART3, "\nReady\n", 7);
	printf("\n\n\rReady..\n\r");
	printf("Sizof: %d\n\r", INT32_MAX);
	// Enter an infinite loop

	while (1)
	{
		if (ticker > (tickertime + 100))
		{
			rcvchar = Board_UARTGetChar();
			if (rcvchar != EOF)
			{
				switch (rcvchar)
				{
					case 't':
						Chip_RTC_GetFullTime(LPC_RTC, &FullTime);
						showTime(&FullTime);
						break;
					case 'c':
						printf("\n\rCaps: %d\n\r", cap);
						break;
					case 'e':
						getProm();
				}
			}
			tickertime = ticker;
		}
	}
	return 0;
}

void SysTick_Handler(void)
{
	ticker++;
}

void TIMER0_IRQHandler(void)
{
	if (Chip_TIMER_MatchPending(LPC_TIMER0, 0))
	{
		Chip_TIMER_ClearMatch(LPC_TIMER0, 0);
	}
}

void TIMER1_IRQHandler(void)
{
	uint32_t bitVal = 0;
	if (Chip_TIMER_CapturePending(LPC_TIMER1, 0))
	{
		Chip_TIMER_ClearCapture(LPC_TIMER1, 0);
		LPC_TIMER0->EMR &= ~(1 << 0);
		//Board_LED_Toggle(0);
		BITBAND_SRAM_GetBit(FIO0GET_ADDRESS, LED_PIN) ? BITBAND_SRAM_SetBit(FIO0CLR_ADDRESS, LED_PIN) : BITBAND_SRAM_SetBit(FIO0SET_ADDRESS, LED_PIN);
		cap++;
	}
}

void showTime(RTC_TIME_T *pTime)
{
	printf("Time: %.2d:%.2d:%.2d %.2d/%.2d/%.4d\r\n",
		   pTime->time[RTC_TIMETYPE_HOUR], pTime->time[RTC_TIMETYPE_MINUTE],
		   pTime->time[RTC_TIMETYPE_SECOND], pTime->time[RTC_TIMETYPE_MONTH],
		   pTime->time[RTC_TIMETYPE_DAYOFMONTH],
		   pTime->time[RTC_TIMETYPE_YEAR]);
}

void getProm(void)
{
	xfer.rxSz = 4;
	xfer.txSz = 2;
	txbuffer[0] = 0;
	txbuffer[1] = 0;

	memset(rxbuffer, 0, sizeof rxbuffer);
	xfer.rxBuff = rxbuffer;

	Chip_I2C_MasterTransfer(I2C1, &xfer);

	printf("\n\rEEPROM: %s\n\r", rxbuffer);
}

int __putchar(int ch)
{
	Board_UARTPutChar(ch);
	return (1);
	//debug_putchar(ch);
}