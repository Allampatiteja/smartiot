#include <WiFi.h>
#include <WebServer.h>
#include "DHT.h"

// Hardware Configuration
#define DHTPIN 14
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);
WebServer server(80);

const char* ssid = "Wokwi-GUEST";
const char* password = "";

// --- History storage logic ---
const int MAX_READINGS = 20;
float tempHistory[MAX_READINGS];
float humHistory[MAX_READINGS];
int readingCount = 0;

void addReading(float t, float h) {
  // Shift old data to make room for new data
  for (int i = MAX_READINGS - 1; i > 0; i--) {
    tempHistory[i] = tempHistory[i - 1];
    humHistory[i] = humHistory[i - 1];
  }
  // Insert newest reading at index 0
  tempHistory[0] = t;
  humHistory[0] = h;
  if (readingCount < MAX_READINGS) readingCount++;
}

// --- Web Page Generator ---
void handleRoot() {
  float t = dht.readTemperature();
  float h = dht.readHumidity();

  // 1. Determine Status and Colors
  String bgColor = "#2ecc71"; // Default Green (Normal)
  String statusMsg = "Normal Temperature";

  if (isnan(t) || isnan(h)) {
    bgColor = "#95a5a6"; // Gray (Error)
    statusMsg = "Sensor Error";
  } else {
    addReading(t, h); // Log valid data into history
    if (t > 35.0) {
      bgColor = "#e74c3c"; // Red (Hot)
      statusMsg = "HOT ALERT!";
    } else if (t < 10.0) {
      bgColor = "#3498db"; // Blue (Cold)
      statusMsg = "Cold Warning";
    }
  }

  // 2. Build HTML
  String html = "<html><head><meta http-equiv='refresh' content='3'>";
  html += "<style>";
  html += "body { font-family: sans-serif; text-align: center; background-color: " + bgColor + "; transition: background 0.5s; margin: 0; padding: 20px; }";
  html += ".card { background: white; padding: 20px; border-radius: 15px; display: inline-block; box-shadow: 0 10px 20px rgba(0,0,0,0.2); min-width: 320px; margin-bottom: 20px; }";
  html += ".status { font-weight: bold; text-transform: uppercase; color: " + bgColor + "; font-size: 1.2em; margin: 10px 0; }";
  html += "table { margin: auto; border-collapse: collapse; width: 100%; font-size: 0.9em; }";
  html += "th, td { border: 1px solid #ddd; padding: 10px; }";
  html += "th { background: #34495e; color: white; }";
  html += "tr:nth-child(even) { background-color: #f9f9f9; }";
  html += "</style></head><body>";
  
  // Current Reading Section
  html += "<div class='card'><h1>ESP32 Weather Station</h1>";
  html += "<p class='status'>" + statusMsg + "</p>";
  if (!isnan(t)) {
    html += "<h2>" + String(t, 1) + " &deg;C</h2>";
    html += "<h3>" + String(h, 1) + " % Humidity</h3>";
  } else {
    html += "<h2>Waiting for sensor...</h2>";
  }
  html += "</div><br>";

  // History Table Section
  html += "<div class='card'><h3>Last 20 Readings</h3><table>";
  html += "<tr><th>#</th><th>Temperature</th><th>Humidity</th></tr>";
  for (int i = 0; i < readingCount; i++) {
    html += "<tr><td>" + String(i + 1) + "</td>";
    html += "<td align=center>" + String(tempHistory[i], 1) + " &deg;C</td>";
    html += "<td align=center>" + String(humHistory[i], 1) + "%</td></tr>";
  }
  html += "</table></div></body></html>";
  
  server.send(200, "text/html", html);
}

// --- Main Arduino Entry Points ---
void setup() {
  Serial.begin(115200);
  dht.begin();
  
  // Initialize WiFi for Wokwi
  WiFi.begin(ssid, password, 6);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  // Start Server Routes
  server.on("/", handleRoot);
  server.begin();
  Serial.println("\nHTTP server started");
}

void loop() {
  server.handleClient();
}