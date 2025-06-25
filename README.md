# RC_Visualizer

This project demonstrates the charging and discharging behavior of an RC (resistor-capacitor) circuit using an Arduino Uno and Python. It allows users to visualize voltage over time and estimate the capacitance of a circuit from experimental data.

## Overview

The system consists of two main components:

- An **Arduino sketch** (`chargePin.ino`) that controls the charging/discharging process and samples voltage across the capacitor.
- A **Python script** (`plotCharge.py`) that receives data over serial, plots voltage vs. time, and computes an estimated capacitance using exponential equations.

The project was created to provide a hands-on understanding of the exponential voltage curves in RC circuits, and how resistance values affect the rate of charging and discharging.

**Note:**

The results for 10 Ω and 100 Ω resistors show poor accuracy due to hardware limitations. At low resistance values, the RC time constant (τ = RC) becomes very small — on the order of milliseconds or less. The Arduino Uno's ADC sampling rate is not fast enough to capture sufficient data points during the rapid charge and discharge cycles, resulting in coarse, stepped plots and significant deviation in estimated capacitance. This limitation becomes less significant at higher resistor values, where slower charging allows for smoother, more accurate voltage sampling and curve fitting.

## How It Works

1. The user selects one of five resistor values (10 Ω to 100 kΩ) via serial input.
2. The Arduino charges a 100 µF capacitor through the selected resistor, samples voltage using its ADC, and then discharges it.
3. Voltage samples are streamed over serial to a connected Python script.
4. The Python script:
   - Plots voltage during both charging and discharging cycles.
   - Estimates capacitance using curve-fitting equations derived from the RC time constant.

## Files

| File             | Description                                                              |
|------------------|--------------------------------------------------------------------------|
| `chargePin.ino`  | Arduino sketch for controlling pins, sampling voltage, and sending data  |
| `plotCharge.py`  | Python script for serial communication, plotting, and capacitance fitting|

## Requirements

- Arduino Uno
- Capacitor: 100 µF
- Resistors: 10 Ω, 100 Ω, 1 kΩ, 10 kΩ, 100 kΩ
- Python 3.x with the following libraries:
- pip install pyserial matplotlib numpy

## Capacitance Calculation

The capacitance is calculated using rearranged versions of the standard RC charge/discharge equations:

**Discharging:**

V(t) = V0 * e^(-t / RC)
C = -t / (R * ln(V(t) / V0))

**Charging:**

V(t) = Vs * (1 - e^(-t / RC))
C = -t / (R * ln(1 - V(t)/Vs))

Where:
- `R` is the selected resistor value
- `V0` is the starting voltage
- `Vs` is the source voltage (5 V)
- `V(t)` is the voltage at time `t`

The script samples voltage data and calculates `C` using the voltage after 100 ms of charging or discharging.

## Example Results


Below are plots and console outputs showing RC charging and discharging curves and corresponding capacitance estimations for each resistor value.

---

### 10 Ω Resistor

**Console Output**

Capacitance from charging:    281.72 µF

Capacitance from discharging: 155.57 µF


**Plot**

![10](https://github.com/user-attachments/assets/2291eae9-79d1-4202-9dfd-1f48abdec8b8)


---

### 100 Ω Resistor

**Console Output**

Capacitance from charging:    105.42 µF

Capacitance from discharging: 82.84 µF


**Plot**

![100](https://github.com/user-attachments/assets/e3c885ef-e9f3-4767-8b7f-12cde601f27f)


---

### 1 kΩ Resistor

**Console Output**

Capacitance from charging:    125.18 µF

Capacitance from discharging: 126.10 µF


**Plot**

![1k](https://github.com/user-attachments/assets/01a00c1c-c22b-4abd-8ece-5cdbf1701daf)


---

### 10 kΩ Resistor

**Console Output**

Capacitance from charging:    109.03 µF

Capacitance from discharging: 112.66 µF


**Plot**

![10k](https://github.com/user-attachments/assets/fff34bc0-4b35-4c7a-aee1-0d7a70c9a752)


---

### 100 kΩ Resistor

**Console Output**

Capacitance from charging:    103.75 µF

Capacitance from discharging: 101.57 µF


**Plot**

![100k](https://github.com/user-attachments/assets/d088313a-edc0-48fb-88d7-c60aec256796)


## Future Improvements

- Support for multiple capacitors
- GUI for resistor selection
- Implementation using STM32 or ESP32 based microcontroller for higher ADC resolution and faster sampling for improved accuracy at low time constants

## License

This project is released under the MIT License.
