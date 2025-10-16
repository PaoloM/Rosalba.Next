#include "display.h"
#include "obd.h"
#include "power.h"
#include "abarth_logo.h"
#include <Arduino.h>
#include <SPIFFS.h>
#include "FS.h"

// Enable SPIFFS bitmap support
#define USE_SPIFFS_BITMAP

// Global display object
TFT_eSPI tft = TFT_eSPI();

void initializeDisplay() {
  Serial.println("Starting display initialization...");
  
  // Test reset pin first
  Serial.println("Testing reset pin...");
  pinMode(TFT_RST, OUTPUT);
  digitalWrite(TFT_RST, LOW);
  delay(100);
  digitalWrite(TFT_RST, HIGH);
  delay(100);
  Serial.println("Reset sequence completed");
  
  // Initialize TFT display (ILI9486/ILI9488)
  Serial.println("Calling tft.init()...");
  tft.init();
  
  Serial.println("Setting rotation to landscape...");
  tft.setRotation(3); // Landscape mode (480x320)
  
  Serial.println("Testing basic color fill...");
  tft.fillScreen(TFT_RED);
  delay(500);
  tft.fillScreen(TFT_GREEN);
  delay(500);
  tft.fillScreen(TFT_BLUE);
  delay(500);
  tft.fillScreen(TFT_BLACK);
  
  Serial.println("ILI9486/ILI9488 TFT Display initialized");
  Serial.println("Resolution: 480x320 (landscape)");
  Serial.println("SPI Interface: MOSI=23, MISO=19, SCK=18, CS=15, DC=2, RST=4");
  
  showStartupScreen();
}

void showStartupScreen() {
  // Show Abarth splash screen first
  showAbarthSplash();
  delay(3000); // Show logo for 3 seconds
  
  // Then show initialization screen
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(3);
  tft.setCursor(120, 100);
  tft.println("Rosalba Monitor");
  tft.setTextSize(2);
  tft.setCursor(150, 150);
  tft.setTextColor(TFT_CYAN);
  tft.println("ILI9486/ILI9488");
  tft.setCursor(150, 180);
  tft.setTextColor(TFT_WHITE);
  tft.println("Initializing...");
}

void showAbarthSplash() {
  // Fill screen with black background
  tft.fillScreen(TFT_BLACK);
  
  // Try to load the authentic BMP logo from SPIFFS
  drawAbarthLogoFromSPIFFS();
  
  // Add version info at bottom corner (will overlay on image)
  tft.setTextColor(0x4208); // Dark gray that shows on both black and colored backgrounds
  tft.setTextSize(1);
  tft.setCursor(390, 300);
  tft.print("v1.1");
}

void drawAbarthLogoVector() {
  // Simplified fallback version
  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(4);
  tft.setCursor(150, 140);
  tft.print("ABARTH");
  tft.setTextSize(2);
  tft.setCursor(180, 180);
  tft.print("LOADING...");
}

void drawAbarthLogoEnhanced() {
  // Enhanced vector version that closely matches the authentic Abarth logo
  tft.fillScreen(TFT_BLACK);
  
  // Define authentic Abarth colors (RGB565)
  #define ABARTH_YELLOW   0xFFE0  // Bright yellow shield
  #define ABARTH_BLUE     0x2945  // Blue banner
  #define ABARTH_RED      0xF800  // Red section
  #define ABARTH_BLACK    0x0000  // Black scorpion
  #define ABARTH_WHITE    0xFFFF  // White text
  
  int centerX = 240;
  int centerY = 160;
  int shieldW = 140;
  int shieldH = 160;
  
  // Draw main shield shape (more accurate proportions)
  // Outer yellow border
  tft.fillRoundRect(centerX - shieldW/2, centerY - shieldH/2, shieldW, shieldH, 20, ABARTH_YELLOW);
  
  // Inner black background
  tft.fillRoundRect(centerX - shieldW/2 + 6, centerY - shieldH/2 + 6, shieldW - 12, shieldH - 12, 15, TFT_BLACK);
  
  // Blue banner at top with "ABARTH" text
  tft.fillRoundRect(centerX - 55, centerY - 60, 110, 25, 8, ABARTH_BLUE);
  tft.setTextColor(ABARTH_WHITE);
  tft.setTextSize(2);
  tft.setCursor(centerX - 35, centerY - 55);
  tft.print("ABARTH");
  
  // Yellow section (upper left quadrant)
  tft.fillRect(centerX - shieldW/2 + 8, centerY - 30, (shieldW - 16)/2, 50, ABARTH_YELLOW);
  
  // Red section (lower right, split with yellow)
  tft.fillRect(centerX, centerY - 30, (shieldW - 16)/2 - 8, 50, ABARTH_RED);
  tft.fillRect(centerX - shieldW/2 + 8, centerY + 20, shieldW - 16, 45, ABARTH_RED);
  
  // Enhanced scorpion silhouette (more detailed)
  // Main body
  tft.fillCircle(centerX - 10, centerY, 12, ABARTH_BLACK);
  tft.fillRect(centerX - 15, centerY - 5, 25, 10, ABARTH_BLACK);
  
  // Scorpion claws (more realistic shape)
  // Left claw
  tft.fillTriangle(centerX - 30, centerY - 18, centerX - 40, centerY - 25, centerX - 25, centerY - 8, ABARTH_BLACK);
  tft.fillCircle(centerX - 35, centerY - 20, 4, ABARTH_BLACK);
  
  // Right claw  
  tft.fillTriangle(centerX + 15, centerY - 18, centerX + 25, centerY - 25, centerX + 10, centerY - 8, ABARTH_BLACK);
  tft.fillCircle(centerX + 20, centerY - 20, 4, ABARTH_BLACK);
  
  // Scorpion body segments
  tft.fillRect(centerX - 8, centerY + 8, 16, 6, ABARTH_BLACK);
  tft.fillRect(centerX - 6, centerY + 14, 12, 5, ABARTH_BLACK);
  tft.fillRect(centerX - 4, centerY + 19, 8, 5, ABARTH_BLACK);
  tft.fillRect(centerX - 2, centerY + 24, 4, 4, ABARTH_BLACK);
  
  // Scorpion tail (curved to the right and up)
  tft.fillRect(centerX + 2, centerY + 28, 3, 10, ABARTH_BLACK);
  tft.fillRect(centerX + 5, centerY + 35, 3, 8, ABARTH_BLACK);
  tft.fillRect(centerX + 8, centerY + 40, 3, 6, ABARTH_BLACK);
  tft.fillRect(centerX + 11, centerY + 43, 5, 3, ABARTH_BLACK); // Stinger
  
  // Add decorative elements
  tft.drawFastHLine(centerX - 50, centerY - 35, 100, ABARTH_WHITE);
  tft.drawFastHLine(centerX - 50, centerY + 50, 100, ABARTH_WHITE);
  
  Serial.println("Enhanced Abarth vector logo displayed");
}

void drawAbarthLogoBitmap() {
  // For now, use the enhanced vector version as primary method
  // TODO: Future enhancement - convert BMP to RGB565 array for true bitmap display
  
  Serial.println("Drawing Abarth logo (vector-based)");
  drawAbarthLogoEnhanced();
  
  // Future implementation ideas:
  // Method 1: Convert your BMP to RGB565 C array using image2cpp
  // Method 2: Use SPIFFS with custom BMP decoder
  // Method 3: Use JPEG decoder library for compressed images
}

void drawAbarthLogoRGB() {
  // Load from SPIFFS file system
  drawAbarthLogoFromSPIFFS();
}

void drawAbarthLogoFromSPIFFS() {
  #ifdef USE_SPIFFS_BITMAP
    if (!SPIFFS.begin(true)) {
      Serial.println("SPIFFS Mount Failed - using vector logo");
      drawAbarthLogoVector();
      return;
    }
    
    // Check if logo file exists
    if (SPIFFS.exists("/abarth_logo.bmp")) {
      Serial.println("Found Abarth logo BMP file");
      
      // Try to decode and display the BMP
      if (drawBMPFromSPIFFS("/abarth_logo.bmp")) {
        Serial.println("BMP logo displayed successfully");
        return;
      } else {
        Serial.println("BMP decode failed - using vector fallback");
      }
    } else {
      Serial.println("Logo file not found - using vector logo");
    }
  #endif
  
  // Fallback to vector version
  drawAbarthLogoVector();
}

bool drawBMPFromSPIFFS(const char* filename) {
  File bmpFile = SPIFFS.open(filename, "r");
  if (!bmpFile) {
    Serial.println("Failed to open BMP file");
    return false;
  }
  
  // Read BMP header
  uint8_t header[54];
  if (bmpFile.read(header, 54) != 54) {
    Serial.println("Invalid BMP header");
    bmpFile.close();
    return false;
  }
  
  // Check BMP signature
  if (header[0] != 'B' || header[1] != 'M') {
    Serial.println("Not a valid BMP file");
    bmpFile.close();
    return false;
  }
  
  // Extract image info
  uint32_t fileSize = *(uint32_t*)&header[2];
  uint32_t dataOffset = *(uint32_t*)&header[10];
  uint32_t width = *(uint32_t*)&header[18];
  uint32_t height = *(uint32_t*)&header[22];
  uint16_t bitsPerPixel = *(uint16_t*)&header[28];
  
  Serial.printf("BMP Info: %dx%d, %d bits, offset: %d\n", width, height, bitsPerPixel, dataOffset);
  
  // Only support 24-bit BMPs for now
  if (bitsPerPixel != 24) {
    Serial.println("Only 24-bit BMPs supported");
    bmpFile.close();
    return false;
  }
  
  // Check if image fits our display
  if (width > 480 || height > 320) {
    Serial.println("BMP too large for display");
    bmpFile.close();
    return false;
  }
  
  // Seek to image data
  bmpFile.seek(dataOffset);
  
  // Calculate row padding (BMP rows are padded to 4-byte boundaries)
  uint32_t rowSize = ((width * 3 + 3) / 4) * 4;
  uint8_t* rowBuffer = (uint8_t*)malloc(rowSize);
  if (!rowBuffer) {
    Serial.println("Failed to allocate row buffer");
    bmpFile.close();
    return false;
  }
  
  // BMPs are stored bottom-to-top, so we read from bottom
  uint16_t* pixelBuffer = (uint16_t*)malloc(width * 2);
  if (!pixelBuffer) {
    Serial.println("Failed to allocate pixel buffer");
    free(rowBuffer);
    bmpFile.close();
    return false;
  }
  
  // Start drawing from center if image is smaller than display
  int startX = (480 - width) / 2;
  int startY = (320 - height) / 2;
  
  // Read and display each row
  for (int32_t y = height - 1; y >= 0; y--) {
    // Read row data
    if (bmpFile.read(rowBuffer, rowSize) != rowSize) {
      Serial.println("Failed to read row data");
      break;
    }
    
    // Convert 24-bit RGB to 16-bit RGB565
    for (uint32_t x = 0; x < width; x++) {
      uint8_t b = rowBuffer[x * 3];
      uint8_t g = rowBuffer[x * 3 + 1];
      uint8_t r = rowBuffer[x * 3 + 2];
      
      // Convert to RGB565
      uint16_t color = ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
      pixelBuffer[x] = color;
    }
    
    // Draw the row to display
    tft.pushImage(startX, startY + (height - 1 - y), width, 1, pixelBuffer);
  }
  
  free(rowBuffer);
  free(pixelBuffer);
  bmpFile.close();
  
  Serial.println("BMP displayed successfully");
  return true;
}

void showErrorScreen(String message) {
  tft.fillScreen(TFT_RED);
  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(3);
  tft.setCursor(80, 100);
  tft.println("ERROR!");
  tft.setTextSize(2);
  tft.setCursor(50, 150);
  tft.println(message);
}

void updateDisplay() {
  // TEMPORARY DEBUG: Force red background instead of black to verify function is called
  tft.fillScreen(TFT_RED);
  
  // Header - Fiat 500 Abarth specific
  tft.setTextColor(TFT_WHITE);  // White text on red background for visibility
  tft.setTextSize(3);
  tft.setCursor(100, 10);
  tft.println("ABARTH MONITOR");
  
  // Add sport mode indicator if active
  if (vehicle.sportMode) {
    tft.fillRect(410, 10, 60, 25, TFT_RED);
    tft.setTextColor(TFT_WHITE);
    tft.setTextSize(2);
    tft.setCursor(415, 15);
    tft.print("SPORT");
  }
  
  // Connection status indicator (CAN)
  tft.fillRect(10, 10, 20, 20, (millis() - vehicle.lastUpdate < CAN_TIMEOUT) ? TFT_GREEN : TFT_RED);
  
  // Power source indicator
  uint16_t powerColor = systemInfo.isUSBPowered ? TFT_CYAN : 
                       (systemInfo.powerSource.startsWith("External") ? TFT_YELLOW : TFT_ORANGE);
  tft.fillRect(35, 10, 20, 20, powerColor);
  
  // Add small labels for indicators
  tft.setTextColor(TFT_GRAY);
  tft.setTextSize(1);
  tft.setCursor(10, 35);
  tft.print("CAN");
  tft.setCursor(35, 35);
  tft.print("PWR");
  
  // Fiat 500 Abarth Turbo Dashboard Layout
  
  // BOOST PRESSURE - Most prominent (Top center)
  drawParameter("BOOST", String(vehicle.boostGauge, 2) + " bar", 180, 50, 4, 
                vehicle.boostGauge > 1.2 ? TFT_RED : (vehicle.boostGauge > 0.8 ? TFT_YELLOW : TFT_CYAN));
  
  // RPM (Top left)
  drawParameter("RPM", String((int)vehicle.rpm), 30, 60, 3, 
                vehicle.rpm > 6500 ? TFT_RED : (vehicle.rpm > 5000 ? TFT_YELLOW : TFT_GREEN));
  
  // Speed (Top right) 
  drawParameter("SPEED", String((int)vehicle.speed) + " km/h", 350, 60, 3,
                vehicle.speed > 140 ? TFT_RED : (vehicle.speed > 100 ? TFT_YELLOW : TFT_GREEN));
  
  // Turbo RPM (Middle left) - if available
  if (vehicle.turboRPM > 0) {
    drawParameter("TURBO RPM", String((int)vehicle.turboRPM), 30, 130, 2, 
                  vehicle.turboRPM > 200000 ? TFT_RED : (vehicle.turboRPM > 150000 ? TFT_YELLOW : TFT_CYAN));
  } else {
    drawParameter("ENGINE LOAD", String((int)vehicle.engineLoad) + "%", 30, 130, 2, TFT_MAGENTA);
  }
  
  // Temperature (Middle right)
  drawParameter("COOLANT", String((int)vehicle.coolantTemp) + "C", 350, 130, 2,
                vehicle.coolantTemp > 105 ? TFT_RED : (vehicle.coolantTemp > 95 ? TFT_YELLOW : TFT_GREEN));
  
  // Intercooler temp (Bottom left) - if available
  if (vehicle.intercoolerTemp > 0) {
    drawParameter("INTERCOOLER", String((int)vehicle.intercoolerTemp) + "C", 30, 200, 2, 
                  vehicle.intercoolerTemp > 60 ? TFT_RED : (vehicle.intercoolerTemp > 45 ? TFT_YELLOW : TFT_BLUE));
  } else {
    drawParameter("INTAKE TEMP", String((int)vehicle.intakeTemp) + "C", 30, 200, 2, TFT_BLUE);
  }
  
  // Throttle Position (Bottom right)
  drawParameter("THROTTLE", String((int)vehicle.throttlePos) + "%", 350, 200, 2, TFT_CYAN);
  
  // Status bar at bottom (line 1)
  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(1);
  tft.setCursor(10, 290);
  tft.print("CAN Bus: 500kbps | Last Update: ");
  if (millis() - vehicle.lastUpdate < CAN_TIMEOUT) {
    tft.setTextColor(TFT_GREEN);
    tft.print("ACTIVE");
  } else {
    tft.setTextColor(TFT_RED); 
    tft.print("NO DATA");
  }
  
  // Status bar (line 2) - Power source information
  tft.setTextColor(TFT_WHITE);
  tft.setCursor(10, 305);
  tft.print("Power: ");
  // Color code power source
  if (systemInfo.isUSBPowered) {
    tft.setTextColor(TFT_CYAN);
  } else if (systemInfo.powerSource.startsWith("External")) {
    tft.setTextColor(TFT_YELLOW);
  } else {
    tft.setTextColor(TFT_ORANGE);
  }
  tft.print(systemInfo.powerSource);
  
  // Add voltage reading
  tft.setTextColor(TFT_WHITE);
  tft.print(" (");
  tft.print(systemInfo.vinVoltage, 1);
  tft.print("V)");
  
  // Draw progress bars for turbo-specific values
  // Boost pressure bar (0-1.5 bar range for Abarth)
  float boostPercent = (vehicle.boostGauge / 1.5) * 100.0;  // Max 1.5 bar boost
  if (boostPercent < 0) boostPercent = 0;
  if (boostPercent > 100) boostPercent = 100;
  drawProgressBar(50, 260, 120, 18, boostPercent, "BOOST", 
                  vehicle.boostGauge > 1.0 ? TFT_RED : TFT_CYAN);
  
  // Throttle position bar
  drawProgressBar(200, 260, 100, 18, vehicle.throttlePos, "THROTTLE", TFT_YELLOW);
  
  // Engine load bar
  drawProgressBar(320, 260, 100, 18, vehicle.engineLoad, "LOAD", TFT_MAGENTA);
}

void drawParameter(String label, String value, int x, int y, int textSize, uint16_t color) {
  // Label
  tft.setTextColor(TFT_GRAY);
  tft.setTextSize(1);
  tft.setCursor(x, y);
  tft.println(label);
  
  // Value
  tft.setTextColor(color);
  tft.setTextSize(textSize);
  tft.setCursor(x, y + 15);
  tft.println(value);
}

void drawProgressBar(int x, int y, int width, int height, float percentage, String label, uint16_t color) {
  // Background
  tft.drawRect(x, y, width, height, TFT_WHITE);
  tft.fillRect(x + 1, y + 1, width - 2, height - 2, TFT_BLACK);
  
  // Fill based on percentage
  int fillWidth = (width - 2) * (percentage / 100.0);
  tft.fillRect(x + 1, y + 1, fillWidth, height - 2, color);
  
  // Label
  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(1);
  tft.setCursor(x, y - 10);
  tft.print(label);
  tft.print(": ");
  tft.print((int)percentage);
  tft.print("%");
}