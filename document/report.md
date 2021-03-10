本代码主要实现了日期星期时间显示，体温测量，通知提醒等手表上常见的功能

# 硬件连接

I2C总线(P0_3&P0_2):一个OLED屏幕和一个SHT30模块

UART外设(RX:P0_5):蓝牙串口透传模块

GPIO(P0_6):一个线性马达

# 数据处理

## 时间数据

使用RTC模块的时间中的时分秒，使用普通变量记录年月日，使用万年历算法计算星期

```c
uint8_t dayRedress_leap[12] = {
        0, 3, 4, 0, 2, 5, 0, 3, 6, 1, 4, 6
};
uint8_t dayRedress_common[12] = {
        0, 3, 3, 6, 1, 4, 6, 2, 5, 0, 3, 5
};

void GetNowTime() {
    uint32_t day, hr, min, sec;

    const uint32_t SECS_PER_MIN = 60;
    const uint32_t SECS_PER_HR = 60 * SECS_PER_MIN;
    const uint32_t SECS_PER_DAY = 24 * SECS_PER_HR;

    sec = RTC_GetSecond();

    day = sec / SECS_PER_DAY;
    sec -= day * SECS_PER_DAY;

    hr = sec / SECS_PER_HR;
    sec -= hr * SECS_PER_HR;

    min = sec / SECS_PER_MIN;
    sec -= min * SECS_PER_MIN;

    if (day >= 1) {
        nowTime.day = nowTime.day + 1;
        RTC_Init(MXC_RTC, sec - 24 * 60 * 60, 0, &sys_cfg);
        RTC_EnableRTCE(MXC_RTC);
    }
    if ((nowTime.year % 400 == 0) || ((nowTime.year % 100 != 0) && (nowTime.year % 4 == 0))) {
        nowTime.leap = true;
    } else {
        nowTime.leap = false;
    }
    switch (nowTime.month) {
        case 1: case 3: case 5: case 7: case 8: case 10: case 12:
            if (nowTime.day >= 32) {
                nowTime.month++;
                nowTime.day -= 31;
            }
            break;
        case 4: case 6: case 9: case 11:
            if (nowTime.day >= 31) {
                nowTime.month++;
                nowTime.day -= 30;
            }
            break;
        case 2:
            if (nowTime.leap) {
                if (nowTime.day >= 30) {
                    nowTime.month++;
                    nowTime.day -= 29;
                }
            } else {
                if (nowTime.day >= 29) {
                    nowTime.month++;
                    nowTime.day -= 28;
                }
            }
            break;
        default:
            break;
    }
    /// （年+年/4+年/400-年/100+月日天数-1）/7＝XX……余星期几
    nowTime.weekday = (nowTime.year + nowTime.year / 4 + nowTime.year / 400 - nowTime.year / 100
                       + (nowTime.leap ? dayRedress_leap[nowTime.month - 1] :
                          dayRedress_common[nowTime.month - 1]) + nowTime.day - 1) % 7;
    nowTime.hour = hr;
    nowTime.minute = min;
    nowTime.second = sec;
}
```

## 温度数据

温度数据可以直接读取SHT30传感器的温湿度，只要温度即可。SHT30的驱动可在[我的github工程](https://github.com/kaidegit/HWDrivers4MCU/tree/main/SHT30_I2c)中找到

```c
void GetTemperature() {
    float humidity = 0.0;
    uint8_t recv_dat[30];
    SHT30_Read_Dat(recv_dat);
    SHT30_Dat_To_Float(recv_dat, &temperature, &humidity);
}

void ShowTemperature(){
    GetTemperature();
    nowAct = Temperature;
}
```

## 串口下发数据

串口下发设定时间数据和通知数据。上位机发送数据格式为"标题:内容"，特殊地，如果标题为time，即为设置时间指令。串口设为异步接收一个字符，接收回调函数如下

```c
void read_cb(uart_req_t *req, int error) {
    static uint8_t position = 0;
    static bool recording = false;
    uint8_t i = 0, j = 0, len = 0;
    if (uartRcv == '\"') {   //从"开始记录，到"结束记录
        if (!recording) {    //如果还未开始记录，开始记录
            recording = true;
        } else {             //如果已经开始记录，结束记录并显示
            memset(title, 0, sizeof(title));
            memset(message, 0, sizeof(message));
            // 分离标题和内容。上位机中用:分割
            len = strlen(revBuf);
            for (i = 0; (i < len) && (revBuf[i] != ':'); i++) {
                title[i] = revBuf[i];
            }
            for (i++, j = 0; i < len; i++, j++) {
                message[j] = revBuf[i];
            }
            // 检查是否为设置时间的通知
            if (strcmp(title, "time") == 0) {
                SolveTimeString(message);
            } else {
                strcpy(notificationTitle, title);
                strcpy(notificationMessage, message);
                nowAct = Notification;
            }
            // 重置到未读取过字符串状态
            memset(revBuf, 0, sizeof(revBuf));
            recording = false;
            position = 0;
        }
    } else if (recording) {  //如果正在记录，就记录
        revBuf[position] = uartRcv;
        position++;
    }
    UART_ReadAsync(MXC_UART_GET_UART(0), &read_req);
}
```

设置时间函数如下

```c
void SolveTimeString(char *timeString) {
    // 上位机发送数据为YYYY-MM-DD-HH-MM-SS
    nowTime.year = atoi(timeString);
    nowTime.month = atoi(timeString + 5);
    nowTime.day = atoi(timeString + 8);
    nowTime.hour = atoi(timeString + 11);
    nowTime.minute = atoi(timeString + 14);
    nowTime.second = atoi(timeString + 17);
    RTC_DisableRTCE(MXC_RTC);
    RTC_Init(MXC_RTC, nowTime.hour * 60 * 60 + nowTime.minute * 60 + nowTime.second, 0, &sys_cfg);
    RTC_EnableRTCE(MXC_RTC);
    // 更新星期数和闰年
    GetNowTime();
}
```

# 人机交互

## OLED显示模块

我使用的是SSD1306的I2C的128x64分辨率的OLED模块，相关驱动也可在[我的github工程](https://github.com/kaidegit/HWDrivers4MCU/tree/main/OLED_SSD1306_i2c)找到

我简单地把显示部分分成了三个场景

```c
enum ActName {
    Time = 1, Temperature, Notification
};
```

默认显示Time场景，按下按钮后切换到Temperature场景，收到串口下发非调整时间的数据切换到Notification场景。Temperature和Notification场景持续5秒后切换回Time场景

```c
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
```

Time场景显示部分

```c
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
```

Temperature场景显示部分

```c
void Oled_ShowTemperature() {
    char ch[30];
    static float temptemp = 0;
    if (temptemp != temperature){
        sprintf(ch, "%.1f", temperature);
        OLED_ShowString(96, 4, ch, 16);
        temptemp = temperature;
    }
}

void Oled_ShowTemperatureBackground() {
    OLED_ShowChinese(96, 0, 0);         //体
    OLED_ShowChinese(112, 0, 1);        //温
}
```

Notification场景显示部分

```c
void Oled_ShowNotification() {
    OLED_ShowString(0, 0, notificationTitle, 16);
    OLED_ShowString(0, 2, notificationMessage, 16);
}
```

## 线性马达震动模块

由于线性马达网上没找到什么单片机控制的教程，购买的小米8的线性马达震感似乎有点弱，也可能是我调教问题。总之，能在通知来时震一下做一个提醒。代码如下

```c
void Motor_Click(){
    GPIO_OutSet(&motor_pin_cfg);
    mxc_delay(MXC_DELAY_MSEC(8));
    GPIO_OutClr(&motor_pin_cfg);
}
```

原理图如图

![motorDriver](motorDriver.png)





