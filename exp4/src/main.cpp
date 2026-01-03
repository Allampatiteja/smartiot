#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <Adafruit_FT6206.h>

// TFT pins
#define TFT_DC 2
#define TFT_CS 15

// UI settings
#define BOXSIZE 40
#define PENRADIUS 3

Adafruit_ILI9341 tft(TFT_CS, TFT_DC);
Adafruit_FT6206 ctp;

int oldcolor, currentcolor;

void setup() {
  Serial.begin(115200);
  Serial.println("Cap Touch Paint!");

  // I2C for FT6206
  Wire.setPins(10, 8);
  Wire.begin();

  tft.begin();

  if (!ctp.begin(40)) {
    Serial.println("Couldn't start FT6206 touchscreen controller");
    while (1);
  }

  Serial.println("Capacitive touchscreen started");

  tft.fillScreen(ILI9341_BLACK);

  // Color palette
  tft.fillRect(0, 0, BOXSIZE, BOXSIZE, ILI9341_RED);
  tft.fillRect(BOXSIZE, 0, BOXSIZE, BOXSIZE, ILI9341_YELLOW);
  tft.fillRect(BOXSIZE * 2, 0, BOXSIZE, BOXSIZE, ILI9341_GREEN);
  tft.fillRect(BOXSIZE * 3, 0, BOXSIZE, BOXSIZE, ILI9341_CYAN);
  tft.fillRect(BOXSIZE * 4, 0, BOXSIZE, BOXSIZE, ILI9341_BLUE);
  tft.fillRect(BOXSIZE * 5, 0, BOXSIZE, BOXSIZE, ILI9341_MAGENTA);

  // Default color = RED
  tft.drawRect(0, 0, BOXSIZE, BOXSIZE, ILI9341_WHITE);
  currentcolor = ILI9341_RED;
}

void loop() {
  if (!ctp.touched()) return;

  TS_Point p = ctp.getPoint();

  // Map touch to screen coordinates
  p.x = map(p.x, 0, 240, 240, 0);
  p.y = map(p.y, 0, 320, 320, 0);

  // Clean structured serial output
  Serial.print("(");
  Serial.print(p.x);
  Serial.print(", ");
  Serial.print(p.y);
  Serial.println(")");

  // Color selection area
  if (p.y < BOXSIZE) {
    oldcolor = currentcolor;

    if (p.x < BOXSIZE) currentcolor = ILI9341_RED;
    else if (p.x < BOXSIZE * 2) currentcolor = ILI9341_YELLOW;
    else if (p.x < BOXSIZE * 3) currentcolor = ILI9341_GREEN;
    else if (p.x < BOXSIZE * 4) currentcolor = ILI9341_CYAN;
    else if (p.x < BOXSIZE * 5) currentcolor = ILI9341_BLUE;
    else currentcolor = ILI9341_MAGENTA;

    if (oldcolor != currentcolor) {
      // Redraw palette
      tft.fillRect(0, 0, BOXSIZE * 6, BOXSIZE, ILI9341_BLACK);
      tft.fillRect(0, 0, BOXSIZE, BOXSIZE, ILI9341_RED);
      tft.fillRect(BOXSIZE, 0, BOXSIZE, BOXSIZE, ILI9341_YELLOW);
      tft.fillRect(BOXSIZE * 2, 0, BOXSIZE, BOXSIZE, ILI9341_GREEN);
      tft.fillRect(BOXSIZE * 3, 0, BOXSIZE, BOXSIZE, ILI9341_CYAN);
      tft.fillRect(BOXSIZE * 4, 0, BOXSIZE, BOXSIZE, ILI9341_BLUE);
      tft.fillRect(BOXSIZE * 5, 0, BOXSIZE, BOXSIZE, ILI9341_MAGENTA);

      int index = p.x / BOXSIZE;
      tft.drawRect(index * BOXSIZE, 0, BOXSIZE, BOXSIZE, ILI9341_WHITE);
    }
  }

  // Draw area
  if (p.y > BOXSIZE && p.y < tft.height()) {
    tft.fillCircle(p.x, p.y, PENRADIUS, currentcolor);
  }

  // Prevent serial flooding
  delay(50);
}