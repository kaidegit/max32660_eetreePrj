//
// Created by yekai on 2021/3/8.
//

#ifndef MAX32660_I2C_SCREEN_H
#define MAX32660_I2C_SCREEN_H

#include "stdint.h"
#include "clock.h"

enum ActName {
    Time = 1, Temperature, Notification
};

/**
 * @brief show the act screen according to the nowAct
 * @note show Temperature and Notification act for only 5 seconds
 *      and turn back to Time act
 * @param tickTime
 */
void Oled_Task(uint8_t tickTime, clock time);

/**
 * @brief show the time to the screen
 */
void Oled_ShowTime(clock time);

/**
 * @brief show the temperature to the screen
 */
void Oled_ShowTemperature();

/**
 * @brief show the back ground when starting to show temperature act
 */
void Oled_ShowTemperatureBackground();

/**
 * @brief show the notification to the screen
 */
void Oled_ShowNotification();

#endif //MAX32660_I2C_SCREEN_H
