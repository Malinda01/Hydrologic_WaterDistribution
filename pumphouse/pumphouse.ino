#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>
#include <NewPing.h>

// WiFi
#define WIFI_SSID "your-ssid"
#define WIFI_PASSWORD "your-password"

// Firebase
#define FIREBASE_HOST "https://newhydro-cd451-default-rtdb.firebaseio.com/"
#define FIREBASE_AUTH "ujc2fHvZ2uSzUKOAJAzmlQU3vFoBXeUs7FgrP20J"

FirebaseData fbdo;

// Relay Pins
#define RELAY_PRESSURE D1   // IN1
#define RELAY_WATERLEVEL D2 // IN2

// Ultrasonic Pins
#define TRIG_PIN D5
#define ECHO_PIN D6
NewPing sonar(TRIG_PIN, ECHO_PIN, 200); // 200cm max range

void setup() {
  Serial.begin(9600);

  // Relay Setup
  pinMode(RELAY_PRESSURE, OUTPUT);
  pinMode(RELAY_WATERLEVEL, OUTPUT);
  digitalWrite(RELAY_PRESSURE, LOW);     // Start OFF
  digitalWrite(RELAY_WATERLEVEL, LOW);   // Start OFF

  // WiFi & Firebase
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);
}

void loop() {
  // === 1. Read ultrasonic level ===
  int level = sonar.ping_cm();
  if (level == 0) level = 999; // Timeout = no reading

  Firebase.setInt(fbdo, "/pump_house/ultrasonic_level", level);
  Serial.print("Water Level: ");
  Serial.println(level);

  // === 2. Control pump based on pressure status ===
  if (Firebase.getString(fbdo, "/water_monitor/pressure_status")) {
    String status = fbdo.stringData();
    if (status == "low") {
      digitalWrite(RELAY_PRESSURE, HIGH); // Turn ON pressure pump
      Firebase.setString(fbdo, "/pump_house/relay_status", "on");
    } else {
      digitalWrite(RELAY_PRESSURE, LOW); // Turn OFF pressure pump
      Firebase.setString(fbdo, "/pump_house/relay_status", "off");
    }
  }

  // === 3. Control 5V pump based on ultrasonic level ===
  if (level < 15) {
    digitalWrite(RELAY_WATERLEVEL, HIGH); // ON if level low
    Firebase.setString(fbdo, "/pump_house/level_pump", "on");
  } else if (level >= 23) {
    digitalWrite(RELAY_WATERLEVEL, LOW);  // OFF when tank is full
    Firebase.setString(fbdo, "/pump_house/level_pump", "off");
  }

  delay(2000);
}
