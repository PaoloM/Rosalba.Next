#pragma once
#include <Arduino.h>

// System information structure
struct SystemInfo {
  float vinVoltage = 0;
  bool isUSBPowered = false;
  String powerSource = "Unknown";
};

// Function declarations
void initializePowerMonitoring();
void detectPowerSource();
void updatePowerStatus();

// Global system info (declared in power.cpp)
extern SystemInfo systemInfo;