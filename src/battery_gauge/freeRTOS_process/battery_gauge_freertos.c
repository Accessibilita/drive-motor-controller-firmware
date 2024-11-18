#include "battery_gauge_freertos.h"
#include "FreeRTOS.h"     // FreeRTOS API
#include "task.h"         // Task management
#include "semphr.h"       // Semaphore for synchronization

// Static variable to store the current battery state data
// Accessible only within this file
static BatteryData battery_data = {0};

// Semaphore handle for controlling thread-safe access to battery_data
static SemaphoreHandle_t data_mutex = NULL;

// Placeholder functions for sensor data acquisition
// These functions must be implemented to interface with the actual hardware
extern float ReadCurrentSensor(void); // Function to read current from sensor (in Amperes)
extern float ReadVoltageSensor(void); // Function to read voltage from sensor (in Volts)

// Function to initialize the battery gauge system
void BatteryGauge_Init(float total_capacity) {
    // Initialize all battery data fields with default values
    battery_data.current = 0.0f;          // Default current to 0 A
    battery_data.voltage = 0.0f;          // Default voltage to 0 V
    battery_data.state_of_charge = 100.0f; // Start with a fully charged battery (100% SoC)
    battery_data.charge_used = 0.0f;      // No charge has been used initially
    battery_data.total_capacity = total_capacity; // Set total battery capacity

    // Create a mutex to protect access to the battery_data structure
    data_mutex = xSemaphoreCreateMutex();
    if (data_mutex == NULL) {
        // If mutex creation fails, halt execution (critical error)
        // This should be replaced with proper error handling in production
        while (1);
    }
}

// Thread-safe function to retrieve the latest battery data
// This function copies the battery_data into the provided pointer
void BatteryGauge_GetData(BatteryData *data) {
    if (data != NULL) { // Ensure the pointer is valid
        // Wait indefinitely until the mutex is available
        if (xSemaphoreTake(data_mutex, portMAX_DELAY) == pdTRUE) {
            *data = battery_data; // Copy the shared data to the caller's buffer
            xSemaphoreGive(data_mutex); // Release the mutex after access
        }
    }
}

// FreeRTOS task responsible for updating battery status
// This task runs in a periodic loop, reading sensors and updating battery metrics
void BatteryGauge_Task(void *pvParameters) {
    // Define the delay period for the task loop in milliseconds
    const TickType_t xDelay = pdMS_TO_TICKS(1000); // 1000 ms (1 second)

    // Infinite loop for the task
    while (1) {
        float current, voltage;

        // Step 1: Read data from sensors
        // Replace these calls with actual hardware interface code
        current = ReadCurrentSensor(); // Read current sensor data in Amperes
        voltage = ReadVoltageSensor(); // Read voltage sensor data in Volts

        // Step 2: Update battery data with thread-safe access
        // Lock the mutex to safely update shared battery_data
        if (xSemaphoreTake(data_mutex, portMAX_DELAY) == pdTRUE) {
            // Update the current reading
            battery_data.current = current;

            // Integrate charge used based on the current reading
            // Formula: charge_used += (current * delta_time) / 3600
            // delta_time is 1 second (loop period)
            battery_data.charge_used += (current * 1.0f) / 3600.0f;

            // Update the voltage reading
            battery_data.voltage = voltage;

            // Calculate the remaining battery capacity
            float remaining_capacity = battery_data.total_capacity - battery_data.charge_used;

            // Calculate the state of charge (SoC) as a percentage
            battery_data.state_of_charge = (remaining_capacity / battery_data.total_capacity) * 100.0f;

            // Ensure that SoC is bounded between 0% and 100%
            if (battery_data.state_of_charge > 100.0f) {
                battery_data.state_of_charge = 100.0f; // Cap at 100%
            } else if (battery_data.state_of_charge < 0.0f) {
                battery_data.state_of_charge = 0.0f; // Floor at 0%
            }

            // Release the mutex after updating shared data
            xSemaphoreGive(data_mutex);
        }

        // Step 3: Delay the task for the specified period
        // This ensures the task runs at a predictable rate (1 Hz in this case)
        vTaskDelay(xDelay);
    }
}
