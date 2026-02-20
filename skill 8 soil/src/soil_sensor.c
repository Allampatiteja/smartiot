#include <Arduino.h>
#include <HardwareSerial.h>
#include "soil_sensor.h"

#define SOIL_PIN 34   // ESP32 ADC pin

void setup() {
    delay(1000);
    Serial.begin(115200);
    while (!Serial) {
        delay(10);
    }
}

void loop() {
    int moisture = readSoilMoisture(SOIL_PIN);
    Serial.print("Soil Moisture: ");
    Serial.println(moisture);
    delay(1000);
}
int readSoilMoisture(int pin) {
    return analogRead(pin);
}