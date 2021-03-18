//
// Created by kai on 2021/3/9.
//

#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "uartReceiver.h"
#include "uart.h"
#include "screen.h"
#include "clock.h"

uint8_t uartRcv = 0;
char revBuf[255] = {0};
char title[30];
char message[255];

extern char notificationTitle[30];
extern char notificationMessage[255];

extern enum ActName nowAct;

static uart_req_t read_req = {
        .data = &uartRcv,
        .len = 1,
        .num = 1,
        .callback = read_cb
};

void My_UART0_Init() {
    NVIC_ClearPendingIRQ(MXC_UART_GET_IRQ(0));
    NVIC_DisableIRQ(MXC_UART_GET_IRQ(0));
    NVIC_SetPriority(MXC_UART_GET_IRQ(0), 1);
    NVIC_EnableIRQ(MXC_UART_GET_IRQ(0));

    const sys_cfg_uart_t sys_uart_cfg = {
            MAP_A,
            UART_FLOW_DISABLE,
    };

    uart_cfg_t uart_cfg;
    uart_cfg.parity = UART_PARITY_DISABLE;
    uart_cfg.size = UART_DATA_SIZE_8_BITS;
    uart_cfg.stop = UART_STOP_1;
    uart_cfg.flow = UART_FLOW_CTRL_DIS;
    uart_cfg.pol = UART_FLOW_POL_DIS;
    uart_cfg.baud = 115200;

    UART_Init(MXC_UART_GET_UART(0), &uart_cfg, &sys_uart_cfg);
}

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
//                SolveTimeString(message);
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

void Uart_Start_Receive() {
    UART_ReadAsync(MXC_UART_GET_UART(0), &read_req);
}

void UART0_IRQHandler(void) {
    UART_Handler(MXC_UART0);
}