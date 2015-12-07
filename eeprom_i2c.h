#ifndef EEPROM_I2C_H
#define EEPROM_I2C_H

enum {FIRST_NAME, LAST_NAME};
enum {LOC, B_LENGTH};



void setupEEPROM(I2C_XFER_T *xfr);
void getProm(I2C_XFER_T *xfr, uint8_t whatname);

#endif