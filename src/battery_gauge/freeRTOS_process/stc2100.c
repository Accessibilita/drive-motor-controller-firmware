#include "stc3100.h"
#include <stdio.h>

// Define I2C address of the STC3100 IC.
#define STC3100_I2C_ADDRESS 0x70

// Example I2C handle for STM32 (replace with your actual I2C handle).
extern I2C_HandleTypeDef hi2c1;

/**
 * @brief Initializes the STC3100 IC for operation.
 * @retval HAL_OK if initialization succeeded, otherwise HAL_ERROR.
 */
HAL_StatusTypeDef STC3100_Init(void) {
    // Send initialization sequence or configuration settings to the STC3100.
    // Implementation depends on the datasheet specifications.
    return HAL_OK; // Example assumes successful initialization.
}

/**
 * @brief Reads the current voltage from the STC3100 IC.
 * @retval Voltage in volts (V) if successful, otherwise a negative value to indicate an error.
 */
float STC3100_ReadVoltage(void) {
    uint8_t voltage_data[2];
    if (HAL_I2C_Mem_Read(&hi2c1, STC3100_I2C_ADDRESS, 0x02, I2C_MEMADD_SIZE_8BIT, voltage_data, 2, HAL_MAX_DELAY) != HAL_OK) {
        return -1.0f; // Error occurred.
    }
    return ((voltage_data[0] << 8) | voltage_data[1]) * 0.00244f; // Convert to volts.
}

/**
 * @brief Reads the current from the STC3100 IC.
 * @retval Current in amperes (A) if successful, otherwise a negative value to indicate an error.
 */
float STC3100_ReadCurrent(void) {
    uint8_t current_data[2];
    if (HAL_I2C_Mem_Read(&hi2c1, STC3100_I2C_ADDRESS, 0x04, I2C_MEMADD_SIZE_8BIT, current_data, 2, HAL_MAX_DELAY) != HAL_OK) {
        return -1.0f; // Error occurred.
    }
    return ((current_data[0] << 8) | current_data[1]) * 0.001f; // Convert to amperes.
}

/**
 * @brief Reads the accumulated charge used from the STC3100 IC.
 * @retval Charge in milliamp-hours (mAh) if successful, otherwise a negative value to indicate an error.
 */
float STC3100_ReadCharge(void) {
    uint8_t charge_data[2];
    if (HAL_I2C_Mem_Read(&hi2c1, STC3100_I2C_ADDRESS, 0x06, I2C_MEMADD_SIZE_8BIT, charge_data, 2, HAL_MAX_DELAY) != HAL_OK) {
        return -1.0f; // Error occurred.
    }
    return ((charge_data[0] << 8) | charge_data[1]) * 0.01f; // Convert to mAh.
}
