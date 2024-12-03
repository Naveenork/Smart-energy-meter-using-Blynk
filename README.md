# Smart Energy Meter

This project is an IoT-based smart energy meter that reads voltage and current data from sensors, calculates the power consumption, and sends the data to the Blynk app and a server. It also displays the data on an OLED screen and calculates the electricity cost based on local rates.

## Features

- **Voltage and Current Measurement**: Uses voltage and current sensors to measure energy consumption.
- **OLED Display**: Displays real-time readings of voltage, current, power, and cost.
- **Blynk Integration**: Sends real-time data to the Blynk app for monitoring and control.
- **Server Communication**: Sends data to a remote server for further processing or storage.
- **Cost Calculation**: Calculates the cost of energy consumed based on the power consumption.

## Components Used

- **ESP8266**: Used as the main microcontroller.
- **Voltage Sensor (e.g., ZPT 1 T)**: Measures the voltage.
- **Current Sensor (e.g., ACS712)**: Measures the current.
- **OLED Display (e.g., Adafruit SSD1306)**: Displays real-time data.
- **Wi-Fi**: Connects the device to the internet for cloud communication.

## Required Libraries

To use this project, you need to install the following libraries:

- `ESP8266WiFi`
- `ESP8266HTTPClient`
- `BlynkSimpleEsp8266`
- `Wire`
- `Adafruit_SSD1306`

You can install these libraries from the Arduino IDE Library Manager.

## Configuration

### Wi-Fi Credentials

Replace the `ssid` and `password` variables with your own Wi-Fi credentials:

```cpp
const char* ssid = "YOUR_SSID";         // Wi-Fi SSID
const char* password = "YOUR_PASSWORD"; // Wi-Fi Password


Blynk Auth Token
Replace the auth variable with your own Blynk Auth Token. You can obtain the Auth Token from the Blynk app:

```cpp

char auth[] = "YOUR_BLYNK_AUTH_TOKEN";  // Your Blynk Auth Token
Server URL
If you're sending data to your own server, set the serverUrl variable:

```cpp
const char* serverUrl = "YOUR_SERVER_URL";  // Server URL
Calibration Factors
Adjust the calibration factors for voltage and current sensors as needed:

```cpp
const float voltageCalibration = 5.0;  // Calibration for voltage
const float currentCalibration = 30.0; // Calibration for current
Cost per Unit
Adjust the cost per unit (electricity rate) based on your local rates:

```cpp
const float costPerUnit = 5.0;  // Example: ₹5/unit
Wiring
Voltage Sensor: Connect the sensor to pin D1.
Current Sensor: Connect the sensor to pin D0.
OLED Display: Connect the OLED display via I2C using pins D6 (SDA) and D5 (SCL).


###How to Run
Install the necessary libraries.
Replace the placeholders for Wi-Fi credentials, Blynk Auth Token, and server URL in the code.
Upload the code to the ESP8266 using the Arduino IDE.
Open the Serial Monitor to view the readings and debug information.
Monitor the data on the Blynk app and the OLED display.
Troubleshooting
Ensure that the Wi-Fi credentials and server URL are correct.
If the OLED display is not working, check the I2C wiring and address (0x3C).
If Blynk is not updating, verify that the Auth Token and internet connection are correct.
