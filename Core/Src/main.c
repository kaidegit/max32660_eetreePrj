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

//#define MAX30102_INT_Port                PORT_0
//#define MAX30102_INT_Pin                 PIN_4

void UART0_IRQHandler(void) {
    printf("uartirq\n");
    UART_Handler(MXC_UART0);
}

float temperature = 0.0;
float humidity = 0.0;
extern time nowTime;

char weekStr[7][3] = {
        "Su", "Mo", "Tu", "We", "Th", "Fr", "Sa"
};

#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"

int main() {
    uint8_t recv_dat[6] = {0};
    char ch[30];

    I2C_Init(MXC_I2C1, I2C_STD_MODE, NULL);
    Clock_Init();
    My_UART0_Init();
    OLED_Init();
    OLED_Clear();
    SHT30_Reset();
    SHT30_Init();
    Uart_Start_Receive();
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
        SHT30_Read_Dat(recv_dat);
        SHT30_Dat_To_Float(recv_dat, &temperature, &humidity);
        sprintf(ch, "%.1f", temperature);
//        OLED_ShowString(96, 4, ch, 16);


//        Max30102_Task();
//        if (!GPIO_InGet(&gpio_in)){
//            Max30102_InterruptCallback();
//        }
//        printf("HR: %d\n\rSpO2: %d\n\r", Max30102_GetHeartRate(), Max30102_GetSpO2Value());

        LED_On(0);
        mxc_delay(MXC_DELAY_MSEC(300));
        LED_Off(0);
        mxc_delay(MXC_DELAY_MSEC(300));

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
//        sprintf(ch, "%02d:%02d:%02d", nowTime.hour, nowTime.minute, nowTime.second);
//        OLED_ShowString(32, 4, ch, 12);
    }
}

#pragma clang diagnostic pop