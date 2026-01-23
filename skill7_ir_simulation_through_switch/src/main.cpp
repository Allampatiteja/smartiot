#include <Arduino.h>

#define BUTTON_PIN 25  
#define LED_PIN 2  

int led_state = LOW;
int button_state;
int last_button_state;

void setup() {
    Serial.begin(115200);
    Serial.println("Hello, ESP32!");
    pinMode(LED_PIN, OUTPUT);
    pinMode(BUTTON_PIN, INPUT_PULLUP);
    button_state = digitalRead(BUTTON_PIN);
}

void loop() {
    last_button_state = button_state;
    button_state = digitalRead(BUTTON_PIN);
    delay(10);
    
    if (button_state != last_button_state && button_state == LOW) {
        Serial.println("Object Detected");
        led_state = HIGH;
        digitalWrite(LED_PIN, led_state);
    } else if (button_state != last_button_state && button_state == HIGH) {
        Serial.println("No Object Detected");
        led_state = LOW;
        digitalWrite(LED_PIN, led_state);
    }
    
    delay(1000);
}
