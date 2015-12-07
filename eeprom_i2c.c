#include "board.h"
#include "eeprom_i2c.h"

static uint8_t EEPROM_Data[2][2] = {{0,4},{5,9}};

void setupEEPROM(I2C_XFER_T *xfr)
{
	//xfr->rxSz = 4;
	//xfr->txSz = ;
	xfr->slaveAddr = 0b1010000;
	xfr->slaveAddr &= 0xFF;
	//uint8_t cmd[] = "xxRobertson";
	//cmd[0] = 0;
	//cmd[1] = 5;

	Chip_I2C_Init(I2C1);
	Chip_I2C_SetClockRate(I2C1, 100000);
	Chip_I2C_SetMasterEventHandler(I2C1, Chip_I2C_EventHandlerPolling);

	//Chip_I2C_MasterSend(I2C1, xfr->slaveAddr, cmd, 11);
}

void getProm(I2C_XFER_T *xfr, uint8_t whatname)
{
	uint8_t R_SIZE = EEPROM_Data[whatname][B_LENGTH];
	uint8_t rxbuffer[R_SIZE + 1];
	uint8_t txbuffer[] = {0, EEPROM_Data[whatname][LOC]};
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