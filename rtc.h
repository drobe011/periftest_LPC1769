#ifndef RTC_H
#define RTC_H

#define RTC_OSCF 4
#define FORCE_RTC_SET 0

void setupRTC(RTC_TIME_T *thisTime);
void showTime(RTC_TIME_T *pTime);

#endif