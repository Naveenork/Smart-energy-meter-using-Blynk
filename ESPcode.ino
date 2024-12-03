#define BLYNK_TEMPLATE_ID ""
#define BLYNK_TEMPLATE_NAME ""

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <BlynkSimpleEsp8266.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>

// Blynk Auth Token (Replace with your actual Auth Token)
char auth[] = "";  // Your Auth Token

// Wi-Fi credentials
const char* ssid = "";                   // Replace with your Wi-Fi SSID
const char* password = "";       // Replace with your Wi-Fi Password

// Server URL (if you're sending data to your own server)
const char* serverUrl = "http://192.168.75.137:3000/data";
// Sensor Pins
const int voltageSensorPin = D1;  // Voltage sensor connected to D1
const int currentSensorPin = D0;  // Current sensor connected to D0

// Calibration factors
const float voltageCalibration = 5.0;
const float currentCalibration = 30.0;
const int adcResolution = 1024;

// Sampling settings
const int samples = 500;

// Cost per unit (adjust based on your local rates)
const float costPerUnit = 5.0;  // Example: ₹5/unit

// I2C Display settings
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void setup() {
  Serial.begin(115200);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  Serial.println("Connecting to Wi-Fi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nConnected to Wi-Fi");

  // Start Blynk
  Blynk.begin(auth, ssid, password);

  // Initialize Display (I2C on D5 and D6)
  Wire.begin(D6, D5);  // SDA on D6, SCL on D5
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {  // Corrected line
    Serial.println("SSD1306 allocation failed");
    for (;;);
  }
  display.clearDisplay();
  display.display();
}

void loop() {
  Blynk.run();

  // Read sensor data
  float voltage = readVoltage();
  float current = readCurrent();
  float power = voltage * current;
  float cost = power * costPerUnit / 1000;  // Calculate cost for kWh

  // Debugging output to Serial Monitor
  Serial.println("===== Sensor Readings =====");
  Serial.printf("Voltage: %.2f V\n", voltage);
  Serial.printf("Current: %.2f A\n", current);
  Serial.printf("Power: %.2f W\n", power);
  Serial.printf("Cost: ₹%.2f\n", cost);

  // Send data to the server
  if (WiFi.status() == WL_CONNECTED) {
    sendToServer(voltage, current, power);
  } else {
    reconnectWiFi();
  }

  // Send data to Blynk Virtual Pins
  Blynk.virtualWrite(V0, voltage);  // Voltage data to V0
  Blynk.virtualWrite(V1, current);  // Current data to V1
  Blynk.virtualWrite(V2, power);    // Power data to V2
  Blynk.virtualWrite(V3, cost);     // Cost data to V3

  // Update Display
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.printf("Voltage: %.2f V\n", voltage);
  display.printf("Current: %.2f A\n", current);
  display.printf("Power: %.2f W\n", power);
  display.printf("Cost: ₹%.2f\n", cost);
  display.display();

  delay(1000);  // Wait for 1 second before next loop iteration
}

// Function to read voltage
float readVoltage() {
  float sum = 0;
  for (int i = 0; i < samples; i++) {
    sum += analogRead(voltageSensorPin);
  }
  float average = sum / samples;
  float voltage = (average * voltageCalibration) / adcResolution;
  return voltage;
}

// Function to read current
float readCurrent() {
  float sum = 0;
  for (int i = 0; i < samples; i++) {
    sum += analogRead(currentSensorPin);
  }
  float average = sum / samples;
  float current = ((average - (adcResolution / 2)) * currentCalibration) / adcResolution;
  return current;
}

// Function to send data to the server
void sendToServer(float voltage, float current, float power) {
  WiFiClient client;
  HTTPClient http;

  http.begin(client, serverUrl);
  http.addHeader("Content-Type", "application/json");

  String jsonPayload = "{\"voltage\":" + String(voltage) +
                       ",\"current\":" + String(current) +
                       ",\"power\":" + String(power) + "}";

  int httpResponseCode = http.POST(jsonPayload);

  if (httpResponseCode > 0) {
    String response = http.getString();
    Serial.printf("Server Response (%d): %s\n", httpResponseCode, response.c_str());
  } else {
    Serial.printf("Error sending data: %s\n", http.errorToString(httpResponseCode).c_str());
  }

  http.end();
}

// Function to reconnect Wi-Fi
void reconnectWiFi() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Wi-Fi disconnected. Reconnecting...");
    WiFi.disconnect();
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
      delay(1000);
      Serial.print("...");
    }
    Serial.println("\nReconnected to Wi-Fi");
  }
}
