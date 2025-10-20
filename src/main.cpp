/**
 * Rosalba.Next - Fiat 500 Abarth CAN Monitor
 * ESP32-S3 based OBD-II and CAN bus interface
 * Minimal version focused on CAN communication only
 * For 2015+ Fiat 500 Abarth turbo monitoring
 */

#include <Arduino.h>
#include "obd.h"
#include "display.h"

void setup() {
  // Initialize Serial with USB CDC support for ESP32-S3
  Serial.begin(115200);
  delay(2000); // Longer delay for USB CDC initialization
  
  Serial.println("\n🏁 Rosalba.Next - Abarth CAN Monitor");
  Serial.println("=====================================");
  Serial.println("ESP32-S3 Waveshare 3.5B Touch Display");
  Serial.println("CAN Bus monitoring for 2015+ Fiat 500 Abarth");
  Serial.println();

  // Initialize display first
  Serial.print("📺 Initializing display... ");
  Serial.flush();
  
  initializeDisplay();
  Serial.println("COMPLETE");
  
  // Show Abarth logo
  Serial.println("🦂 Showing Abarth logo...");
  showAbarthLogo();
  
  // Show startup screen
  Serial.println("🚀 Loading startup screen...");
  showStartupScreen();

  // Initialize CAN bus communication  
  Serial.print("🚗 Initializing CAN bus... ");
  displayConnectionStatus(false); // Show disconnected status
  
  try {
    initializeCAN();
    Serial.println("✓ CAN bus ready");
    displayConnectionStatus(true); // Show connected status
  } catch (const std::exception& e) {
    Serial.print("❌ CAN initialization failed: ");
    Serial.println(e.what());
    Serial.println("Continuing without CAN...");
    displayConnectionStatus(false);
  }

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
  static unsigned long lastSerialUpdate = 0;
  static unsigned long lastDisplayUpdate = 0;
  static unsigned long lastStatus = 0;
  unsigned long currentTime = millis();
  
  // Process CAN messages continuously
  processCANMessages();
  
  // Update display every 150ms (smooth for boost readings)
  if (currentTime - lastDisplayUpdate >= DISPLAY_UPDATE_INTERVAL) {
    displayVehicleData(vehicle);
    lastDisplayUpdate = currentTime;
  }
  
  // Print vehicle data to serial every 500ms
  if (currentTime - lastSerialUpdate >= SERIAL_UPDATE_INTERVAL) {
    printVehicleData();
    lastSerialUpdate = currentTime;
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