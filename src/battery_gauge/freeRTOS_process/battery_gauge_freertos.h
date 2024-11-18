#ifndef BATTERY_GAUGE_FREERTOS_H
#define BATTERY_GAUGE_FREERTOS_H

#include <stdint.h>

// Data structure to hold the battery status information
typedef struct {
    float current;        // Current reading in Amperes (A)
    float voltage;        // Voltage reading in Volts (V)
    float state_of_charge; // State of Charge (SoC) as a percentage (0% to 100%)
    float charge_used;    // Accumulated charge used in Ampere-hours (Ah)
    float total_capacity; // Total battery capacity in Ampere-hours (Ah)
} BatteryData;

// Function to initialize the battery gauge system
// This sets up initial parameters like total battery capacity and creates synchronization primitives.
void BatteryGauge_Init(float total_capacity);

// Function to retrieve the latest battery data
// This function is thread-safe and ensures consistent data retrieval using a mutex.
void BatteryGauge_GetData(BatteryData *data);

// FreeRTOS task function responsible for periodically reading sensors, 
// updating battery state, and performing calculations like SoC.
void BatteryGauge_Task(void *pvParameters);

#endif // BATTERY_GAUGE_FREERTOS_H
