#include "main.h"
#include "led.h"
#include "mxc_delay.h"
#include "i2c.h"
#include "oled.h"
#include "string.h"
#include "sht30.h"
#include "rtc.h"
#include "time.h"
#include "algorithm.h"
#include "max30102.h"

#define MAX_BRIGHTNESS 255

uint32_t aun_ir_buffer[500]; //IR LED sensor data
int32_t n_ir_buffer_length;    //data length
uint32_t aun_red_buffer[500];    //Red LED sensor data
int32_t n_sp02; //SPO2 value
int8_t ch_spo2_valid;   //indicator to show if the SP02 calculation is valid
int32_t n_heart_rate;   //heart rate value
int8_t ch_hr_valid;    //indicator to show if the heart rate calculation is valid
uint8_t uch_dummy;

int main() {
    uint8_t recv_dat[6] = {0};
    float temperature = 0.0;
    float humidity = 0.0;
    char ch[30];
    sys_cfg_rtc_t sys_cfg;
    time nowTime;
    uint32_t un_min, un_max, un_prev_data;  //variables to calculate the on-board LED brightness that reflects the heartbeats
    int i;
    int32_t n_brightness;
    float f_temp;

    I2C_Init(MXC_I2C1, I2C_STD_MODE, NULL);
    sys_cfg.tmr = MXC_TMR0;
    RTC_Init(MXC_RTC, 0, 0, &sys_cfg);
    RTC_EnableRTCE(MXC_RTC);

    OLED_Init();
    OLED_Clear();
    SHT30_Reset();
    SHT30_Init();
    maxim_max30102_reset();
    mxc_delay(MXC_DELAY_MSEC(1000));
    maxim_max30102_read_reg(0, &uch_dummy);
    maxim_max30102_init();

    OLED_ShowChinese(96, 0, 0);         //体
    OLED_ShowChinese(112, 0, 1);        //温
    OLED_ShowChinese(0, 0, 2);            //心
    OLED_ShowChinese(16, 0, 3);           //率

    while (1) {
        SHT30_Read_Dat(recv_dat);
        SHT30_Dat_To_Float(recv_dat, &temperature, &humidity);
        sprintf(ch, "%.1f", temperature);
        OLED_ShowString(96, 4, ch, 16);

//        LED_On(0);
//        mxc_delay(MXC_DELAY_MSEC(500));
//        LED_Off(0);
//        mxc_delay(MXC_DELAY_MSEC(1500));

        nowTime = GetNowTime();
        sprintf(ch, "%02d:%02d:%02d", nowTime.hour, nowTime.minute, nowTime.second);
        OLED_ShowString(32, 4, ch, 12);
    }
}