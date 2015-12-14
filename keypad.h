#ifndef KEYPAD_H
#define KEYPAD_H

#define POLL_TIME_MS 5
#define KP_GPIO_PORT 2
#define KP_NO_PUPD 2
#define KP_PULLDOWN 3
#define KP_GPIO_FUNC 0
#define KP_COLUMN_4 1
#define KP_COLUMN_3 2
#define KP_COLUMN_2 4
#define KP_COLUMN_1 8
#define KP_COLUMN_0 64 

#define GET_ROW_VALUES(x) (x >>= 5)

void setupKeypad(void);
uint16_t getKPChar(uint8_t *kpchar, uint32_t (*pf)(void));

#endif