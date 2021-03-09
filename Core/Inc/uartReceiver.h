//
// Created by kai on 2021/3/9.
//

#ifndef MAX32660_I2C_UARTRECEIVER_H
#define MAX32660_I2C_UARTRECEIVER_H

#include "uart.h"

void My_UART0_Init();

void Uart_Start_Receive();

void read_cb(uart_req_t *req, int error);


#endif //MAX32660_I2C_UARTRECEIVER_H
