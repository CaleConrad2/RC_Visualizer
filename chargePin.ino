const int resistorPins[] = {6, 5, 4, 3, 2};
const int analogPins[] = {A0, A1, A2, A3, A4};

const unsigned long maxChargeTime_ms[] = {200, 500, 3000, 20000, 70000};
const unsigned long maxDischargeTime_us = 10000000;
const unsigned long samplePeriod_us = 50;
const int sampleLimit = 200;

int16_t chargeData[sampleLimit];
int16_t dischargeData[sampleLimit];

void setup() {
  Serial.begin(115200);
  Serial.println("Enter resistor index (0 = 10Ω to 4 = 100kΩ):");
  for (int i = 0; i < 5; i++) pinMode(resistorPins[i], INPUT);
}

void loop() {
  if (Serial.available()) {
    int index = Serial.parseInt();
    if (index < 0 || index > 4) {
      Serial.println("Invalid index. Enter 0–4:");
      return;
    }

    while (Serial.available()) Serial.read();
    int skipFactor = 1;
    if (index == 2) skipFactor = 10;
    if (index == 3) skipFactor = 100;
    if (index == 4) skipFactor = 1000;

    for (int i = 0; i < sampleLimit; i++) {
      chargeData[i] = 0;
      dischargeData[i] = 0;
    }

    for (int i = 0; i < 5; i++) {
      if (i == index) {
        pinMode(resistorPins[i], OUTPUT);
        digitalWrite(resistorPins[i], LOW);
      } else {
        pinMode(resistorPins[i], INPUT);
      }
    }

    float voltage = 5.0;
    while (voltage > 0.2) {
      int raw = analogRead(analogPins[index]);
      voltage = raw * (5.0 / 1023.0);
      delay(10);
    }

    digitalWrite(resistorPins[index], HIGH);
    unsigned long startMicros = micros();
    unsigned long elapsed_us = 0;
    int totalSamples = 0, chargeSamples = 0;

    while (elapsed_us < maxChargeTime_ms[index] * 1000 && chargeSamples < sampleLimit) {
      int raw = analogRead(analogPins[index]);
      if (totalSamples % skipFactor == 0) {
        chargeData[chargeSamples++] = raw;
      }
      totalSamples++;
      elapsed_us += samplePeriod_us;
      while (micros() - startMicros < elapsed_us);
    }

    delay(3000);

    digitalWrite(resistorPins[index], LOW);
    delay(2);
    startMicros = micros();
    elapsed_us = 0;
    totalSamples = 0;
    int dischargeSamples = 0;

    while (elapsed_us < maxDischargeTime_us && dischargeSamples < sampleLimit) {
      int raw = analogRead(analogPins[index]);
      if (totalSamples % skipFactor == 0) {
        dischargeData[dischargeSamples++] = raw;
      }
      totalSamples++;
      elapsed_us += samplePeriod_us;
      delayMicroseconds(samplePeriod_us);
    }

    pinMode(resistorPins[index], INPUT);

    Serial.println("time_ms,charge_V,discharge_V");
    int maxSamples = max(chargeSamples, dischargeSamples);
    for (int i = 0; i < maxSamples; i++) {
      unsigned long time_ms = (i * samplePeriod_us * skipFactor) / 1000;

      float v_charge = 0;
      if (i < chargeSamples) {
       v_charge = chargeData[i] * (5.0 / 1023);
      }

      float v_discharge = 0;
      if (i < dischargeSamples) {
        v_discharge = dischargeData[i] * (5.0 / 1023);
      }

      Serial.print(time_ms);
      Serial.print(",");
      Serial.print(v_charge, 3);
      Serial.print(",");
      Serial.println(v_discharge, 3);

      delayMicroseconds(300); 
    }

    Serial.println("Done. Enter another resistor index (0–4):");
  }
}