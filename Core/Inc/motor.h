#ifndef MAX32660_I2C_MOTOR_H
#define MAX32660_I2C_MOTOR_H

#define MOTOR_PIN  PIN_6
#define MOTOR_PORT PORT_0

/**
 * @brief Init the linear motor
 */
void Motor_Init();

/**
 * @brief have a motor click
 */
void Motor_Click();

#endif //MAX32660_I2C_MOTOR_H
