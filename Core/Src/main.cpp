#include "main.h"
#include "led.h"
#include "mxc_delay.h"
#include "i2c.h"
#include "oled.h"
#include "string.h"
#include "sht30.h"
#include "rtc.h"
#include "clock.h"
#include "uart.h"
#include "notifications.h"
#include "screen.h"
#include "uartReceiver.h"
#include "pb.h"
#include "temperature.h"
#include "motor.h"


enum ActName nowAct;
extern time nowTime;

#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"

int main() {

    I2C_Init(MXC_I2C1, I2C_STD_MODE, NULL);
//    Clock_Init();
    clock time;
    PB_Init();
    My_UART0_Init();
    Motor_Init();
    OLED_Init();
    OLED_Clear();
    SHT30_Reset();
    SHT30_Init();
    Uart_Start_Receive();
    nowAct = Time;

    while (1) {
        if (PB_Get(0)) {
            ShowTemperature();
        }
        Oled_Task(5, time);
        mxc_delay(MXC_DELAY_MSEC(5));
    }
}

#pragma clang diagnostic pop