//
// Created by kai on 2021/3/7.
//

#include "max30102.h"

int maxim_max30102_write_reg(uint8_t uch_addr, uint8_t uch_data) {
    char ach_i2c_data[2];
    ach_i2c_data[0] = uch_addr;
    ach_i2c_data[1] = uch_data;
    return E_NO_ERROR == I2C_MasterWrite(MXC_I2C1, MAX30102_ADDR_WRITE, (uint8_t *) ach_i2c_data, 2, 0);
}

int maxim_max30102_read_reg(uint8_t uch_addr, uint8_t *puch_data) {
    char ch_i2c_data = uch_addr;
    I2C_MasterWrite(MXC_I2C1, MAX30102_ADDR_WRITE, (uint8_t *) &ch_i2c_data, 1, 1);
    return E_NO_ERROR == I2C_MasterRead(MXC_I2C1, MAX30102_ADDR_READ, (uint8_t *) puch_data, 1, 0);
}

bool maxim_max30102_init() {
    if (!maxim_max30102_write_reg(REG_INTR_ENABLE_1, 0xc0)) // INTR setting
        return false;
    if (!maxim_max30102_write_reg(REG_INTR_ENABLE_2, 0x00))
        return false;
    if (!maxim_max30102_write_reg(REG_FIFO_WR_PTR, 0x00))  //FIFO_WR_PTR[4:0]
        return false;
    if (!maxim_max30102_write_reg(REG_OVF_COUNTER, 0x00))  //OVF_COUNTER[4:0]
        return false;
    if (!maxim_max30102_write_reg(REG_FIFO_RD_PTR, 0x00))  //FIFO_RD_PTR[4:0]
        return false;
    if (!maxim_max30102_write_reg(REG_FIFO_CONFIG, 0x0f))  //sample avg = 1, fifo rollover=false, fifo almost full = 17
        return false;
    if (!maxim_max30102_write_reg(REG_MODE_CONFIG, 0x03))   //0x02 for Red only, 0x03 for SpO2 mode 0x07 multimode LED
        return false;
    if (!maxim_max30102_write_reg(REG_SPO2_CONFIG,
                                  0x27))  // SPO2_ADC range = 4096nA, SPO2 sample rate (100 Hz), LED pulseWidth (400uS)
        return false;

    if (!maxim_max30102_write_reg(REG_LED1_PA, 0x24))   //Choose value for ~ 7mA for LED1
        return false;
    if (!maxim_max30102_write_reg(REG_LED2_PA, 0x24))   // Choose value for ~ 7mA for LED2
        return false;
    if (!maxim_max30102_write_reg(REG_PILOT_PA, 0x7f))   // Choose value for ~ 25mA for Pilot LED
        return false;
    return true;
}

bool maxim_max30102_read_fifo(uint32_t *pun_red_led, uint32_t *pun_ir_led) {
    uint32_t un_temp;
    unsigned char uch_temp;
    *pun_red_led = 0;
    *pun_ir_led = 0;
    char ach_i2c_data[6];

    //read and clear status register
    maxim_max30102_read_reg(REG_INTR_STATUS_1, &uch_temp);
    maxim_max30102_read_reg(REG_INTR_STATUS_2, &uch_temp);

    ach_i2c_data[0] = REG_FIFO_DATA;
    if (E_NO_ERROR != I2C_MasterWrite(MXC_I2C1, MAX30102_ADDR_WRITE, (uint8_t *) ach_i2c_data, 1, 1)) {
        return false;
    }
    if (E_NO_ERROR != I2C_MasterRead(MXC_I2C1, MAX30102_ADDR_READ, (uint8_t *) ach_i2c_data, 6, 0)) {
        return false;
    }

    un_temp = (unsigned char) ach_i2c_data[0];
    un_temp <<= 16;
    *pun_red_led += un_temp;
    un_temp = (unsigned char) ach_i2c_data[1];
    un_temp <<= 8;
    *pun_red_led += un_temp;
    un_temp = (unsigned char) ach_i2c_data[2];
    *pun_red_led += un_temp;

    un_temp = (unsigned char) ach_i2c_data[3];
    un_temp <<= 16;
    *pun_ir_led += un_temp;
    un_temp = (unsigned char) ach_i2c_data[4];
    un_temp <<= 8;
    *pun_ir_led += un_temp;
    un_temp = (unsigned char) ach_i2c_data[5];
    *pun_ir_led += un_temp;
    *pun_red_led &= 0x03FFFF;  //Mask MSB [23:18]
    *pun_ir_led &= 0x03FFFF;  //Mask MSB [23:18]

    return true;
}

bool maxim_max30102_reset() {
    return maxim_max30102_write_reg(REG_MODE_CONFIG, 0x40);
}