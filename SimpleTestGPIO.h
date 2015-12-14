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

#define LOOP_DELAY_MS 100

volatile int ticker = 0;
const uint8_t keypad[5][5] = {{'2','5','9','d','/'},
	{'3','6','-','+','*'},
		{'1','4','8','b','c'}, 
			{'0','a','7','%','='},
				{'^','.','0','0','|'}};

#define BUFFERSIZE 16
uint8_t keybuffer[BUFFERSIZE];

uint8_t getKeypad(uint8_t sz, uint8_t ESC_char, uint32_t (*pf)(void));
uint32_t callBackFx(void);

#endif