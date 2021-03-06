#ifndef __OLED_H_
#define __OLED_H_

#include "stdint.h"
#include "i2c.h"

/// the i2c address of the oled
#define OLED_ADDR 0x78

/**
 * @brief Construct a new oled send object
 *
 * @param dc the control command or the data order
 * @param data
 */
inline void oled_send(uint8_t dc, uint8_t data) {
    uint8_t txdata[2] = {dc, data};
    I2C_MasterWrite(MXC_I2C1, OLED_ADDR, txdata, 2, 0);
//#error Write the i2c send function here.
// for STM32 HAL Lib
// HAL_I2C_Mem_Write(&hi2c1, OLED_ADDR, dc, I2C_MEMADD_SIZE_8BIT, data, 1, 0x100);
}

/**
 * @brief write the control cammand to the oled
 *
 * @param cmd
 */
void OLED_WR_CMD(uint8_t cmd);

/**
 * @brief write the data to the oled
 *
 * @param data
 */
void OLED_WR_DATA(uint8_t data);

/**
 * @brief init the OLED
 *
 */
void OLED_Init(void);

/**
 * @brief clear the oled screen
 *
 */
void OLED_Clear(void);

/**
 * @brief turn on the display
 *
 */
void OLED_Display_On(void);

/**
 * @brief turn off the display
 *
 */
void OLED_Display_Off(void);

void OLED_Set_Pos(uint8_t x, uint8_t y);

void OLED_On(void);

/**
 * @brief display a character at the place on the OLED.
 *
 * @param x the abscissa of the character. Should between 0 and 127.
 * @param y the ordinate of the character. Should between 0 and 7.
 * @param chr the character
 * @param Char_Size the size of the character. Should be 16 or 12.
 */
void OLED_ShowChar(uint8_t x, uint8_t y, uint8_t chr, uint8_t Char_Size);

/**
 * @brief display a string at the place on the OLED.
 *
 * @param x the abscissa of the character. Should between 0 and 127.
 * @param y the ordinate of the character. Should between 0 and 7.
 * @param chr the pointer of the char array
 * @param Char_Size the size of the character. Should be 16 or 12.
 */
void OLED_ShowString(uint8_t x, uint8_t y, uint8_t *chr, uint8_t Char_Size);

/**
 * @brief display a Chinese character at the place on the OLED.
 * the Chinese character should be build by a character matrix software
 * like PCtoLCD. and it should be stored in the hzk array.
 *
 * @param x the abscissa of the character. Should between 0 and 127.
 * @param y the ordinate of the character. Should between 0 and 7.
 * @param no the first coordinate of the Chinese character in hzk.
 */
void OLED_ShowChinese(uint8_t x, uint8_t y, uint8_t no);

void OLED_ShowBigNum(uint8_t x, uint8_t y, uint8_t num);

#endif
