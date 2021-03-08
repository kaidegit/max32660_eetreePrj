//
// Created by kai on 2021/3/4.
//

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

void GetNowTime();

void Clock_Init();

#endif //MAX32660_I2C_TIME_H
