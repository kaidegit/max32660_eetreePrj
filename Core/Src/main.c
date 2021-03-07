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

#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"

int main() {
    uint8_t recv_dat[6] = {0};
    float temperature = 0.0;
    float humidity = 0.0;
    char ch[30];
    time nowTime;

    I2C_Init(MXC_I2C1, I2C_STD_MODE, NULL);

    sys_cfg_rtc_t sys_cfg;
    sys_cfg.tmr = MXC_TMR0;
    RTC_Init(MXC_RTC, 0, 0, &sys_cfg);
    RTC_EnableRTCE(MXC_RTC);

    My_UART0_Init();

//    gpio_cfg_t gpio_in;
//    gpio_in.port = MAX30102_INT_Port;
//    gpio_in.mask = MAX30102_INT_Pin;
//    gpio_in.pad = GPIO_PAD_PULL_UP;
//    gpio_in.func = GPIO_FUNC_IN;
//    GPIO_Config(&gpio_in);

    OLED_Init();
    OLED_Clear();
    SHT30_Reset();
    SHT30_Init();
    Uart_Start_Receive();
//    Max30102_Init();

    OLED_ShowChinese(96, 0, 0);         //体
    OLED_ShowChinese(112, 0, 1);        //温
    OLED_ShowChinese(0, 0, 2);            //心
    OLED_ShowChinese(16, 0, 3);           //率

    while (1) {
        SHT30_Read_Dat(recv_dat);
        SHT30_Dat_To_Float(recv_dat, &temperature, &humidity);
        sprintf(ch, "%.1f", temperature);
        OLED_ShowString(96, 4, ch, 16);


//        Max30102_Task();
//        if (!GPIO_InGet(&gpio_in)){
//            Max30102_InterruptCallback();
//        }
//        printf("HR: %d\n\rSpO2: %d\n\r", Max30102_GetHeartRate(), Max30102_GetSpO2Value());

        LED_On(0);
        mxc_delay(MXC_DELAY_MSEC(300));
        LED_Off(0);
        mxc_delay(MXC_DELAY_MSEC(300));

        nowTime = GetNowTime();
        sprintf(ch, "%02d:%02d:%02d", nowTime.hour, nowTime.minute, nowTime.second);
        OLED_ShowString(32, 4, ch, 12);
    }
}

#pragma clang diagnostic pop