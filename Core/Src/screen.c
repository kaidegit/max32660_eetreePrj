//
// Created by yekai on 2021/3/8.
//

#include "screen.h"
#include "oled.h"
#include "time.h"
#include "string.h"
#include "stdio.h"
#include "temperature.h"
#include "motor.h"

extern time nowTime;
extern char notificationTitle[30];
extern char notificationMessage[255];
extern float temperature;
extern enum ActName nowAct;
uint16_t displayTempOrNotiTime;


void Oled_Task(uint8_t tickTime) {
    switch (nowAct) {
        case Temperature:
            if (displayTempOrNotiTime == 0) {                       // 仅首次执行
                OLED_Clear();
                Oled_ShowTemperatureBackground();
                displayTempOrNotiTime++;
            } else if (displayTempOrNotiTime < 5000 / tickTime) {  // 5s内刷新
                Oled_ShowTemperature();
                GetTemperature();
                displayTempOrNotiTime++;
            } else {                                                // 结束后执行
                nowAct = Time;
                OLED_Clear();
                Oled_ShowTemperature();
            }
            break;
        case Notification:
            if (displayTempOrNotiTime == 0) {                       // 仅首次执行
                OLED_Clear();
                Oled_ShowNotification();
                Motor_Click();
                displayTempOrNotiTime++;
            } else if (displayTempOrNotiTime < 5000 / tickTime) {   // 5s内持续执行
                displayTempOrNotiTime++;
            } else {                                                // 结束后执行
                nowAct = Time;
                OLED_Clear();
            }
            break;
        default:
            displayTempOrNotiTime = 0;
            Oled_ShowTime();
            break;
    }
}

void Oled_ShowTime() {
    const char weekStr[7][3] = {
            "Su", "Mo", "Tu", "We", "Th", "Fr", "Sa"
    };

    char ch[30];
    GetNowTime();
    OLED_ShowBigNum(0, 3, nowTime.hour / 10);
    OLED_ShowBigNum(16, 3, nowTime.hour % 10);
    OLED_ShowBigNum(32, 3, 10);
    OLED_ShowBigNum(48, 3, nowTime.minute / 10);
    OLED_ShowBigNum(64, 3, nowTime.minute % 10);
    OLED_ShowBigNum(80, 3, 10);
    OLED_ShowBigNum(96, 3, nowTime.second / 10);
    OLED_ShowBigNum(112, 3, nowTime.second % 10);
    OLED_ShowChinese(32, 0, 4);
    OLED_ShowChinese(64, 0, 5);
    OLED_ShowChinese(96, 0, 6);
    sprintf(ch, "%04d", nowTime.year);
    OLED_ShowString(0, 0, ch, 16);
    sprintf(ch, "%02d", nowTime.month);
    OLED_ShowString(48, 0, ch, 16);
    sprintf(ch, "%02d", nowTime.day);
    OLED_ShowString(80, 0, ch, 16);
    OLED_ShowString(112, 0, weekStr[nowTime.weekday], 16);
}

void Oled_ShowTemperature() {
    static float temptemp = 0;
    if (temptemp != temperature) {
        OLED_ShowBigNum(0, 3, (int) (temperature / 10));
        OLED_ShowBigNum(16, 3, (int) temperature % 10);
        OLED_ShowBigNum(32, 3, 11);
        OLED_ShowBigNum(48, 3, (int) ((temperature - (int) temperature) * 10));
        OLED_ShowBigNum(64,3,12);
        OLED_ShowBigNum(80,3,13);
        if (temperature >= 37.5) {
            OLED_ShowChinese(104,3,9);
            OLED_ShowChinese(104, 5, 10);
        }else{
            OLED_ShowChinese(104,3,7);
            OLED_ShowChinese(104,5,8);
        }
        temptemp = temperature;
    }
}

void Oled_ShowTemperatureBackground() {
    OLED_ShowChinese(0, 0, 0);         //体
    OLED_ShowChinese(16, 0, 1);        //温
}

void Oled_ShowNotification() {
    OLED_ShowString(0, 0, notificationTitle, 16);
    OLED_ShowString(0, 2, notificationMessage, 16);
}