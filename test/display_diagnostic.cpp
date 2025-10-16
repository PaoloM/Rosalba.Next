/**
 * Display Diagnostic Test - White Screen Troubleshooting
 * 
 * This diagnostic program helps identify why the display shows white.
 * It tests each component systematically and provides detailed output.
 * 
 * Upload this instead of main.cpp to diagnose display issues.
 */

#include <Arduino.h>
#include <TFT_eSPI.h>

// Use the same pin definitions as your project
#define TFT_MISO 19
#define TFT_MOSI 23
#define TFT_SCLK 18
#define TFT_CS   15
#define TFT_DC   2
#define TFT_RST  4

TFT_eSPI tft = TFT_eSPI();

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  Serial.println("\n========================================");
  Serial.println("DISPLAY DIAGNOSTIC TEST");
  Serial.println("Troubleshooting White Screen Issues");
  Serial.println("========================================\n");
  
  // Step 1: Test pin assignments
  Serial.println("Step 1: Pin Configuration Check");
  Serial.print("TFT_MISO: GPIO"); Serial.println(TFT_MISO);
  Serial.print("TFT_MOSI: GPIO"); Serial.println(TFT_MOSI);
  Serial.print("TFT_SCLK: GPIO"); Serial.println(TFT_SCLK);
  Serial.print("TFT_CS:   GPIO"); Serial.println(TFT_CS);
  Serial.print("TFT_DC:   GPIO"); Serial.println(TFT_DC);
  Serial.print("TFT_RST:  GPIO"); Serial.println(TFT_RST);
  Serial.println();
  
  // Step 2: Test reset pin control
  Serial.println("Step 2: Testing Reset Pin Control");
  pinMode(TFT_RST, OUTPUT);
  
  Serial.println("- Pulling RESET low (display should go dark)");
  digitalWrite(TFT_RST, LOW);
  delay(500);
  
  Serial.println("- Releasing RESET high (display should show white/random)");
  digitalWrite(TFT_RST, HIGH);
  delay(1000);
  Serial.println("  Did display change? (dark->white means reset working)\n");
  
  // Step 3: Initialize display
  Serial.println("Step 3: Display Initialization");
  Serial.println("- Calling tft.init()...");
  
  try {
    tft.init();
    Serial.println("  ✓ tft.init() completed without error");
  } catch (...) {
    Serial.println("  ✗ tft.init() threw exception!");
  }
  
  delay(1000);
  
  // Step 4: Test different orientations
  Serial.println("\nStep 4: Testing Display Orientations");
  for (int rotation = 0; rotation < 4; rotation++) {
    Serial.print("- Testing rotation "); Serial.print(rotation);
    tft.setRotation(rotation);
    
    // Fill with different colors for each rotation
    uint16_t colors[] = {0xF800, 0x07E0, 0x001F, 0xFFE0}; // Red, Green, Blue, Yellow
    tft.fillScreen(colors[rotation]);
    
    Serial.print(" ("); 
    Serial.print(tft.width());
    Serial.print("x");
    Serial.print(tft.height());
    Serial.println(")");
    
    delay(2000);
  }
  
  // Step 5: Test basic drawing functions
  Serial.println("\nStep 5: Testing Basic Drawing Functions");
  tft.setRotation(3); // Back to landscape
  
  Serial.println("- Black screen");
  tft.fillScreen(TFT_BLACK);
  delay(1000);
  
  Serial.println("- White screen");
  tft.fillScreen(TFT_WHITE);
  delay(1000);
  
  Serial.println("- Red screen");
  tft.fillScreen(TFT_RED);
  delay(1000);
  
  Serial.println("- Green screen");
  tft.fillScreen(TFT_GREEN);
  delay(1000);
  
  Serial.println("- Blue screen");
  tft.fillScreen(TFT_BLUE);
  delay(1000);
  
  // Step 6: Test text rendering
  Serial.println("\nStep 6: Testing Text Rendering");
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(2);
  tft.setCursor(10, 10);
  tft.println("DIAGNOSTIC TEST");
  tft.setCursor(10, 40);
  tft.print("Resolution: ");
  tft.print(tft.width());
  tft.print("x");
  tft.println(tft.height());
  
  Serial.println("- Text rendered on black background");
  delay(3000);
  
  // Step 7: Test geometric shapes
  Serial.println("\nStep 7: Testing Geometric Shapes");
  tft.fillScreen(TFT_BLACK);
  
  // Draw rectangles
  tft.fillRect(50, 50, 100, 80, TFT_RED);
  tft.fillRect(200, 100, 120, 60, TFT_GREEN);
  tft.drawCircle(400, 200, 50, TFT_BLUE);
  
  Serial.println("- Shapes drawn (red rect, green rect, blue circle)");
  delay(3000);
  
  // Final result
  Serial.println("\n========================================");
  Serial.println("DIAGNOSTIC COMPLETE");
  Serial.println("========================================");
  Serial.println("\nIf you see colors and shapes, display is working!");
  Serial.println("If still white, check wiring and driver type.");
  Serial.println("\nCommon issues:");
  Serial.println("1. DC pin (GPIO2) not connected to display pin 19");
  Serial.println("2. Wrong driver (try ILI9486 vs ILI9488)");
  Serial.println("3. Power supply insufficient (need 5V)");
  Serial.println("4. Loose connections on SPI pins");
  
  // Show final test pattern
  drawTestPattern();
}

void loop() {
  // Cycle through colors to confirm continuous operation
  static unsigned long lastChange = 0;
  static int colorIndex = 0;
  uint16_t testColors[] = {TFT_RED, TFT_GREEN, TFT_BLUE, TFT_YELLOW, TFT_CYAN, TFT_MAGENTA};
  
  if (millis() - lastChange > 2000) {
    tft.fillScreen(testColors[colorIndex]);
    colorIndex = (colorIndex + 1) % 6;
    lastChange = millis();
    
    Serial.print("Color cycle: ");
    Serial.println(colorIndex);
  }
}

void drawTestPattern() {
  Serial.println("\nDrawing final test pattern...");
  
  tft.fillScreen(TFT_BLACK);
  
  // Draw grid pattern
  for (int x = 0; x < tft.width(); x += 40) {
    tft.drawFastVLine(x, 0, tft.height(), TFT_DARKGRAY);
  }
  for (int y = 0; y < tft.height(); y += 40) {
    tft.drawFastHLine(0, y, tft.width(), TFT_DARKGRAY);
  }
  
  // Draw title
  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(3);
  tft.setCursor(120, 50);
  tft.println("DISPLAY OK!");
  
  tft.setTextSize(2);
  tft.setCursor(100, 100);
  tft.print("Resolution: ");
  tft.print(tft.width());
  tft.print(" x ");
  tft.println(tft.height());
  
  tft.setCursor(140, 140);
  tft.setTextColor(TFT_GREEN);
  tft.println("Test Passed");
  
  tft.setCursor(80, 180);
  tft.setTextColor(TFT_CYAN);
  tft.println("Check Serial Monitor");
  
  // Draw color bars at bottom
  int barWidth = tft.width() / 6;
  uint16_t colors[] = {TFT_RED, TFT_GREEN, TFT_BLUE, TFT_YELLOW, TFT_CYAN, TFT_MAGENTA};
  for (int i = 0; i < 6; i++) {
    tft.fillRect(i * barWidth, tft.height() - 40, barWidth, 40, colors[i]);
  }
}