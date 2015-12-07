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
	uint32_t tickerStore = ticker;
	extern volatile int duty;
	extern volatile int cap;

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
		if (ticker > (tickerStore + LOOP_DELAY_MS))
		{
			#ifdef DEBUG_ENABLE
			if (debug_kbhit())
			{
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
						case 'f':
							getProm(&xfer, FIRST_NAME);
							break;
						case 'l':
							getProm(&xfer, LAST_NAME);
							break;
					}
				}
				#endif
			}
		}
	}
}

void SysTick_Handler(void)
{
	ticker++;
}











//int __putchar(int ch)
//{
//	while ((LPC_UART3->LSR & UART_LSR_THRE) == 0) {}
//	LPC_UART3->THR = ch;
//	//Board_UARTPutChar(ch);
//	return (1);
//	//debug_putchar(ch);
//}