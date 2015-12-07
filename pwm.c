#include "board.h"
//include "pwm.h"

void setupPWM(uint32_t period, uint32_t onTime)
{
	Chip_IOCON_PinMux(LPC_IOCON, 1, 20, 2, 2); //pwm1.2
	Chip_GPIO_SetPinDIROutput(LPC_GPIO, 1, 20);

	Chip_PWM_Init(LPC_PWM1);
	Chip_PWM_Reset(LPC_PWM1);
	Chip_PWM_PrescaleSet(LPC_PWM1, 299); // PCLK (Systemcoreclock/4) / 299 = 100kHz
	Chip_PWM_ResetOnMatchEnable(LPC_PWM1, 0);
	Chip_PWM_SetMatch(LPC_PWM1, 0, period); // PR / period (100) = 1kHz
	Chip_PWM_SetMatch(LPC_PWM1, 2, onTime);
	Chip_PWM_LatchEnable(LPC_PWM1, 0, 1);
	Chip_PWM_LatchEnable(LPC_PWM1, 2, 1);
	Chip_PWM_SetControlMode(LPC_PWM1, 2, PWM_SINGLE_EDGE_CONTROL_MODE, PWM_OUT_ENABLED);
	Chip_PWM_Enable(LPC_PWM1);
}
