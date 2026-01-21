#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "Wokwi-GUEST";
const char* password = "";

// Wokwi test HTTP server
const char* serverUrl = "http://httpbin.org/post";

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi connected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    // Dummy sensor values
    float temperature = 31;
    float humidity = 68.3;

    // JSON payload
    String jsonPayload = "{";
    jsonPayload += "\"temperature\": " + String(temperature, 2) + ",";
    jsonPayload += "\"humidity\": " + String(humidity, 2);
    jsonPayload += "}";

    http.begin(serverUrl);
    http.addHeader("Content-Type", "application/json");

    int httpResponseCode = http.POST(jsonPayload);

    // Print formatted output
    Serial.println("\n========================================");
    Serial.println("        HTTP POST REQUEST");
    Serial.println("========================================");
    Serial.print("Timestamp: ");
    Serial.println(millis());
    Serial.print("Server URL: ");
    Serial.println(serverUrl);
    Serial.print("Payload: ");
    Serial.println(jsonPayload);
    Serial.println("----------------------------------------");

    if (httpResponseCode > 0) {
      Serial.print("HTTP Response Code: ");
      Serial.println(httpResponseCode);
      Serial.println("Status: SUCCESS ✓");
      Serial.println("----------------------------------------");
      Serial.println("Server Response:");
      Serial.println("----------------------------------------");

      String response = http.getString();
      Serial.println(response);
      Serial.println("----------------------------------------");
    } else {
      Serial.print("HTTP Response Code: ");
      Serial.println(httpResponseCode);
      Serial.println("Status: ERROR ✗");
      Serial.println("----------------------------------------");
    }

    Serial.println("========================================\n");

    http.end();
  } else {
    Serial.println("WiFi not connected");
  }

  delay(5000);  // Send every 5 seconds
}
