//
// Created by kai on 2021/3/7.
//

#ifndef MAX32660_I2C_MAX30102_H
#define MAX32660_I2C_MAX30102_H

#include <stdbool.h>
#include "i2c.h"

#define MAX30102_ADDR 0x57
#define MAX30102_ADDR_WRITE (MAX30102_ADDR << 1)
#define MAX30102_ADDR_READ (MAX30102_ADDR_WRITE + 1)

#define REG_INTR_STATUS_1 0x00
#define REG_INTR_STATUS_2 0x01
#define REG_INTR_ENABLE_1 0x02
#define REG_INTR_ENABLE_2 0x03
#define REG_FIFO_WR_PTR 0x04
#define REG_OVF_COUNTER 0x05
#define REG_FIFO_RD_PTR 0x06
#define REG_FIFO_DATA 0x07
#define REG_FIFO_CONFIG 0x08
#define REG_MODE_CONFIG 0x09
#define REG_SPO2_CONFIG 0x0A
#define REG_LED1_PA 0x0C
#define REG_LED2_PA 0x0D
#define REG_PILOT_PA 0x10
#define REG_MULTI_LED_CTRL1 0x11
#define REG_MULTI_LED_CTRL2 0x12
#define REG_TEMP_INTR 0x1F
#define REG_TEMP_FRAC 0x20
#define REG_TEMP_CONFIG 0x21
#define REG_PROX_INT_THRESH 0x30
#define REG_REV_ID 0xFE
#define REG_PART_ID 0xFF

/**
* \brief        Write a value to a MAX30102 register
* \param[in]    uch_addr    - register address
* \param[in]    uch_data    - register data
* \retval       true on success
*/
int maxim_max30102_write_reg(uint8_t uch_addr, uint8_t uch_data);

/**
* \brief        Read a MAX30102 register
* \param[in]    uch_addr    - register address
* \param[out]   puch_data    - pointer that stores the register data
* \retval       true on success
*/
int maxim_max30102_read_reg(uint8_t uch_addr, uint8_t *puch_data);

/**
* \brief        Initialize the MAX30102
* \param        None
* \retval       true on success
*/
bool maxim_max30102_init();

/**
* \brief        Read a set of samples from the MAX30102 FIFO register
* \param[out]   *pun_red_led   - pointer that stores the red LED reading data
* \param[out]   *pun_ir_led    - pointer that stores the IR LED reading data
* \retval       true on success
*/
bool maxim_max30102_read_fifo(uint32_t *pun_red_led, uint32_t *pun_ir_led);

/**
* \brief        Reset the MAX30102
* \param        None
* \retval       true on success
*/
bool maxim_max30102_reset();


#endif //MAX32660_I2C_MAX30102_H
