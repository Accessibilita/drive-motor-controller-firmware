#include "battery_gauge_process.h"
#include "stc3100.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include <stdio.h>

// Static instance of battery data for shared use by tasks.
static BatteryData_t battery_data;

// Mutex for protecting access to the shared battery data.
static SemaphoreHandle_t data_mutex;

/**
 * @brief Initializes the FreeRTOS battery gauge process.
 *        Creates the task and initializes synchronization primitives.
 */
void BatteryGaugeProcess_Init(void) {
    // Initialize the battery data structure with default values.
    battery_data.voltage = 0.0f;
    battery_data.current = 0.0f;
    battery_data.charge_used = 0.0f;
    battery_data.state_of_charge = 100.0f; // Assume fully charged.
    battery_data.total_capacity = 2000.0f; // Example capacity: 2000 mAh.

    // Create a mutex for thread-safe access to battery data.
    data_mutex = xSemaphoreCreateMutex();
    if (data_mutex == NULL) {
        printf("Failed to create mutex for battery data.\n");
    }

    // Create the FreeRTOS task for battery data management.
    BaseType_t task_created = xTaskCreate(
        BatteryGaugeProcess_Task,  // Task function.
        "BatteryGaugeTask",        // Name for debugging.
        512,                       // Stack size in words.
        NULL,                      // Task parameter (not used).
        2,                         // Task priority.
        NULL                       // Task handle (not used).
    );

    if (task_created != pdPASS) {
        printf("Failed to create battery gauge task.\n");
    }
}

/**
 * @brief FreeRTOS task for periodic updates of battery metrics.
 *        Reads data from the STC3100 and updates shared battery data.
 * @param argument Task parameter (unused in this implementation, set to NULL).
 */
void BatteryGaugeProcess_Task(void *argument) {
    TickType_t last_wake_time = xTaskGetTickCount();

    // Task loop for periodic updates.
    for (;;) {
        float voltage, current, charge;

        // Read sensor data from the STC3100.
        voltage = STC3100_ReadVoltage();
        current = STC3100_ReadCurrent();
        charge = STC3100_ReadCharge();

        // Validate sensor readings.
        if (voltage < 0 || current < 0 || charge < 0) {
            printf("Error reading STC3100 data.\n");
            continue;
        }

        // Acquire mutex for thread-safe access to shared data.
        if (xSemaphoreTake(data_mutex, pdMS_TO_TICKS(100)) == pdTRUE) {
            // Update shared battery data with the latest readings.
            battery_data.voltage = voltage;
            battery_data.current = current;
            battery_data.charge_used = charge;

            // Calculate state of charge (SoC).
            battery_data.state_of_charge =
                ((battery_data.total_capacity - charge) / battery_data.total_capacity) * 100.0f;

            // Clamp SoC to valid range [0%, 100%].
            if (battery_data.state_of_charge > 100.0f) {
                battery_data.state_of_charge = 100.0f;
            } else if (battery_data.state_of_charge < 0.0f) {
                battery_data.state_of_charge = 0.0f;
            }

            // Release the mutex after updating.
            xSemaphoreGive(data_mutex);
        }

        // Delay the task for 1 second to maintain periodic execution.
        vTaskDelayUntil(&last_wake_time, pdMS_TO_TICKS(1000));
    }
}

/**
 * @brief Retrieves the latest battery data in a thread-safe manner.
 * @param data Pointer to a BatteryData_t structure to receive the data.
 * @retval true if data retrieval was successful, false otherwise.
 */
bool BatteryGaugeProcess_GetData(BatteryData_t *data) {
    // Attempt to acquire the mutex to access shared data.
    if (xSemaphoreTake(data_mutex, pdMS_TO_TICKS(100)) == pdTRUE) {
        *data = battery_data; // Copy data to the provided structure.
        xSemaphoreGive(data_mutex); // Release the mutex after access.
        return true; // Successfully retrieved data.
    }

    return false; // Failed to acquire mutex.
}
