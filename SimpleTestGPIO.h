/*!
 * \file SimpleTestGPIO.h
 *
 * \author David Robertson
 * \date 14 December, 2015
 *
 * [your comment here]
 */

#ifndef SIMPLETESTGPIO_H
#define SIMPLETESTGPIO_H

#include "board.h"
#include <stdlib.h>
#include <string.h>
//#include "timer.h"
//#include "counter.h"
#include "rtc.h"
#include "eeprom_i2c.h"
//#include "pwm.h"
//#include "rit.h"
#include "keypad.h"

/** Defines polling delay for UART_Getchar loop.*/
#define LOOP_DELAY_MS 100
/** Global variable to hold Systick counter.*/
volatile int ticker = 0;
/** Two-dimensional array to hold keypad map values.*/
const uint8_t keypad[5][5] = {{'2','5','9','d','/'},
	{'3','6','-','+','*'},
		{'1','4','8','b','c'}, 
			{'0','a','7','%','='},
				{'^','.','0','0','|'}};
/** Define max size of receive buffer.*/
#define BUFFERSIZE 16
/** Array to hold received keypad data.*/
uint8_t keybuffer[BUFFERSIZE];

/*!
 * \brief Gets input from keypad
 *
 * Receives up to \a BUFFERSIZE -1 digits from keypad
 * or until \a ESC_char is received.  Passes \a pf for
 * callback (or set to \a NULL).
 *
 * \param[in] sz Max receive size.
 * \param[in] ESC_char Escape/Termination character.
 * \param[in] pf Function pointer to callback routine.
 * \return 0 if error/escape
 * 1 if successful
 */
uint8_t getKeypad(uint8_t sz, uint8_t ESC_char, uint32_t (*pf)(void));
/*!
 * \brief Callback function call from \a keypad routine.
 *
 * Function is called from getKPChar(uint8_t *kpchar, uint32_t (*pf)(void))
 * during state polling.
 *
 * \return 0 if error/escape
 * 1 if successful
 */
uint32_t callBackFx(void);

#endif