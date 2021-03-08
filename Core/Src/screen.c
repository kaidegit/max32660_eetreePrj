//
// Created by yekai on 2021/3/8.
//

#include "screen.h"
#include "oled.h"
#include "time.h"

extern time nowTime;
extern float temperature;
extern char* notificationTitle;
extern char* notificationMessage;

void Oled_Task(enum ActName Act) {
    switch (Act) {
        case Time:
            Oled_ShowTime();
            break;
        case Temperature:
            Oled_ShowTemperature();
            break;
        case Notification:
            Oled_ShowNotification();
            break;
        default:
            break;
    }
}

void Oled_ShowTime(){
    GetNowTime();

}

void Oled_ShowTemperature(){
    OLED_ShowChinese(96, 0, 0);         //体
    OLED_ShowChinese(112, 0, 1);        //温
}

void Oled_ShowNotification(){

}