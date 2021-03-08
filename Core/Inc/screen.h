//
// Created by yekai on 2021/3/8.
//

#ifndef MAX32660_I2C_SCREEN_H
#define MAX32660_I2C_SCREEN_H

enum ActName {
    Time = 1, Temperature, Notification
};

void Oled_Task(enum ActName Act);

void Oled_ShowTime();

void Oled_ShowTemperature();

void Oled_ShowNotification();

#endif //MAX32660_I2C_SCREEN_H