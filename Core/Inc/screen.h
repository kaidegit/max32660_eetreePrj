//
// Created by yekai on 2021/3/8.
//

#ifndef MAX32660_I2C_SCREEN_H
#define MAX32660_I2C_SCREEN_H

#include "stdint.h"

enum ActName {
    Time = 1, Temperature, Notification
};

void Oled_Task(uint8_t tickTime);

void Oled_ShowTime();

void Oled_ShowTemperature();

void Oled_ShowTemperatureBackground();

void Oled_ShowNotification();

#endif //MAX32660_I2C_SCREEN_H
