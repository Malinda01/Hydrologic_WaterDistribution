#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>
#include <HX710B.h>

// === WiFi Credentials ===
#define WIFI_SSID "your-ssid"
#define WIFI_PASSWORD "your-password"

// === Firebase Credentials ===
#define FIREBASE_HOST "https://newhydro-cd451-default-rtdb.firebaseio.com/"
#define FIREBASE_AUTH "ujc2fHvZ2uSzUKOAJAzmlQU3vFoBXeUs7FgrP20J"

FirebaseData fbdo;

// === Flow Sensor ===
#define FLOW_SENSOR D1
volatile int flowPulseCount = 0;
float totalLiters = 0.0;

// === Pressure Sensor ===
#define SCK_PIN D6
#define DOUT_PIN D5
HX710B pressureSensor(SCK_PIN, DOUT_PIN);
long pressureValue = 0;

// === Flow Sensor Interrupt ===
void IRAM_ATTR countPulse() {
  flowPulseCount++;
}

void setup() {
  Serial.begin(9600);
  pinMode(FLOW_SENSOR, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(FLOW_SENSOR), countPulse, RISING);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);
}

void loop() {
  // === Flow Calculation ===
  int pulses = flowPulseCount;
  flowPulseCount = 0;

  float liters = (pulses / 7.5);  // Adjust based on sensor spec
  totalLiters += liters;
  float unitsUsed = totalLiters / 1000.0; // Example: 1 unit = 1000L

  // === Pressure Reading ===
  pressureValue = pressureSensor.read();
  String pressureStatus = pressureValue < 100000 ? "low" : "normal";

  // === Firebase Update ===
  Firebase.setFloat(fbdo, "/water_monitor/liters_used", totalLiters);
  Firebase.setFloat(fbdo, "/water_monitor/units_used", unitsUsed);
  Firebase.setInt(fbdo, "/water_monitor/pressure", pressureValue);
  Firebase.setString(fbdo, "/water_monitor/pressure_status", pressureStatus);

  Serial.printf("Flow: %.2fL, Units: %.3f, Pressure: %ld (%s)\n",
    totalLiters, unitsUsed, pressureValue, pressureStatus.c_str());

  delay(5000);  // Adjust as needed
}
