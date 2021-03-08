//
// Created by kai on 2021/3/4.
//

#include "time.h"
#include "rtc.h"
#include "mxc_delay.h"

time nowTime;

static sys_cfg_rtc_t sys_cfg = {
        .tmr = MXC_TMR0
};

uint8_t dayRedress_leap[12] = {
        0, 3, 4, 0, 2, 5, 0, 3, 6, 1, 4, 6
};
uint8_t dayRedress_common[12] = {
        0, 3, 3, 6, 1, 4, 6, 2, 5, 0, 3, 5
};

void GetNowTime() {
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

    if (day >= 1) {
        nowTime.day = nowTime.day + 1;
        RTC_Init(MXC_RTC, sec - 24 * 60 * 60, 0, &sys_cfg);
    }
    if ((nowTime.year % 400 == 0) || ((nowTime.year % 100 != 0) && (nowTime.year % 4 == 0))) {
        nowTime.leap = true;
    } else {
        nowTime.leap = false;
    }
    switch (nowTime.month) {
        case 1:
        case 3:
        case 5:
        case 7:
        case 8:
        case 10:
        case 12:
            if (nowTime.day >= 32) {
                nowTime.month++;
                nowTime.day -= 31;
            }
            break;
        case 4:
        case 6:
        case 9:
        case 11:
            if (nowTime.day >= 31) {
                nowTime.month++;
                nowTime.day -= 30;
            }
            break;
        case 2:
            if (nowTime.leap) {
                if (nowTime.day >= 30) {
                    nowTime.month++;
                    nowTime.day -= 29;
                }
            } else {
                if (nowTime.day >= 29) {
                    nowTime.month++;
                    nowTime.day -= 28;
                }
            }
            break;
        default:
            break;
    }
    /// （年+年/4+年/400-年/100+月日天数-1）/7＝XX……余星期几
    nowTime.weekday = (nowTime.year + nowTime.year / 4 + nowTime.year / 400 - nowTime.year / 100
                       + (nowTime.leap ? dayRedress_leap[nowTime.month - 1] :
                          dayRedress_common[nowTime.month - 1]) + nowTime.day - 1) % 7;
    nowTime.hour = hr;
    nowTime.minute = min;
    nowTime.second = sec;
}

void Clock_Init() {
    mxc_delay(MXC_DELAY_MSEC(300)); ///To avoid rtc starting failed
    RTC_Init(MXC_RTC, 0, 0, &sys_cfg);
    RTC_EnableRTCE(MXC_RTC);
    nowTime.year = 2021;
    nowTime.month = 3;
    nowTime.day = 8;
    nowTime.hour = 0;
    nowTime.minute = 0;
    nowTime.second = 0;
    nowTime.leap = false;
}