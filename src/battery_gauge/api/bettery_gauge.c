#include "battery_gauge.h"

// Static data for battery state
static BatteryData battery_data = {0};

// Initialize the battery gauge system
void BatteryGauge_Init(float total_capacity) {
    battery_data.current = 0.0f;
    battery_data.voltage = 0.0f;
    battery_data.state_of_charge = 100.0f; // Start at 100% charge
    battery_data.charge_used = 0.0f;
    battery_data.total_capacity = total_capacity;
}

// Update the current reading and calculate charge used (Coulomb Counting)
void BatteryGauge_UpdateCurrent(float current, float delta_time) {
    battery_data.current = current;
    battery_data.charge_used += (current * delta_time) / 3600.0f; // Convert seconds to hours
}

// Update the voltage reading
void BatteryGauge_UpdateVoltage(float voltage) {
    battery_data.voltage = voltage;
}

// Compute the State of Charge (SoC)
void BatteryGauge_ComputeSoC(void) {
    float remaining_capacity = battery_data.total_capacity - battery_data.charge_used;
    battery_data.state_of_charge = (remaining_capacity / battery_data.total_capacity) * 100.0f;

    // Ensure SoC is within bounds
    if (battery_data.state_of_charge > 100.0f) {
        battery_data.state_of_charge = 100.0f;
    } else if (battery_data.state_of_charge < 0.0f) {
        battery_data.state_of_charge = 0.0f;
    }
}

// Retrieve all battery data
void BatteryGauge_GetData(BatteryData *data) {
    if (data != NULL) {
        *data = battery_data;
    }
}
