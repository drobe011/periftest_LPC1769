#ifndef SIMPLETESTGPIO_H
#define SIMPLETESTGPIO_H

#include "board.h"
#include "mDT.h"
#include <stdlib.h>
#include <string.h>
#include "bitband.h"

#define RTC_OSCF 4
#define FORCE_RTC_SET 0
#define FIOSET_OFFSET 0x18
#define FIOPIN_OFFSET 0x14
#define FIOCLR_OFFSET 0x1C
#define FIO0SET_ADDRESS LPC_GPIO0_BASE + FIOSET_OFFSET // 0x2009C018 //C01A  // FIO0SET2
#define FIO0GET_ADDRESS LPC_GPIO0_BASE + FIOPIN_OFFSET
#define FIO0CLR_ADDRESS LPC_GPIO0_BASE + FIOCLR_OFFSET
#define LED_PIN 22 //6 //pin22
#define KHZ(x) x * 100

volatile int ticker = 0;
volatile int cap = 0;
volatile int updatePWMTimer = 0;
volatile int duty = 1;
volatile int flashDir = 1;

enum {FIRST_NAME, LAST_NAME};
enum {LOC, B_LENGTH};

uint8_t EEPROM_Data[2][2] = {{0,4},{5,9}};
		
void setupTimer(void);
void setupCounter(void);
void setupPWM(uint32_t period, uint32_t onTime);
void setupRTC(RTC_TIME_T *thisTime);
void setupEEPROM(I2C_XFER_T *xfr);
void setupRITimer(void);
void showTime(RTC_TIME_T *pTime);
void getProm(I2C_XFER_T *xfr, uint8_t whatname);

#endif