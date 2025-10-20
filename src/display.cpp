#include "display.h"
#include <Arduino_GFX_Library.h>
#include "TCA9554.h"

// Pin definitions from working example
#define LCD_QSPI_CS   12
#define LCD_QSPI_CLK  5
#define LCD_QSPI_D0   1
#define LCD_QSPI_D1   2
#define LCD_QSPI_D2   3
#define LCD_QSPI_D3   4
#define GFX_BL        6   // Backlight control

// I2C pins from working example
#define I2C_SDA      21   
#define I2C_SCL      22   // Correct pin from example

// TCA9554 setup from working example
TCA9554 TCA(0x20);  // Correct address from example

// Display objects from working example
Arduino_DataBus *bus = nullptr;
Arduino_GFX *g = nullptr;
Arduino_Canvas *gfx = nullptr;

void initializeDisplay() {
    Serial.println("=== DISPLAY INITIALIZATION (Working Example Method) ===");
    
    // Step 1: Initialize I2C with correct pins from working example
    Serial.println("Step 1: Initializing I2C...");
    Wire.begin(I2C_SDA, I2C_SCL);  // 21, 22 from working example
    
    // Step 2: Initialize TCA9554 I2C expander
    Serial.println("Step 2: Initializing TCA9554...");
    if (!TCA.begin()) {
        Serial.println("ERROR: TCA9554 initialization failed!");
        return;
    }
    Serial.println("TCA9554 initialized successfully");
    
    // Step 3: Display reset sequence from working example
    Serial.println("Step 3: Display reset sequence...");
    TCA.pinMode1(1, OUTPUT);
    TCA.write1(1, 1);
    delay(10);
    TCA.write1(1, 0);
    delay(10);
    TCA.write1(1, 1);
    delay(200);
    Serial.println("Display reset sequence complete");
    
    // Step 4: Initialize display hardware
    Serial.println("Step 4: Creating display objects...");
    bus = new Arduino_ESP32QSPI(LCD_QSPI_CS, LCD_QSPI_CLK, LCD_QSPI_D0, LCD_QSPI_D1, LCD_QSPI_D2, LCD_QSPI_D3);
    g = new Arduino_AXS15231B(bus, -1 /* RST */, 0 /* rotation */, false, 320, 480);
    
    // Step 5: Start with basic display (no canvas yet to avoid memory issues)
    Serial.println("Step 5: Starting display...");
    if (!g->begin()) {
        Serial.println("ERROR: Display begin() failed!");
        return;
    }
    Serial.println("Display started successfully!");
    
    // Step 6: Basic test - fill screen with color
    Serial.println("Step 6: Testing basic display...");
    g->fillScreen(0x001F);  // Blue color
    Serial.println("Blue screen displayed");
    
    // Step 7: Enable backlight
    Serial.println("Step 7: Enabling backlight...");
    pinMode(GFX_BL, OUTPUT);
    digitalWrite(GFX_BL, HIGH);
    Serial.println("Backlight enabled");
    
    delay(2000);
    
    // Step 8: Test colors
    Serial.println("Step 8: Testing colors...");
    g->fillScreen(0xF800);  // Red
    delay(1000);
    g->fillScreen(0x07E0);  // Green  
    delay(1000);
    g->fillScreen(0x0000);  // Black
    delay(1000);
    
    Serial.println("=== BASIC DISPLAY TEST COMPLETE ===");
    Serial.println("Display is working! Canvas will be added once stable.");
}

void clearScreen() {
  Serial.println("Display: Clear screen");
}

void setTextStyle(uint16_t color, int size) {
  Serial.printf("Display: Set text style - color: 0x%04X, size: %d\n", color, size);
}

void showAbarthLogo() {
  Serial.println("Display: Showing Abarth logo");
  clearScreen();
  Serial.println("Display: Drawing Abarth branding");
  delay(2000); // Show logo for 2 seconds
}

void showStartupScreen() {
  Serial.println("Display: Showing startup screen");
  clearScreen();
  
  Serial.println("Display: ROSALBA.NEXT");
  Serial.println("Display: Fiat 500 Abarth CAN Monitor");
  Serial.println("Display: ESP32-S3 | v2.0");
  Serial.println("Display: Initializing...");
  
  // Simulate initialization progress
  for (int i = 0; i <= 100; i += 10) {
    Serial.printf("Display: Progress %d%%\n", i);
    delay(100);
  }
  
  Serial.println("Display: Ready!");
  delay(1000);
}

void drawAbarthLogo(int x, int y, int scale) {
  Serial.printf("Display: Drawing Abarth logo at (%d,%d) scale %d\n", x, y, scale);
}

void drawScorpion(int x, int y, int scale) {
  Serial.printf("Display: Drawing scorpion at (%d,%d) scale %d\n", x, y, scale);
}

void drawAbarthText(int x, int y, int scale) {
  Serial.printf("Display: Drawing ABARTH text at (%d,%d) scale %d\n", x, y, scale);
}

void drawProgressBar(int x, int y, int width, int height, int percentage, uint16_t color) {
  Serial.printf("Display: Progress bar %d%% at (%d,%d) %dx%d\n", percentage, x, y, width, height);
}

void displayConnectionStatus(bool connected) {
  if (connected) {
    Serial.println("Display: CAN CONNECTED");
  } else {
    Serial.println("Display: CAN DISCONNECTED");
  }
}

void displayVehicleData(const VehicleData& data) {
  Serial.println("Display: ABARTH PERFORMANCE");
  Serial.printf("Display: RPM: %d\n", (int)data.rpm);
  Serial.printf("Display: SPEED: %d km/h\n", (int)data.speed);
  Serial.printf("Display: BOOST: %d kPa\n", (int)data.boostPressure);
  Serial.printf("Display: THROTTLE: %d%%\n", (int)data.throttlePos);
  Serial.printf("Display: COOLANT: %d°C\n", (int)data.coolantTemp);
  Serial.printf("Display: INTAKE: %d°C\n", (int)data.intakeTemp);
  
  if (data.sportMode) {
    Serial.println("Display: ⚡ SPORT MODE ⚡");
  }
}

void updateDisplay() {
  // Display update placeholder - will be implemented with actual hardware
  Serial.println("Display: Update display (placeholder)");
}