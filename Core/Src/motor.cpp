#include "motor.h"
#include "gpio.h"
#include "mxc_delay.h"

gpio_cfg_t motor_pin_cfg={
        .port = MOTOR_PORT,
        .mask = MOTOR_PIN,
        .func = GPIO_FUNC_OUT,
        .pad = GPIO_PAD_NONE
};

void Motor_Init(){
    GPIO_Config(&motor_pin_cfg);
    GPIO_OutClr(&motor_pin_cfg);
}

void Motor_Click(){
    GPIO_OutSet(&motor_pin_cfg);
    mxc_delay(MXC_DELAY_MSEC(8));
    GPIO_OutClr(&motor_pin_cfg);
}