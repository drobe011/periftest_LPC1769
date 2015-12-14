#include "board.h"
#include "rtc.h"
#include "mDT.h"

void setupRTC(RTC_TIME_T *thisTime)
{
	if ((LPC_RTC->RTC_AUX & _BIT(RTC_OSCF)) || (FORCE_RTC_SET == 1))
	{
		thisTime->time[RTC_TIMETYPE_SECOND] = (uint32_t)mS;
		thisTime->time[RTC_TIMETYPE_MINUTE] = (uint32_t)mMN;
		thisTime->time[RTC_TIMETYPE_HOUR] = (uint32_t)mHR;
		thisTime->time[RTC_TIMETYPE_DAYOFMONTH] = (uint32_t)mDY;
		thisTime->time[RTC_TIMETYPE_DAYOFWEEK] = (uint32_t)mWD;
		thisTime->time[RTC_TIMETYPE_DAYOFYEAR] = (uint32_t)mDOY;
		thisTime->time[RTC_TIMETYPE_MONTH] = (uint32_t)mMO;
		thisTime->time[RTC_TIMETYPE_YEAR] = (uint32_t)mYR;

		Chip_RTC_SetFullTime(LPC_RTC, thisTime);
		LPC_RTC->RTC_AUX |= _BIT(RTC_OSCF);
		Chip_RTC_Enable(LPC_RTC, 1);
	}
}

void showTime(RTC_TIME_T *pTime)
{
	NVIC_ClearPendingIRQ(RITIMER_IRQn);
	NVIC_DisableIRQ(RITIMER_IRQn);
	Chip_RTC_GetFullTime(LPC_RTC, pTime);
	#ifdef DEBUG_ENABLE
	printf("\r\nTime: %.2d:%.2d:%.2d",
		   pTime->time[RTC_TIMETYPE_HOUR], pTime->time[RTC_TIMETYPE_MINUTE],
		   pTime->time[RTC_TIMETYPE_SECOND]);
	printf(" %.2d/%.2d/%.4d\r\n", pTime->time[RTC_TIMETYPE_MONTH],
		   pTime->time[RTC_TIMETYPE_DAYOFMONTH],
		   pTime->time[RTC_TIMETYPE_YEAR]);
	#endif
	NVIC_ClearPendingIRQ(RITIMER_IRQn);
	NVIC_EnableIRQ(RITIMER_IRQn);
}