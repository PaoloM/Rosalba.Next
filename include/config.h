#pragma once

// CAN bus configuration for Freenove FNK0103S with SN65HVD230 transceiver
#define CAN_TX_PIN 1   // ESP32 CAN TX to SN65HVD230 D pin (TXD0 - UART0 TX)
#define CAN_RX_PIN 3   // ESP32 CAN RX to SN65HVD230 R pin (RXD0 - UART0 RX) 
// Note: CAN speed (500kbps) is configured in CAN.begin()
// Note: Using UART0 pins - keeps RGB LED (GPIO16,17,22) fully functional

// Timing configuration - Optimized for Fiat 500 Abarth turbo monitoring
#define OBD_REQUEST_INTERVAL 300      // OBD-II request frequency (ms) - fast for boost response
#define CAN_TIMEOUT 2000              // Connection timeout (ms)
#define SERIAL_UPDATE_INTERVAL 500    // Serial data output frequency (ms)

// Vehicle-specific configuration
#define ENABLE_FIAT_PIDS true         // Enable Fiat-specific PID requests
#define ENABLE_DIRECT_CAN true        // Enable direct CAN message monitoring
#define BOOST_SENSOR_HYDRAULIC true   // Use hydraulic boost sensor for accurate readings