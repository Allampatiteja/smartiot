//skill 16 main.cpp
#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "Wokwi-GUEST";
const char* password = "";

WebServer server(80);

String firmwareVersion = "1.0";
bool updated = false;

String htmlPage() {

  String message = "";

  if (updated) {
    message = "<h3 style='color:green;'>✅ Firmware Updated Successfully!</h3>";
  }

  String page = "<!DOCTYPE html><html><head>";
page += "<meta charset='UTF-8'>";
page += "<title>ESP32 OTA Demo</title>";
page += "</head><body style='text-align:center;font-family:Arial;'>";

  page += "<h2>ESP32 OTA Academic Demo</h2>";
  page += "<h3>Current Firmware Version: " + firmwareVersion + "</h3>";
  page += message;

  page += "<br><br><button onclick=\"location.href='/update'\" ";
  page += "style='padding:15px;font-size:18px;'>Check for Firmware Update</button>";

  page += "</body></html>";

  return page;
}

void simulateOTA() {

  Serial.println("Checking for firmware update...");
  delay(1000);

  Serial.println("Firmware found!");
  delay(1000);

  Serial.println("Downloading firmware...");
  delay(1500);

  Serial.println("Writing to flash...");
  delay(1500);

  Serial.println("Verifying firmware...");
  delay(1000);

  Serial.println("Update Successful!");

  firmwareVersion = "2.0";
  updated = true;

  Serial.println("Firmware version updated to 2.0");
}

void handleRoot() {
  server.send(200, "text/html", htmlPage());
}

void handleUpdate() {

  simulateOTA();

  server.sendHeader("Location", "/");
  server.send(303);
}

void setup() {

  Serial.begin(115200);

  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nConnected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.on("/update", handleUpdate);
  server.begin();

  Serial.println("Web server started");
}

void loop() {
  server.handleClient();
}