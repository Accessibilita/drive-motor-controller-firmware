#ifndef STC3100_H
#define STC3100_H

#include "stm32f4xx_hal.h" // Adjust for your STM32 series

// STC3100 I2C Address
#define STC3100_DEVICE_ADDRESS    (0x70 << 1) // Default 7-bit I2C address

// Register Addresses
#define STC3100_REG_MODE          0x00
#define STC3100_REG_VOLTAGE       0x08
#define STC3100_REG_CURRENT       0x0A
#define STC3100_REG_CHARGE        0x02

/**
 * @brief Initializes the STC3100 IC for operation.
 * @retval HAL_OK if initialization succeeded, otherwise HAL_ERROR.
 */
HAL_StatusTypeDef STC3100_Init(void);

/**
 * @brief Reads the current voltage from the STC3100 IC.
 * @retval Voltage in volts (V) if successful, otherwise a negative value to indicate an error.
 */
float STC3100_ReadVoltage(void);

/**
 * @brief Reads the current from the STC3100 IC.
 * @retval Current in amperes (A) if successful, otherwise a negative value to indicate an error.
 */
float STC3100_ReadCurrent(void);

/**
 * @brief Reads the accumulated charge used from the STC3100 IC.
 * @retval Charge in milliamp-hours (mAh) if successful, otherwise a negative value to indicate an error.
 */
float STC3100_ReadCharge(void);

#endif // STC3100_H
