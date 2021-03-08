//
// Created by kai on 2021/3/7.
//

#include <stdbool.h>
#include "notifications.h"
#include "uart.h"
#include "main.h"
#include "max32660.h"
#include "stdio.h"
#include "string.h"

uint8_t uartRcv = 0;
char revBuf[255] = {0};
char notificationTitle[30];
char notificationMessage[255];

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
    uart_cfg_t uart_cfg;
    const sys_cfg_uart_t sys_uart_cfg = {
            MAP_A,
            UART_FLOW_DISABLE,
    };
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
    int i = 0;
    if (uartRcv == '\"') {   //从"开始记录，到"结束记录
        if (!recording) {    //如果还未开始记录，开始记录
            recording = true;
        } else {             //如果已经开始记录，结束记录并显示
            position = 0;
            printf("%s", revBuf);
            memset(notificationTitle,0,sizeof(notificationTitle));
            memset(notificationMessage,0, sizeof(notificationMessage));
            //分离标题和内容。上位机中用:分割
            for (i = 0; (i < strlen(revBuf)) && (revBuf[i] != ':'); i++) {
                notificationTitle[i] = revBuf[i];
            }
            if (i != strlen(revBuf)){
                strcpy(revBuf + i, notificationMessage);
            }
            //TODO display the notification
            memset(revBuf, 0, sizeof(revBuf));
        }
    } else if (recording) {  //如果正在记录，就记录
        revBuf[position] = uartRcv;
        position++;
    }
    printf("rec\n");
    printf("%c\n", uartRcv);
    UART_ReadAsync(MXC_UART_GET_UART(0), &read_req);
}

void Uart_Start_Receive() {
    UART_ReadAsync(MXC_UART_GET_UART(0), &read_req);
}
