//
// Created by kai on 2021/3/9.
//

#ifndef MAX32660_I2C_UARTRECEIVER_H
#define MAX32660_I2C_UARTRECEIVER_H

#include "uart.h"

/**
 * @brief Init the uart to receive message
 */
void My_UART0_Init();

/**
 * @brief start to receive a character from uart0
 */
void Uart_Start_Receive();

/**
 * @brief the call back function when receiving a character
 *      solving the character and cut the string to title and message
 *      and start to receive a character again
 * @param req
 * @param error
 */
void read_cb(uart_req_t *req, int error);


#endif //MAX32660_I2C_UARTRECEIVER_H
