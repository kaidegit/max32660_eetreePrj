#include "main.h"
#include "led.h"
#include "mxc_delay.h"
#include "i2c.h"
#include "oled.h"
#include "string.h"
#include "sht30.h"
#include "rtc.h"
#include "time.h"
#include "uart.h"
#include "notifications.h"
//#include "MAX30102.h"
#include "screen.h"
#include "uartReceiver.h"
#include "pb.h"
#include "temperature.h"
#include "motor.h"

//#define MAX30102_INT_Port                PORT_0
//#define MAX30102_INT_Pin                 PIN_4

enum ActName nowAct;
extern time nowTime;

#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"

int main() {
    I2C_Init(MXC_I2C1, I2C_STD_MODE, NULL);
    Clock_Init();
    PB_Init();
    My_UART0_Init();
    Motor_Init();
    OLED_Init();
    OLED_Clear();
    SHT30_Reset();
    SHT30_Init();
    Uart_Start_Receive();
    nowAct = Time;
//    gpio_cfg_t gpio_in;
//    gpio_in.port = MAX30102_INT_Port;
//    gpio_in.mask = MAX30102_INT_Pin;
//    gpio_in.pad = GPIO_PAD_PULL_UP;
//    gpio_in.func = GPIO_FUNC_IN;
//    GPIO_Config(&gpio_in);


//    Max30102_Init();

//    OLED_ShowChinese(96, 0, 0);         //体
//    OLED_ShowChinese(112, 0, 1);        //温
//    OLED_ShowChinese(0, 0, 2);            //心
//    OLED_ShowChinese(16, 0, 3);           //率

    while (1) {
        Oled_Task(5);
        mxc_delay(MXC_DELAY_MSEC(5));
        if (PB_Get(0)){
            ShowTemperature();
        }
//        SHT30_Read_Dat(recv_dat);
//        SHT30_Dat_To_Float(recv_dat, &temperature, &humidity);
//        sprintf(ch, "%.1f", temperature);
//        OLED_ShowString(96, 4, ch, 16);


//        Max30102_Task();
//        if (!GPIO_InGet(&gpio_in)){
//            Max30102_InterruptCallback();
//        }
//        printf("HR: %d\n\rSpO2: %d\n\r", Max30102_GetHeartRate(), Max30102_GetSpO2Value());

//        LED_On(0);
//        mxc_delay(MXC_DELAY_MSEC(300));
//        LED_Off(0);
//        mxc_delay(MXC_DELAY_MSEC(300));

//        Oled_ShowTime();

//        sprintf(ch, "%02d:%02d:%02d", nowTime.hour, nowTime.minute, nowTime.second);
//        OLED_ShowString(32, 4, ch, 12);
    }
}

#pragma clang diagnostic pop