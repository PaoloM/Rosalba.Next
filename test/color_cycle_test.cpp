/**
 * Display Color Cycle Test
 * This will continuously cycle through bright colors to verify display output
 * Replace main.cpp temporarily if you want to test just display functionality
 */

#include <Arduino.h>
#include <TFT_eSPI.h>

TFT_eSPI tft = TFT_eSPI();

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  Serial.println("=== DISPLAY COLOR CYCLE TEST ===");
  
  // Initialize display
  tft.init();
  tft.setRotation(3);  // Landscape
  
  Serial.println("Display initialized - starting color cycle");
  Serial.println("Should see: Red -> Green -> Blue -> Yellow -> Cyan -> Magenta -> White -> Black");
}

void loop() {
  // Bright, easily visible colors
  uint16_t colors[] = {
    0xF800,  // Red
    0x07E0,  // Green  
    0x001F,  // Blue
    0xFFE0,  // Yellow
    0x07FF,  // Cyan
    0xF81F,  // Magenta
    0xFFFF,  // White
    0x0000   // Black
  };
  
  String colorNames[] = {
    "RED", "GREEN", "BLUE", "YELLOW", 
    "CYAN", "MAGENTA", "WHITE", "BLACK"
  };
  
  for (int i = 0; i < 8; i++) {
    Serial.print("Showing: ");
    Serial.println(colorNames[i]);
    
    tft.fillScreen(colors[i]);
    
    // Add text overlay (except on white)
    if (i != 6) {  // Skip text on white background
      tft.setTextColor(0xFFFF);  // White text
      tft.setTextSize(3);
      tft.setCursor(150, 140);
      tft.println(colorNames[i]);
    } else {
      // Black text on white background
      tft.setTextColor(0x0000);
      tft.setTextSize(3);
      tft.setCursor(150, 140);
      tft.println(colorNames[i]);
    }
    
    delay(2000);  // 2 seconds per color
  }
  
  Serial.println("Color cycle complete - repeating...");
}