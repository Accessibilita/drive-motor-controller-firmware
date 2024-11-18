# Detailed Write-Up: Battery Gauge System Design

---

### **Overview**
The battery gauge system measures the state of a LiFePO4 battery by continuously monitoring **current**, **voltage**, and **charge usage** using sensors. The system integrates these measurements with an STM32 microcontroller running FreeRTOS to estimate key battery metrics like **State of Charge (SoC)**, **charge consumed**, and **remaining capacity**. This design ensures real-time updates and thread-safe access in a multitasking environment.

---

### **System Components**

1. **Hardware Sensors**
   - **Current Sensor**:
     - Measures the instantaneous current flowing into or out of the battery in amperes (A).
     - Provides a key input for calculating the charge usage via Coulomb counting.
   - **Voltage Sensor**:
     - Measures the battery’s terminal voltage in volts (V).
     - Helps verify battery charge levels and health.
   - **Coulomb Counter**:
     - Measures the total charge transferred over time by integrating the current readings.

2. **STM32F446RET6 Microcontroller**
   - Collects data from the sensors.
   - Performs computations such as:
     - Charge accumulation (via Coulomb counting).
     - SoC estimation.
     - Data management.
   - Runs FreeRTOS for multitasking and thread-safe operations.

3. **FreeRTOS**
   - Ensures periodic polling of sensors via a dedicated task.
   - Uses semaphores for synchronization and safe access to shared battery data.

4. **Software Components**
   - **Battery Data Struct**:
     - Centralized structure for holding current battery metrics:
       - Current (`current`)
       - Voltage (`voltage`)
       - State of Charge (`state_of_charge`)
       - Charge Used (`charge_used`)
       - Total Capacity (`total_capacity`)
   - **Update and Compute Algorithms**:
     - Update sensor readings and integrate current to calculate charge usage.
     - Estimate SoC based on the relationship between charge used and total capacity.

---

### **Key Algorithms**

1. **Coulomb Counting**:
   - This algorithm estimates the charge transferred to/from the battery over time:
     \[
     \text{Charge Used (Ah)} += \frac{\text{Current (A)} \times \Delta t}{3600}
     \]
   - **Key Points**:
     - The current is integrated over time (\( \Delta t \)), converting seconds into hours for Ampere-hours (Ah).
     - Positive current indicates charging; negative current indicates discharging.

2. **State of Charge (SoC) Estimation**:
   - SoC reflects the percentage of the battery's capacity that remains:
     \[
     \text{State of Charge (\%)} = \left( \frac{\text{Remaining Capacity (Ah)}}{\text{Total Capacity (Ah)}} \right) \times 100
     \]
   - Remaining capacity is calculated as:
     \[
     \text{Remaining Capacity (Ah)} = \text{Total Capacity (Ah)} - \text{Charge Used (Ah)}
     \]
   - **Boundary Conditions**:
     - SoC is clamped between 0% (fully discharged) and 100% (fully charged).

3. **Voltage Validation**:
   - Voltage readings validate the SoC estimate:
     - Fully charged LiFePO4 batteries typically have a voltage near 3.65V per cell.
     - Fully discharged voltage drops to around 2.5V per cell.

---

### **System Workflow**

1. **Initialization**:
   - The `BatteryGauge_Init` function initializes the battery metrics:
     - `current`, `voltage`, `charge_used`, and `state_of_charge` are set to defaults.
     - The `data_mutex` semaphore is created to ensure safe access to shared data.

2. **Sensor Polling**:
   - The `BatteryGauge_Task` runs in a periodic loop (1-second interval):
     - Reads current and voltage values from sensors.
     - Updates the `current` and `voltage` fields in `battery_data`.

3. **Charge Usage Calculation**:
   - The task integrates the current reading over time (using Coulomb counting) to update the `charge_used` field.

4. **State of Charge Computation**:
   - The remaining capacity is derived from total capacity and charge used.
   - SoC is computed and stored in `battery_data`.

5. **Thread-Safe Data Access**:
   - Other tasks or functions retrieve the latest battery data using `BatteryGauge_GetData`, which locks the `data_mutex` to ensure consistent and accurate readings.

---

### **Code Components**

1. **`BatteryData` Structure**:
   - Holds all battery metrics for centralized access and updates.

2. **`BatteryGauge_Init` Function**:
   - Prepares the system by setting default values and creating synchronization primitives.

3. **`BatteryGauge_Task` Function**:
   - Periodically polls sensors, updates battery metrics, and computes derived values.

4. **`BatteryGauge_GetData` Function**:
   - Provides thread-safe access to battery metrics.

---

### **Advantages of the Design**

1. **Real-Time Monitoring**:
   - Ensures periodic and accurate updates to battery data.

2. **Thread-Safe Access**:
   - Uses mutexes to prevent race conditions in a multitasking environment.

3. **Flexibility**:
   - Modular design allows for easy integration of additional sensors or features.

4. **Accurate SoC Calculation**:
   - Combines Coulomb counting with voltage validation for precise SoC estimates.

5. **Scalability**:
   - Can be adapted to other battery chemistries or capacities by modifying constants.

---

### **Limitations and Improvements**

1. **Drift in Coulomb Counting**:
   - Integration errors over time can lead to drift. Periodic calibration using voltage measurements or external systems can improve accuracy.

2. **Temperature Considerations**:
   - Battery performance is temperature-dependent. Adding a temperature sensor could improve SoC accuracy under varying conditions.

3. **Low Power Optimization**:
   - Incorporate sleep modes to reduce energy consumption when idle.

---

### **Summary**

The battery gauge system is a robust, real-time monitoring solution for LiFePO4 batteries. It utilizes current and voltage sensors, combined with Coulomb counting and SoC algorithms, to provide detailed insights into battery usage and health. Running on FreeRTOS ensures efficient multitasking and thread safety, making this system ideal for embedded applications like renewable energy systems, electric vehicles, or portable devices.
