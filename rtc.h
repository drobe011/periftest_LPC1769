/*!
 * \file rtc.h
 *
 * \author David Robertson
 * \date 15 December, 2015
 *
 * LPC1769 internal RTC
 */
/** @defgroup RTC LPC1769 Real Time Clock
 *  Module for setting and reading the RTC
 *  @{
 */
#ifndef RTC_H
#define RTC_H

#define RTC_OSCF 4
#define FORCE_RTC_SET 0

/*!
 * \brief Initialize RTC
 *
 * Tests if RTC domain has reset.  If so, sets date/
 * time based on header mDT.h.
 *
 * \param[in] thisTime date/time data struct.
 */
void setupRTC(RTC_TIME_T *thisTime);
/*!
 * \brief Retrieve data/time from RTC and send to STDIO
 *
 * \param[in] pTime pointer to date/time data struct.
 */
void showTime(RTC_TIME_T *pTime);

#endif
/** @} */