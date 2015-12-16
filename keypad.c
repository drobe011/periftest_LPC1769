/*!
 * \file keypad.c
 *
 * \author David Robertson
 * \date 14 December, 2015
 *
 * [your comment here]
 */

#include "board.h"
#include "keypad.h"

/** Global variable to hold Systick counter.*/
extern volatile int ticker;
/** Global array to hold \a KP_rowPin physical pins.*/
static const uint32_t A_PINS[] = {11, 8, 7, 6, 5};

void setupKeypad(void)
{
	uint8_t pinCounter = 0;

	for (pinCounter = 0; pinCounter < 5; pinCounter++)
	{
		Chip_IOCON_PinMux(LPC_IOCON, KP_GPIO_PORT, pinCounter, KP_NO_PUPD, KP_GPIO_FUNC);
		Chip_GPIO_SetPinDIROutput(LPC_GPIO, KP_GPIO_PORT, pinCounter);
		Chip_GPIO_SetPinOutLow(LPC_GPIO, KP_GPIO_PORT, pinCounter);
		
		Chip_IOCON_PinMux(LPC_IOCON, KP_GPIO_PORT, A_PINS[pinCounter], KP_PULLDOWN, KP_GPIO_FUNC);
		Chip_GPIO_SetPinDIRInput(LPC_GPIO, KP_GPIO_PORT, A_PINS[pinCounter]);
	}
}

uint32_t getKPChar(uint8_t *kpchar, uint32_t (*pf)(void), uint32_t (*display)(uint8_t, uint8_t))
{
	const uint32_t GPIO_Port2Mask = 0b100111111111;
	const uint32_t GPIO_RowPinMask = 0b11111;
	uint32_t maskedPort = 0;
	uint32_t KP_columnValue = 0;
	uint32_t tickerState = ticker;
	uint8_t KP_rowPin = 0;
	uint8_t scan = ENABLE;
	uint32_t callbackReturnValue = 0;
	enum
	{
		CHANGE_HIGH_PIN,
		CHECK_FOR_PRESS,
		CALLBACK,
		WAIT_FOR_RELEASE,
		RELEASED
	};
	uint8_t state = CHANGE_HIGH_PIN;

	while (scan)
	{
		if (ticker > tickerState + POLL_TIME_MS)
		{
			switch (state)
			{
				case CHANGE_HIGH_PIN:
					Chip_GPIO_SetPinOutLow(LPC_GPIO, KP_GPIO_PORT, KP_rowPin);
					KP_rowPin = (KP_rowPin == 4) ? 0 : KP_rowPin + 1;
					Chip_GPIO_SetPinOutHigh(LPC_GPIO, KP_GPIO_PORT, KP_rowPin);
					state = CHECK_FOR_PRESS;
					break;
				case CHECK_FOR_PRESS:
					maskedPort = Chip_GPIO_GetPortValue(LPC_GPIO, KP_GPIO_PORT) & GPIO_Port2Mask;
					GET_ROW_VALUES(maskedPort);
					if (maskedPort)
					{
						switch (maskedPort)
						{
							case KP_COLUMN_4:
								KP_columnValue = 4;
								break;
							case KP_COLUMN_3:
								KP_columnValue = 3;
								break;
							case KP_COLUMN_2:
								KP_columnValue = 2;
								break;
							case KP_COLUMN_1:
								KP_columnValue = 1;
								break;
							case KP_COLUMN_0:
								KP_columnValue = 0;
								break;
							default:
								KP_columnValue = 255;
						}
						if (KP_columnValue != 255)
						{
							*kpchar = KP_rowPin;
							kpchar++;
							*kpchar = KP_columnValue;
							display(KP_rowPin, KP_columnValue);
							state = WAIT_FOR_RELEASE;
						}
					} //if maskedport
					else state = CALLBACK;
					break;
					case CALLBACK:
						callbackReturnValue = 1;
						if (pf) callbackReturnValue = pf();
						state = (callbackReturnValue) ? CHANGE_HIGH_PIN : RELEASED;
						break;
					case WAIT_FOR_RELEASE:
						maskedPort = Chip_GPIO_GetPortValue(LPC_GPIO, KP_GPIO_PORT) & GPIO_Port2Mask;
						GET_ROW_VALUES(maskedPort);
						if (!maskedPort) state = RELEASED;
						break;
					case RELEASED:
						Chip_GPIO_SetPortOutLow(LPC_GPIO, KP_GPIO_PORT, GPIO_RowPinMask);
						scan = DISABLE;
						break;
			} //switch state
			tickerState = ticker;
		} //switch ticker
	} //while scan
	if (!callbackReturnValue) return 0;
	return 1;
}
