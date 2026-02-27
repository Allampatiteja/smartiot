//Skill 15 main.cpp
#include <WiFi.h>
#include <WebServer.h>

// Replace with your network credentials
const char* ssid = "Wokwi-GUEST";
const char* password = "";
// Create an instance of the WebServer
WebServer server(80);

// Define the onboard LED pin
const int ledPin = 2; 
// HTML page content to control the LED with added CSS for modern design
const char* htmlContent = R"(
<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>ESP32 LED Control</title>
  <style>
    body {
      font-family: 'Arial', sans-serif;
      background-color: #f4f4f4;
      color: #333;
      text-align: center;
      padding: 50px;
    }
    h1 {
      color: #0078d4;
      font-size: 2.5em;
      margin-bottom: 30px;
    }
    .btn {
      background-color: #4CAF50;
      border: none;
      color: white;
      padding: 15px 32px;
      text-align: center;
      text-decoration: none;
      display: inline-block;
      font-size: 16px;
      margin: 10px;
      cursor: pointer;
      border-radius: 5px;
      transition: background-color 0.3s ease;
    }
    .btn:hover {
      background-color: #45a049;
    }
    .btn-off {
      background-color: #f44336;
    }
    .btn-off:hover {
      background-color: #da190b;
    }
    .container {
      display: flex;
      justify-content: center;
      align-items: center;
      flex-direction: column;
      gap: 20px;
    }
  </style>
</head>
<body>

  <h1>Control ESP32 LED</h1>
  <div class="container">
    <a href="/on"><button class="btn">Turn LED ON</button></a>
    <a href="/off"><button class="btn btn-off">Turn LED OFF</button></a>
  </div>

</body>
</html>
)";

// Handle the root URL and display the HTML page
void handleRoot() {
  server.send(200, "text/html", htmlContent);
}

// Handle the "on" URL and turn the LED on
void handleOn() {
  digitalWrite(ledPin, HIGH); // Turn LED on
  server.sendHeader("Location", "/"); // Redirect back to the main page
  server.send(303);
}

// Handle the "off" URL and turn the LED off
void handleOff() {
  digitalWrite(ledPin, LOW); // Turn LED off
  server.sendHeader("Location", "/"); // Redirect back to the main page
  server.send(303);
}

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);

  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nConnected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.on("/on", handleOn);
  server.on("/off", handleOff);

  // 🔥 ADD THIS TO REMOVE ERROR
  server.onNotFound([](){
    server.send(200, "text/plain", "OK");
  });

  server.begin();
  Serial.println("Web server started");
}

void loop() {
  // Handle client requests
  server.handleClient();
}