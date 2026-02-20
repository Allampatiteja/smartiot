// Skill 13 Main.cpp
#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <DHT.h>

const char* ssid = "Wokwi-GUEST";
const char* password = "";

#define DHTPIN 4
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

unsigned long lastTime = 0;
unsigned long timerDelay = 2000;

// Optimized HTML with "Auto-Port" WebSocket logic
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <title>Real-Time Sensor Dashboard</title>
  <style>
    body { font-family: 'Segoe UI', sans-serif; text-align: center; margin-top: 50px; background-color: #2c3e50; color: white; }
    .container { display: flex; justify-content: center; gap: 20px; margin-top: 30px; }
    .card { background: #ecf0f1; padding: 30px; border-radius: 15px; width: 200px; color: #333; }
    .reading { font-size: 3em; font-weight: bold; }
    .temp-color { color: #e74c3c; }
    .hum-color { color: #3498db; }
    #status { font-size: 1em; margin-top: 20px; color: #bdc3c7; }
  </style>
</head>
<body>
  <h1>Live Environment Data</h1>
  <div class="container">
    <div class="card">
      <h2>Temperature</h2>
      <p><span class="reading temp-color" id="temp">--</span> &deg;C</p>
    </div>
    <div class="card">
      <h2>Humidity</h2>
      <p><span class="reading hum-color" id="hum">--</span> %</p>
    </div>
  </div>
  <p id="status">Connecting WebSocket...</p>

  <script>
    // FIXED: Using location.host ensures port 8180 is included
    var gateway = `ws://${window.location.host}/ws`;
    var websocket;
    
    function initWebSocket() {
      console.log('Connecting to:', gateway);
      websocket = new WebSocket(gateway);
      websocket.onopen = () => { document.getElementById('status').innerHTML = 'WebSocket: Connected'; };
      websocket.onclose = () => { 
        document.getElementById('status').innerHTML = 'WebSocket: Disconnected (Retrying...)';
        setTimeout(initWebSocket, 2000); 
      };
      websocket.onmessage = (event) => {
        var obj = JSON.parse(event.data);
        document.getElementById('temp').innerHTML = obj.temp.toFixed(1);
        document.getElementById('hum').innerHTML = obj.hum.toFixed(1);
      };
    }
    window.onload = initWebSocket;
  </script>
</body>
</html>
)rawliteral";

void notifyClients() {
  float t = dht.readTemperature();
  float h = dht.readHumidity();

  if (!isnan(t) && !isnan(h)) {
    String jsonString = "{\"temp\":" + String(t) + ", \"hum\":" + String(h) + "}";
    ws.textAll(jsonString);
    Serial.println("[+] Broadcast Sent: " + jsonString);
  } else {
    Serial.println("[-] DHT Sensor Error!");
  }
}

void setup() {
  Serial.begin(115200);
  dht.begin();
  
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) { delay(500); Serial.print("."); }
  Serial.println("\n[!] WiFi Connected! IP: " + WiFi.localIP().toString());

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    // Explicit cast to (const char*) fixes the "declared here" note/warning
    request->send_P(200, "text/html", (const char*)index_html);
  });

  server.addHandler(&ws);
  server.begin();
  Serial.println("[!] Server & WebSocket Started");
}

void loop() {
  ws.cleanupClients();
  if ((millis() - lastTime) > timerDelay) {
    notifyClients();
    lastTime = millis();
  }
}