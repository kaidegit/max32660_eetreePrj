//
// Created by kai on 2021/3/4.
//

#include "time.h"
#include "rtc.h"

time nowTime;

void GetNowTime(){
    uint32_t day, hr, min, sec;

    const uint32_t SECS_PER_MIN = 60;
    const uint32_t SECS_PER_HR = 60 * SECS_PER_MIN;
    const uint32_t SECS_PER_DAY = 24 * SECS_PER_HR;

    sec = RTC_GetSecond();

    day = sec / SECS_PER_DAY;
    sec -= day * SECS_PER_DAY;

    hr = sec / SECS_PER_HR;
    sec -= hr * SECS_PER_HR;

    min = sec / SECS_PER_MIN;
    sec -= min * SECS_PER_MIN;

    nowTime.day = day;
    nowTime.hour = hr;
    nowTime.minute = min;
    nowTime.second = sec;
}