#include "power.h"
#include "config.h"

// Global system info
SystemInfo systemInfo;

void initializePowerMonitoring() {
  pinMode(POWER_SENSE_PIN, INPUT);
  detectPowerSource();
  
  Serial.print("Power Source: ");
  Serial.println(systemInfo.powerSource);
  Serial.print("VIN Voltage: ");
  Serial.print(systemInfo.vinVoltage, 2);
  Serial.println("V");
}

void detectPowerSource() {
  // Read VIN voltage through voltage divider
  // NodeMCU-32S has built-in voltage divider on some pins
  int adcReading = analogRead(POWER_SENSE_PIN);
  
  // Convert ADC reading to voltage
  // ESP32 ADC: 12-bit (0-4095), reference voltage ~3.3V
  systemInfo.vinVoltage = (adcReading / 4095.0) * 3.3 * 2.0; // Assuming 2:1 voltage divider
  
  // Determine power source based on voltage level
  if (systemInfo.vinVoltage > 4.5) {
    // External 5V power (USB or VIN pin)
    if (systemInfo.vinVoltage > 4.7 && systemInfo.vinVoltage < 5.3) {
      systemInfo.powerSource = "USB (5V)";
      systemInfo.isUSBPowered = true;
    } else {
      systemInfo.powerSource = "External (" + String(systemInfo.vinVoltage, 1) + "V)";
      systemInfo.isUSBPowered = false;
    }
  } else if (systemInfo.vinVoltage > 3.0) {
    systemInfo.powerSource = "3.3V Direct";
    systemInfo.isUSBPowered = false;
  } else {
    systemInfo.powerSource = "Low/Unknown";
    systemInfo.isUSBPowered = false;
  }
}

void updatePowerStatus() {
  static unsigned long lastPowerCheck = 0;
  if (millis() - lastPowerCheck > POWER_CHECK_INTERVAL) {
    detectPowerSource();
    lastPowerCheck = millis();
  }
}