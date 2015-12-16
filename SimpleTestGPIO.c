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

//#ifdef DEBUG_ENABLE
//#include <cross_studio_io.h>
//#endif

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

//	setupTimer();
//	setupCounter();
	setupRTC(&FullTime);
	setupEEPROM(&xfer);
//	setupPWM(KHZ(1), duty);
//	setupRITimer();
	setupKeypad();

#ifdef DEBUG_ENABLE
	printf("\n\n\r..Ready..\n\r");
#endif

	while (1)
	{
		if (ticker > (tickerStore + LOOP_DELAY_MS))
		{
#ifdef DEBUG_ENABLE
				rcvchar = Board_UARTGetChar();
				if (rcvchar != EOF)
				{
					switch (rcvchar)
					{
						case 't':
							showTime(&FullTime);
							break;
						case 'f':
							getProm(&xfer, FIRST_NAME);
							break;
						case 'l':
							getProm(&xfer, LAST_NAME);
							break;
						case 'k':
							getKeypad(10, '=', callBackFx, dispKPChar);
							break;
					}
				}
#endif
		}
	}
}

uint32_t getKeypad(uint8_t sz, uint8_t ESC_char, uint32_t (*pf)(void), uint32_t (*display)(uint8_t, uint8_t))
{
	extern volatile uint8_t isNew;
	
	if (sz > BUFFERSIZE) return 0;
	memset(keybuffer, '\0', BUFFERSIZE);
	
	#ifdef DEBUG_ENABLE
	printf("\n\rInput\n\r");
	#endif
	
	uint8_t keyP[] = {0,0};
	uint8_t keyP_State[] = {0,0};
	uint8_t keyLoopCnt = 0;
	uint8_t kpVal = 0;
	
	for (keyLoopCnt = 0; keyLoopCnt < sz; keyLoopCnt++)
	{
		kpVal = getKPChar(keyP, pf, display);
		if (!kpVal) break;
		if (keypad[keyP[0]][keyP[1]] == ESC_char) break;
		keybuffer[keyLoopCnt] = keypad[keyP[0]][keyP[1]];
	}
	keybuffer[keyLoopCnt + 1] = '\0';
	if (strcmp(keybuffer, "6648"))
	{
		#ifdef DEBUG_ENABLE
		printf("\n\rNot Vallid %s\n\r", keybuffer);
		#endif
	}
	else
	{
		#ifdef DEBUG_ENABLE
		printf("\n\rCorrect %s\n\r", keybuffer);
		#endif
	}
	return 1;
}

uint32_t callBackFx(void)
{
	//Board_LED_Toggle(0);
	if (Board_UARTGetChar() == 'x')
	{
		#ifdef DEBUG_ENABLE
		printf("\n\rAbort%s\n\r");
		#endif
		return 0;
	}
	else return 1;
}

uint32_t dispKPChar(uint8_t row, uint8_t col)
{
	#ifdef DEBUG_ENABLE
	printf("%c", keypad[row][col]);
	#endif
	return 1;
}
void SysTick_Handler(void)
{
	ticker++;
}

int __putchar(int ch)
{
	while ((LPC_UART3->LSR & UART_LSR_THRE) == 0) {}
	LPC_UART3->THR = ch;
}