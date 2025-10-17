/**
 * Rosalba.Next - Fiat 500 Abarth CAN Monitor
 * ESP32-S3 based OBD-II and CAN bus interface
 * Minimal version focused on CAN communication only
 * For 2015+ Fiat 500 Abarth turbo monitoring
 */

#include <Arduino.h>
#include "obd.h"

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  Serial.println("\n🏁 Rosalba.Next - Abarth CAN Monitor (CAN-Only)");
  Serial.println("=================================================");
  Serial.println("ESP32-S3 Waveshare with SN65HVD230 CAN Transceiver");
  Serial.println("CAN Bus monitoring for 2015+ Fiat 500 Abarth");
  Serial.println("Focused on turbo boost and performance data");
  Serial.println();

  // Initialize CAN bus communication  
  Serial.print("🚗 Initializing CAN bus... ");
  initializeCAN();
  Serial.println("✓ CAN bus ready");

  Serial.println("🚀 System ready - monitoring Abarth turbo data");
  Serial.println("Connect CAN transceiver to vehicle OBD-II port");
  Serial.println();
  Serial.println("📊 Monitoring parameters:");
  Serial.println("  • Engine RPM");
  Serial.println("  • Boost Pressure (MAP/hydraulic sensor)");
  Serial.println("  • Throttle Position");  
  Serial.println("  • Intake Air Temperature");
  Serial.println("  • Coolant Temperature");
  Serial.println("  • Sport Mode Status");
  Serial.println();
}

void loop() {
  static unsigned long lastUpdate = 0;
  static unsigned long lastStatus = 0;
  unsigned long currentTime = millis();
  
  // Process CAN messages continuously
  processCANMessages();
  
  // Print vehicle data every 500ms
  if (currentTime - lastUpdate >= 500) {
    printVehicleData();
    lastUpdate = currentTime;
  }
  
  // Print status every 10 seconds
  if (currentTime - lastStatus >= 10000) {
    Serial.println("🔄 CAN monitor active - listening for Abarth data...");
    lastStatus = currentTime;
  }
  
  // Small delay to prevent watchdog issues
  delay(10);
}

// Print current vehicle data to serial
void printVehicleData() {
  Serial.println("═══════════════════════════════════════");
  Serial.print("🏁 RPM: ");
  Serial.print(vehicle.rpm);
  Serial.println(" rpm");
  
  Serial.print("💨 Boost: ");
  Serial.print(vehicle.boostPressure);
  Serial.print(" kPa (");
  Serial.print(vehicle.boostSensorPSI);
  Serial.println(" PSI hydraulic)");
  
  Serial.print("🎛️  Throttle: ");
  Serial.print(vehicle.throttlePos);
  Serial.println("%");
  
  Serial.print("🌡️  Intake: ");
  Serial.print(vehicle.intakeTemp);
  Serial.println("°C");
  
  Serial.print("💧 Coolant: ");
  Serial.print(vehicle.coolantTemp);
  Serial.println("°C");
  
  Serial.print("🏎️  Sport Mode: ");
  Serial.println(vehicle.sportMode ? "ON" : "OFF");
  
  Serial.print("⏱️  Last Update: ");
  Serial.print((millis() - vehicle.lastUpdate) / 1000.0);
  Serial.println("s ago");
  Serial.println("═══════════════════════════════════════");
}