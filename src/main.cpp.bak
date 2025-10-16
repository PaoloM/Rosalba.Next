/**
 * Rosalba.Next - Fiat 500 Abarth Turbo Monitor
 * 
 * Real-time turbo monitoring system designed for 2015 Fiat 500 Abarth:
 * - Boost pressure monitoring and display
 * - Turbocharger RPM and temperature
 * - Sport mode detection
 * - Intercooler temperature monitoring
 * - Performance-oriented dashboard layout
 * 
 * Hardware:
 * - ESP32 NodeMCU-32S microcontroller
 * - SN65HVD230 CAN transceiver  
 * - ILI9486/ILI9488 TFT display (SPI interface)
 * - OBD-II vehicle diagnostics
 * 
 * Author: PaoloM
 * Repository: https://github.com/PaoloM/Rosalba.Next
 * Target Vehicle: 2015 Fiat 500 Abarth (1.4L MultiAir Turbo)
 */

#include <Arduino.h>
#include "config.h"
#include "obd.h"
#include "display.h"
#include "power.h"
// Note: encoder.h and sdcard.h optional - wire components first

void setup() {
  // Initialize serial communication
  Serial.begin(115200);
  while (!Serial);
  
  Serial.println("==========================================");
  Serial.println("Rosalba.Next - Fiat 500 Abarth Monitor");
  Serial.println("Turbo Performance Dashboard v1.1");
  Serial.println("==========================================");
  
  try {
    // Initialize all subsystems
    initializePowerMonitoring();
    initializeDisplay();
    initializeCAN();
    
    // TODO: Add encoder.init() and initializeSDCard() after wiring
    
    // Show success screen
    tft.fillScreen(TFT_GREEN);
    tft.setTextColor(TFT_BLACK);
    tft.setTextSize(3);
    tft.setCursor(150, 120);
    tft.println("System Ready!");
    tft.setTextSize(2);
    tft.setCursor(130, 170);
    tft.println("Starting Monitor...");
    
    delay(2000);
    
    Serial.println("All systems initialized successfully!");
    Serial.println("Starting vehicle parameter monitoring...");
    
  } catch (const std::exception& e) {
    Serial.print("Initialization failed: ");
    Serial.println(e.what());
    showErrorScreen("Init Failed!");
    while (1) delay(1000); // Halt system
  }
}

void loop() {
  // Process CAN messages and handle OBD-II responses
  processCANMessages();
  
  // Update power status periodically
  updatePowerStatus();
  
  // TODO: Add processEncoderInput() and updateSDCard() after wiring
  
  // Update display at configured interval
  static unsigned long lastDisplayUpdate = 0;
  if (millis() - lastDisplayUpdate > DISPLAY_UPDATE_INTERVAL) {
    updateDisplay();
    lastDisplayUpdate = millis();
  }
  
  // Send OBD-II requests at configured interval
  static unsigned long lastOBDRequest = 0;
  static uint8_t currentPID = 0;
  if (millis() - lastOBDRequest > OBD_REQUEST_INTERVAL) {
    sendOBDRequest(getCurrentPID(currentPID));
    currentPID = (currentPID + 1) % 4; // Cycle through main PIDs
    lastOBDRequest = millis();
  }
  
  // Small delay to prevent overwhelming the system
  delay(10);
}