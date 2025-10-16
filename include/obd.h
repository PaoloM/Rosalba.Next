#pragma once
#include <Arduino.h>

// Standard OBD-II PID definitions
#define PID_ENGINE_RPM 0x0C
#define PID_VEHICLE_SPEED 0x0D
#define PID_ENGINE_COOLANT_TEMP 0x05
#define PID_THROTTLE_POSITION 0x11
#define PID_ENGINE_LOAD 0x04
#define PID_INTAKE_TEMP 0x0F
#define PID_MAF_FLOW 0x10
#define PID_FUEL_LEVEL 0x2F

// Turbo-specific PIDs (standard OBD-II)
#define PID_INTAKE_MANIFOLD_PRESSURE 0x0B  // Boost pressure
#define PID_BAROMETRIC_PRESSURE 0x33       // Atmospheric pressure
#define PID_TURBO_RPM 0x52                 // Turbocharger speed (if supported)
#define PID_CHARGE_AIR_COOLER_TEMP 0x77    // Intercooler temperature

// Fiat-specific PIDs (manufacturer specific - may need adjustment)
#define PID_FIAT_SPORT_MODE 0x22F190       // Sport mode status (custom)
#define PID_FIAT_BOOST_ACTUAL 0x22F18C     // Actual boost pressure
#define PID_FIAT_BOOST_TARGET 0x22F18D     // Target boost pressure
#define PID_FIAT_TURBO_TEMP 0x22F18E       // Turbo temperature

// Additional boost monitoring PIDs
#define PID_FIAT_BOOST_SENSOR 0x22F189     // Direct hydraulic boost sensor
#define PID_FIAT_WASTEGATE 0x22F187        // Wastegate position
#define PID_FIAT_CHARGE_PRESSURE 0x22F18A  // Charge air pressure

// Extended CAN message IDs for direct monitoring
#define CAN_ID_ENGINE_DATA_1 0x316         // Primary engine parameters
#define CAN_ID_ENGINE_DATA_2 0x329         // Secondary engine data  
#define CAN_ID_BOOST_CONTROL 0x3D7         // Turbo/boost control data
#define CAN_ID_ABARTH_PERFORMANCE 0x4A1    // Abarth performance parameters

// Vehicle parameter structure
struct VehicleData {
  // Basic engine parameters
  float rpm = 0;
  float speed = 0;
  float coolantTemp = 0;
  float throttlePos = 0;
  float engineLoad = 0;
  float intakeTemp = 0;
  float mafFlow = 0;
  float fuelLevel = 0;
  
  // Turbo-specific parameters (Fiat 500 Abarth)
  float boostPressure = 0;        // Manifold absolute pressure (kPa)
  float boostSensorPSI = 0;       // Direct hydraulic boost sensor (PSI)
  float targetBoost = 0;          // ECU target boost pressure (PSI)
  float wastegatePosition = 0;    // Wastegate actuator position (%)
  float chargePressure = 0;       // Charge air pressure after intercooler
  float boostGauge = 0;           // Boost relative to atmospheric (bar)
  float atmosphericPressure = 0;   // Barometric pressure (kPa)  
  float turboRPM = 0;             // Turbocharger speed (RPM)
  float intercoolerTemp = 0;      // Charge air cooler temp (°C)
  
  // Fiat-specific parameters
  bool sportMode = false;         // Sport mode active
  float turboTemp = 0;            // Turbo temperature (°C)
  
  unsigned long lastUpdate = 0;
};

// Function declarations
void initializeCAN();
void processCANMessages();
void processOBDResponse(uint8_t pid, uint8_t* data, uint8_t length);
void sendOBDRequest(uint8_t pid);
uint8_t getCurrentPID(uint8_t index);
void processFiatCANMessage(long canId, uint8_t* data, int dlc);

// Global vehicle data (declared in obd.cpp)
extern VehicleData vehicle;