import serial
import time
import matplotlib.pyplot as plt
import numpy as np
import math

port = 'COM3'
baud = 115200
V_s = 5.0

ser = serial.Serial(port, baud, timeout=1)
time.sleep(2)

resistor_index = input("Enter resistor index (0 = 10Ω to 4 = 100kΩ): ")
ser.write((resistor_index + '\n').encode())
resistor_index = int(resistor_index)

resistor_values = {0: 10, 1: 100, 2: 1000, 3: 10000, 4: 100000}
R = resistor_values.get(resistor_index, 1000)

time_data = []
charge_data = []
discharge_data = []

print("Collecting data...\n")

while True:
    line = ser.readline().decode('utf-8').strip()
    if "Done." in line:
        break
    if ',' in line and line[0].isdigit():
        t, v_charge, v_discharge = line.split(',')
        time_data.append(int(t))
        charge_data.append(float(v_charge))
        discharge_data.append(float(v_discharge))

ser.close()
time_ms = np.array(time_data)
time_s = time_ms / 1000.0
charge_v = np.array(charge_data)
discharge_v = np.array(discharge_data)

target_time = 0.1
index_100ms = np.argmin(np.abs(time_s - target_time))

V0_discharge = discharge_v[0]
Vt_discharge = discharge_v[index_100ms]
t_discharge = time_s[index_100ms]

Vt_charge = charge_v[index_100ms]
t_charge = time_s[index_100ms]

try:
    C_discharge = -t_discharge / (R * math.log(Vt_discharge / V0_discharge))
    C_charge = -t_charge / (R * math.log(1 - Vt_charge / V_s))

    print(f"Capacitance from charging:   {C_charge * 1e6:.2f} µF")
    print(f"Capacitance from discharging: {C_discharge * 1e6:.2f} µF")


plt.figure(figsize=(10, 5))
plt.plot(time_ms, charge_v, label='Charge (V)', color='blue')
plt.plot(time_ms, discharge_v, label='Discharge (V)', color='orange')
plt.xlabel("Time (ms)")
plt.ylabel("Voltage (V)")
plt.title("RC Charging and Discharging")
plt.ylim(0, 5.2)
plt.grid(True)
plt.legend()
plt.tight_layout()
plt.show()