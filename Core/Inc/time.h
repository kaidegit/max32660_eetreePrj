#ifndef MAX32660_I2C_TIME_H
#define MAX32660_I2C_TIME_H

#include <stdbool.h>
#include "main.h"

typedef struct {
    uint32_t year;
    uint8_t month;
    uint32_t day;
    uint8_t hour;
    uint8_t minute;
    uint8_t second;
    uint8_t weekday;
    bool leap;
}time;

/**
 * @brief get the time from RTC clock
 */
void GetNowTime();

/**
 * @brief init the RTC clock and set a time
 */
void Clock_Init();

/**
 * @brief solve the time string from uart
 * @param timeString
 */
void SolveTimeString(char *timeString);

#endif //MAX32660_I2C_TIME_H
