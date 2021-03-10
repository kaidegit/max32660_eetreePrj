//
// Created by yekai on 2021/3/8.
//

#include "screen.h"
#include "oled.h"
#include "time.h"
#include "string.h"
#include "stdio.h"

extern time nowTime;
extern char notificationTitle[30];
extern char notificationMessage[255];
extern enum ActName nowAct;
uint16_t displayTempOrNotiTime;


void Oled_Task(uint8_t tickTime) {
    switch (nowAct) {
        case Temperature:
            // 仅首次执行
            if (displayTempOrNotiTime == 0){
                OLED_Clear();
                Oled_ShowTemperatureBackground();
            }
            // 10s内刷新
            if (displayTempOrNotiTime < 10000 / tickTime) {
                Oled_ShowTemperature();
                displayTempOrNotiTime++;
            } else {
                nowAct = Time;
                OLED_Clear();
            }
            break;
        case Notification:
            // 仅首次执行
            if (displayTempOrNotiTime == 0) {
                OLED_Clear();
                Oled_ShowNotification();
            }
            // 5s内持续执行
            if (displayTempOrNotiTime < 5000 / tickTime) {
                displayTempOrNotiTime++;
            } else {
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
    char weekStr[7][3] = {
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
    OLED_ShowChinese(96, 0, 0);         //体
    OLED_ShowChinese(112, 0, 1);        //温
}

void Oled_ShowTemperatureBackground() {
    OLED_ShowChinese(96, 0, 0);         //体
    OLED_ShowChinese(112, 0, 1);        //温
}
void Oled_ShowNotification() {
    OLED_ShowString(0,0,notificationTitle,16);
    OLED_ShowString(0,2,notificationMessage,16);
}