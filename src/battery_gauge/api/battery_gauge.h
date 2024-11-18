#ifndef BATTERY_GAUGE_H
#define BATTERY_GAUGE_H

#include <stdint.h>

// Struct to hold battery data
typedef struct {
    float current;       // Current in Amperes
    float voltage;       // Voltage in Volts
    float state_of_charge; // State of Charge in percentage
    float charge_used;   // Charge used in Ah
    float total_capacity; // Battery capacity in Ah
} BatteryData;

// Function Prototypes
void BatteryGauge_Init(float total_capacity);
void BatteryGauge_UpdateCurrent(float current, float delta_time);
void BatteryGauge_UpdateVoltage(float voltage);
void BatteryGauge_ComputeSoC(void);
void BatteryGauge_GetData(BatteryData *data);

#endif // BATTERY_GAUGE_H
