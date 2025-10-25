#include "obd.h"
#include "config.h"
#include <CAN.h>

// Global vehicle data
VehicleData vehicle;

void initializeCAN() {
  // Set CAN pins for ESP32
  CAN.setPins(CAN_RX_PIN, CAN_TX_PIN);
  
  // Initialize CAN bus at 500 kbps (OBD-II standard)
  if (!CAN.begin(500E3)) {
    Serial.println("ERROR: CAN initialization failed!");
    Serial.println("Check wiring:");
    Serial.print("- Freenove GPIO");
    Serial.print(CAN_TX_PIN);
    Serial.println(" (TXD0) -> SN65HVD230 D pin");
    Serial.print("- Freenove GPIO");
    Serial.print(CAN_RX_PIN);
    Serial.println(" (RXD0) -> SN65HVD230 R pin");
    Serial.println("Note: Using UART0 pins - RGB LED remains functional");
    throw std::runtime_error("CAN initialization failed");
  }
  
  Serial.println("CAN bus initialized successfully!");
  Serial.println("Bus speed: 500 kbps");
  Serial.print("Freenove FNK0103S CAN pins - TX: GPIO");
  Serial.print(CAN_TX_PIN);
  Serial.print(" (TXD0), RX: GPIO");
  Serial.print(CAN_RX_PIN);
  Serial.println(" (RXD0)");
}

void processCANMessages() {
  // Check for incoming CAN messages
  int packetSize = CAN.parsePacket();
  
  if (packetSize) {
    // Process received CAN message
    uint32_t canId = CAN.packetId();
    uint8_t dlc = packetSize;
    
    // Read data from CAN packet
    uint8_t data[8];
    for (int i = 0; i < dlc && i < 8; i++) {
      data[i] = CAN.read();
    }
    
    // Process OBD-II response messages (7E8, 7E9, 7EA, 7EB, etc.)
    if ((canId >= 0x7E8 && canId <= 0x7EF) && dlc >= 3) {
      if (data[0] >= 0x03 && data[1] == 0x41) { // Positive response
        processOBDResponse(data[2], &data[3], data[0] - 2);
      }
    }
    
    // Process direct CAN messages from Fiat ECUs
    processFiatCANMessage(canId, data, dlc);
    
    // Log raw CAN data to serial
    Serial.print("0x");
    Serial.print(canId, HEX);
    Serial.print(" [");
    Serial.print(dlc);
    Serial.print("] ");
    for (int i = 0; i < dlc; i++) {
      if (data[i] < 0x10) Serial.print("0");
      Serial.print(data[i], HEX);
      Serial.print(" ");
    }
    Serial.println();
  }
}

void processOBDResponse(uint8_t pid, uint8_t* data, uint8_t length) {
  vehicle.lastUpdate = millis();
  
  switch (pid) {
    case PID_ENGINE_RPM:
      if (length >= 2) {
        vehicle.rpm = ((data[0] * 256) + data[1]) / 4.0;
      }
      break;
      
    case PID_VEHICLE_SPEED:
      if (length >= 1) {
        vehicle.speed = data[0];
      }
      break;
      
    case PID_ENGINE_COOLANT_TEMP:
      if (length >= 1) {
        vehicle.coolantTemp = data[0] - 40;
      }
      break;
      
    case PID_THROTTLE_POSITION:
      if (length >= 1) {
        vehicle.throttlePos = (data[0] * 100.0) / 255.0;
      }
      break;
      
    case PID_ENGINE_LOAD:
      if (length >= 1) {
        vehicle.engineLoad = (data[0] * 100.0) / 255.0;
      }
      break;
      
    case PID_INTAKE_TEMP:
      if (length >= 1) {
        vehicle.intakeTemp = data[0] - 40;
      }
      break;
      
    // Turbo-specific parameters
    case PID_INTAKE_MANIFOLD_PRESSURE:
      if (length >= 1) {
        vehicle.boostPressure = data[0];  // kPa absolute
        // Calculate boost gauge reading (relative to atmospheric)
        if (vehicle.atmosphericPressure > 0) {
          vehicle.boostGauge = (vehicle.boostPressure - vehicle.atmosphericPressure) / 100.0; // Convert to bar
        }
      }
      break;
      
    case PID_BAROMETRIC_PRESSURE:
      if (length >= 1) {
        vehicle.atmosphericPressure = data[0];  // kPa
      }
      break;
      
    case PID_TURBO_RPM:
      if (length >= 2) {
        vehicle.turboRPM = ((data[0] * 256) + data[1]) * 10.0;  // RPM * 10
      }
      break;
      
    case PID_CHARGE_AIR_COOLER_TEMP:
      if (length >= 1) {
        vehicle.intercoolerTemp = data[0] - 40;  // °C
      }
      break;
      
    default:
      // Unknown PID - could add logging here
      break;
  }
}

void sendOBDRequest(uint8_t pid) {
  // Begin CAN packet with standard OBD-II request ID
  CAN.beginPacket(0x7DF);
  
  // Fill OBD-II request data
  CAN.write(0x02);   // Length
  CAN.write(0x01);   // Mode 01 (current data)
  CAN.write(pid);    // PID
  CAN.write(0x00);   // Padding
  CAN.write(0x00);
  CAN.write(0x00);
  CAN.write(0x00);
  CAN.write(0x00);
  
  // Send the frame
  if (!CAN.endPacket()) {
    Serial.println("Warning: CAN send failed");
  }
}

uint8_t getCurrentPID(uint8_t index) {
  // Optimized PID rotation for Fiat 500 Abarth turbo monitoring
  uint8_t pids[] = {
    PID_ENGINE_RPM,                    // Most important - engine speed
    PID_INTAKE_MANIFOLD_PRESSURE,     // Boost pressure (key for turbo)
    PID_VEHICLE_SPEED,                 // Speed
    PID_ENGINE_COOLANT_TEMP,          // Temperature monitoring
    PID_THROTTLE_POSITION,            // Throttle input
    PID_ENGINE_LOAD,                  // Engine load
    PID_BAROMETRIC_PRESSURE,          // For boost calculation
    PID_INTAKE_TEMP,                  // Intake air temperature
    PID_CHARGE_AIR_COOLER_TEMP,       // Intercooler temp (if supported)
    PID_TURBO_RPM                     // Turbo speed (if supported)
  };
  return pids[index % (sizeof(pids) / sizeof(pids[0]))];
}

// Process Fiat-specific CAN messages for direct boost sensor access
void processFiatCANMessage(long canId, uint8_t* data, int dlc) {
  switch (canId) {
    case CAN_ID_ENGINE_DATA_1: // 0x316 - Primary engine data
      if (dlc >= 8) {
        // Parse engine RPM from direct CAN (typically bytes 0-1)
        uint16_t rpmRaw = (data[0] << 8) | data[1];
        vehicle.rpm = rpmRaw * 0.25; // Common scaling factor
        
        // Parse throttle position (byte 2)
        vehicle.throttlePos = data[2] * 0.392; // Convert to percentage
        
        Serial.print("Direct CAN RPM: ");
        Serial.println(vehicle.rpm);
      }
      break;
      
    case CAN_ID_BOOST_CONTROL: // 0x3D7 - Turbo/boost control data
      if (dlc >= 6) {
        // Parse hydraulic boost sensor (bytes 0-1, signed 16-bit)
        int16_t boostRaw = (data[0] << 8) | data[1];
        vehicle.boostSensorPSI = boostRaw * 0.01; // Convert to PSI
        
        // Parse target boost (bytes 2-3)
        uint16_t targetRaw = (data[2] << 8) | data[3];
        vehicle.targetBoost = targetRaw * 0.01;
        
        // Parse wastegate position (byte 4)
        vehicle.wastegatePosition = data[4] * 0.392; // Convert to percentage
        
        // Parse charge pressure (byte 5)
        vehicle.chargePressure = data[5] * 2.0; // Convert to kPa
        
        Serial.print("🔥 Hydraulic Boost Sensor: ");
        Serial.print(vehicle.boostSensorPSI);
        Serial.print(" PSI, Target: ");
        Serial.print(vehicle.targetBoost);
        Serial.print(" PSI, Wastegate: ");
        Serial.print(vehicle.wastegatePosition);
        Serial.println("%");
      }
      break;
      
    case CAN_ID_ABARTH_PERFORMANCE: // 0x4A1 - Abarth performance data
      if (dlc >= 4) {
        // Parse sport mode status (byte 0, bit 0)
        vehicle.sportMode = (data[0] & 0x01) == 1;
        
        // Parse performance flags (byte 1)
        bool trackMode = (data[1] & 0x02) == 2;
        bool launchControl = (data[1] & 0x04) == 4;
        
        if (trackMode) {
          Serial.println("🏁 Track Mode Active!");
        }
        
        Serial.print("🏁 Sport Mode: ");
        Serial.println(vehicle.sportMode ? "ON" : "OFF");
      }
      break;
      
    case CAN_ID_ENGINE_DATA_2: // 0x329 - Secondary engine data
      if (dlc >= 8) {
        // Parse coolant temperature (byte 0)
        vehicle.coolantTemp = data[0] - 40; // Convert from offset
        
        // Parse intake air temperature (byte 1) 
        vehicle.intakeTemp = data[1] - 40;
        
        // Parse MAF flow (bytes 2-3)
        uint16_t mafRaw = (data[2] << 8) | data[3];
        vehicle.mafFlow = mafRaw * 0.01; // Convert to g/s
        
        // Parse fuel level (byte 4)
        vehicle.fuelLevel = data[4] * 0.392; // Convert to percentage
      }
      break;
      
    default:
      // Log unknown CAN messages for analysis
      if (canId < 0x800) { // Standard CAN frame
        Serial.print("Unknown CAN ID: 0x");
        Serial.print(canId, HEX);
        Serial.print(" Data: ");
        for (int i = 0; i < dlc; i++) {
          Serial.print("0x");
          Serial.print(data[i], HEX);
          Serial.print(" ");
        }
        Serial.println();
      }
      break;
  }
}