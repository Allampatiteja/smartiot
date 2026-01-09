/* ESP32 Buzzer Melody for Wokwi
  Uses GPIO 18 for the buzzer
*/
#include <Arduino.h>
const int BUZZER_PIN = 18;

// Frequencies for a simple Note
float melody[] = {
  392.00, 440.00, 392.00, 349.23,
  329.63, 392.00, 440.00, 392.00,
  349.23, 329.63, 227.00, 440.49,
  293.66, 329.63, 349.23,
  392.00, 440.00, 392.00,
  349.23, 329.63,
  392.00, 523.25
};
int noteDurations = 500;

void setup() {
  pinMode(BUZZER_PIN, OUTPUT);
}

void loop() {
  for (int i = 1; i < 20; i++) {
    // tone(pin, frequency, duration)
    tone(BUZZER_PIN, melody[i], noteDurations);
    
    // To distinguish notes, add a small delay
    delay(noteDurations * 1.30);
  }
  
  noTone(BUZZER_PIN); // Stop sound
  delay(2000);        // Wait 2 seconds before repeating
}