#ifndef BATTERY_GAUGE_PROCESS_H
#define BATTERY_GAUGE_PROCESS_H

#include <stdbool.h>
#include "battery_gauge.h"

/**
 * @brief Initializes the FreeRTOS process for the battery gauge.
 *        This includes creating tasks and preparing resources.
 */
void BatteryGaugeProcess_Init(void);

/**
 * @brief FreeRTOS task to periodically update battery metrics.
 *        Reads data from the STC3100 and updates the shared battery data structure.
 * @param argument Task parameter (unused in this implementation, set to NULL).
 */
void BatteryGaugeProcess_Task(void *argument);

/**
 * @brief Retrieves the latest battery data from the FreeRTOS process.
 * @param data Pointer to a BatteryData_t structure to receive the data.
 * @retval true if data retrieval was successful, false otherwise.
 */
bool BatteryGaugeProcess_GetData(BatteryData_t *data);

#endif // BATTERY_GAUGE_PROCESS_H
