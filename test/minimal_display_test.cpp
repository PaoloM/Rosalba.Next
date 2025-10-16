/**
 * Minimal Display Test - White Screen Debug
 * Replace main.cpp temporarily with this to isolate display issues
 */

#include <Arduino.h>
#include <TFT_eSPI.h>

TFT_eSPI tft = TFT_eSPI();

void setup() {
  Serial.begin(115200);
  delay(2000);
  
  Serial.println("\n=== MINIMAL DISPLAY TEST ===");
  
  // Test 1: Pin control
  Serial.println("1. Testing pin control...");
  pinMode(4, OUTPUT);  // Reset pin
  digitalWrite(4, LOW);
  delay(100);
  digitalWrite(4, HIGH);
  delay(100);
  Serial.println("   Reset pulse sent");
  
  // Test 2: Basic init
  Serial.println("2. Initializing display...");
  tft.init();
  Serial.println("   tft.init() completed");
  
  // Test 3: Rotation
  Serial.println("3. Setting rotation...");
  tft.setRotation(0);  // Try portrait first
  Serial.println("   Rotation set to 0 (portrait)");
  
  // Test 4: Simple color test
  Serial.println("4. Testing colors...");
  
  Serial.println("   -> RED");
  tft.fillScreen(0xF800);  // Pure red
  delay(2000);
  
  Serial.println("   -> GREEN");
  tft.fillScreen(0x07E0);  // Pure green
  delay(2000);
  
  Serial.println("   -> BLUE");
  tft.fillScreen(0x001F);  // Pure blue
  delay(2000);
  
  Serial.println("   -> BLACK");
  tft.fillScreen(0x0000);  // Black
  delay(2000);
  
  // Test 5: Try different rotation
  Serial.println("5. Testing landscape mode...");
  tft.setRotation(3);
  tft.fillScreen(0xFFE0);  // Yellow
  delay(2000);
  
  // Test 6: Text test
  Serial.println("6. Testing text...");
  tft.fillScreen(0x0000);
  tft.setTextColor(0xFFFF);
  tft.setTextSize(2);
  tft.setCursor(10, 10);
  tft.println("HELLO WORLD");
  tft.setCursor(10, 40);
  tft.print("Size: ");
  tft.print(tft.width());
  tft.print("x");
  tft.println(tft.height());
  
  Serial.println("=== TEST COMPLETE ===");
  Serial.println("If display still white:");
  Serial.println("1. Check wiring to pin 19 (DC)");
  Serial.println("2. Verify 5V power supply");
  Serial.println("3. Try ILI9488_DRIVER instead");
}

void loop() {
  // Flash different colors to show it's running
  static unsigned long lastFlash = 0;
  static bool state = false;
  
  if (millis() - lastFlash > 1000) {
    if (state) {
      tft.fillScreen(0x07E0);  // Green
      Serial.println("Flash: GREEN");
    } else {
      tft.fillScreen(0x001F);  // Blue  
      Serial.println("Flash: BLUE");
    }
    state = !state;
    lastFlash = millis();
  }
}