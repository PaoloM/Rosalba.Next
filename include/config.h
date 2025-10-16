#pragma once

// Pin definitions for SN65HVD230 connection
#define CAN_TX_PIN 21  // ESP32 CAN TX to SN65HVD230 D pin
#define CAN_RX_PIN 22  // ESP32 CAN RX to SN65HVD230 R pin

// Power source detection pin
#define POWER_SENSE_PIN 35  // ADC pin to monitor VIN voltage

// Display configuration - SPI Interface
#define SCREEN_WIDTH 480
#define SCREEN_HEIGHT 320

// SPI interface pin definitions (based on pinout pins 31-34)
#define TFT_MISO 19  // SPI_MISO (Pin 31) - Master In Slave Out
#define TFT_MOSI 23  // SPI_MOSI (Pin 32) - Master Out Slave In  
#define TFT_SCLK 18  // SPI_CLK (Pin 33) - SPI Clock
#define TFT_CS   15  // Could use SD_CS (Pin 34) or LCD_CS (Pin 21)
#define TFT_DC   2   // LCD_RS (Pin 19) - Data/Command (same as RS)
#define TFT_RST  4   // LCD_RST (Pin 22) - Reset

// CAN bus configuration
#define CAN_SPEED 500E3  // 500 kbps

// Timing configuration - Optimized for Fiat 500 Abarth turbo monitoring
#define DISPLAY_UPDATE_INTERVAL 150   // Display refresh rate (ms) - faster for turbo
#define OBD_REQUEST_INTERVAL 300      // OBD-II request frequency (ms) - faster for boost response
#define POWER_CHECK_INTERVAL 5000     // Power status check interval (ms)
#define CAN_TIMEOUT 2000             // Connection timeout (ms)

// Color definitions (TFT_eSPI compatible)
#define TFT_BLACK    0x0000
#define TFT_BLUE     0x001F
#define TFT_RED      0xF800
#define TFT_GREEN    0x07E0
#define TFT_CYAN     0x07FF
#define TFT_MAGENTA  0xF81F
#define TFT_YELLOW   0xFFE0
#define TFT_WHITE    0xFFFF
#define TFT_ORANGE   0xFD20
#define TFT_GRAY     0x8410
#define TFT_DARKGRAY 0x4208