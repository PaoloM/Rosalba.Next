#pragma once

// CAN bus configuration for ESP32-S3 with SN65HVD230 transceiver
#define CAN_TX_PIN 17  // ESP32-S3 CAN TX to SN65HVD230 D pin
#define CAN_RX_PIN 18  // ESP32-S3 CAN RX to SN65HVD230 R pin
// Note: CAN speed (500kbps) is configured in ACAN_ESP32_Settings

// Timing configuration - Optimized for Fiat 500 Abarth turbo monitoring
#define OBD_REQUEST_INTERVAL 300      // OBD-II request frequency (ms) - fast for boost response
#define CAN_TIMEOUT 2000              // Connection timeout (ms)
#define SERIAL_UPDATE_INTERVAL 500    // Serial data output frequency (ms)

// Vehicle-specific configuration
#define ENABLE_FIAT_PIDS true         // Enable Fiat-specific PID requests
#define ENABLE_DIRECT_CAN true        // Enable direct CAN message monitoring
#define BOOST_SENSOR_HYDRAULIC true   // Use hydraulic boost sensor for accurate readings

// Display configuration for integrated Waveshare ESP32-S3
#define SCREEN_WIDTH 480
#define SCREEN_HEIGHT 320
#define DISPLAY_UPDATE_INTERVAL 150   // Display refresh rate (ms)

// Abarth brand colors (RGB565 format)
#define ABARTH_RED    0xF800    // Bright red
#define ABARTH_WHITE  0xFFFF    // White
#define ABARTH_BLACK  0x0000    // Black
#define ABARTH_GRAY   0x8410    // Gray
#define ABARTH_GREEN  0x07E0    // Green for boost readings