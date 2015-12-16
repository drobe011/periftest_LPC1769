/*!
 * \file keypad.h
 *
 * \author David Robertson
 * \date 14 December, 2015
 *
 * Blocking keypad routine with callback.
 */
/** @defgroup Keypad Keypad Module
 *  Module for reading keypad matrix
 *  @{
 */
#ifndef KEYPAD_H
#define KEYPAD_H

/** Sets state poll time in mS.*/
#define POLL_TIME_MS 5
/** GPIO port that keypad is connected.*/
#define KP_GPIO_PORT 2
/** GPIO pin mode - No pullup or pulldown.*/
#define KP_NO_PUPD 2
/** GPIO pin mode - pulldown enabled.*/
#define KP_PULLDOWN 3
/** GPIO pin function - primary (GPIO).*/
#define KP_GPIO_FUNC 0
//!@{
/**  Maps pin bit locations.*/
#define KP_COLUMN_4 1
#define KP_COLUMN_3 2
#define KP_COLUMN_2 4
#define KP_COLUMN_1 8
#define KP_COLUMN_0 64 
//!@}
/*! \def GET_ROW_VALUES(x)
* \brief A macro that returns the \a KP_COLUMN_x pin states.
 *
 * Takes the masked port 2 value and shifts out row pin states.
 */
#define GET_ROW_VALUES(x) (x >>= 5)
/*!
 * \brief Initialize GPIO for keypad
 */
void setupKeypad(void);
/*!
 * \brief Gets input from keypad
 *
 * Gets single key press from keypad.
 *
 * \param[in] pf Function Pointer to callback routine.
 * \param[out] kpchar Stores keypress x,y value.
 * \return 0 if error/escape
 * 1 if successful
 */
uint16_t getKPChar(uint8_t *kpchar, uint32_t (*pf)(void));

#endif
/** @} */