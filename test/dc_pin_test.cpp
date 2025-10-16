/**
 * DC Pin Specific Test - This tests the most critical connection
 * The DC pin (Data/Command) is the #1 cause of white screen issues
 */

#include <Arduino.h>
#include <TFT_eSPI.h>

extern TFT_eSPI tft;

void setup() {
  Serial.begin(115200);
  delay(2000);
  
  Serial.println("\n═══════════════════════════════════");
  Serial.println("     DC PIN DIAGNOSTIC TEST");  
  Serial.println("═══════════════════════════════════");
  Serial.println("This tests the DC pin (GPIO2 ↔ Pin 19)");
  Serial.println("The DC pin is the #1 cause of white screens!");
  Serial.println();
  
  // Manual DC pin test
  Serial.println("STEP 1: Manual DC Pin Test");
  Serial.println("---------------------------");
  
  pinMode(2, OUTPUT);   // DC pin
  pinMode(4, OUTPUT);   // Reset pin  
  pinMode(15, OUTPUT);  // CS pin
  
  // Reset sequence
  digitalWrite(4, LOW);
  delay(100);
  digitalWrite(4, HIGH);
  delay(100);
  
  // Initialize SPI
  SPI.begin(18, 19, 23, 15);
  SPI.setFrequency(1000000);  // 1MHz - very slow for reliability
  
  Serial.println("Sending command with DC LOW (command mode):");
  digitalWrite(15, LOW);  // Select display
  digitalWrite(2, LOW);   // DC LOW = Command mode
  SPI.transfer(0x01);     // Software reset command
  digitalWrite(15, HIGH); // Deselect
  delay(100);
  
  Serial.println("Sending data with DC HIGH (data mode):");
  digitalWrite(15, LOW);  // Select display  
  digitalWrite(2, HIGH);  // DC HIGH = Data mode
  SPI.transfer(0xFF);     // Some data
  digitalWrite(15, HIGH); // Deselect
  delay(100);
  
  Serial.println("✓ Manual DC test completed");
  Serial.println();
  
  // Test with TFT library
  Serial.println("STEP 2: TFT Library with DC Test");
  Serial.println("---------------------------------");
  
  Serial.println("Initializing display...");
  tft.init();
  
  Serial.println("Testing different DC states during drawing...");
  
  // Force DC manipulation during drawing
  for (int i = 0; i < 5; i++) {
    Serial.print("Test ");
    Serial.print(i + 1);
    Serial.println(": Filling screen...");
    
    uint16_t colors[] = {0xF800, 0x07E0, 0x001F, 0xFFE0, 0xF81F};
    String names[] = {"RED", "GREEN", "BLUE", "YELLOW", "MAGENTA"};
    
    Serial.print("  -> Drawing ");
    Serial.println(names[i]);
    
    tft.fillScreen(colors[i]);
    delay(2000);
    
    // Manual DC toggle to see if it affects display
    digitalWrite(2, LOW);
    delayMicroseconds(100);
    digitalWrite(2, HIGH);
  }
  
  Serial.println();
  Serial.println("STEP 3: Text Test with DC Verification");
  Serial.println("---------------------------------------");
  
  tft.fillScreen(0x0000);  // Black background
  tft.setTextColor(0xFFFF, 0x0000);  // White text on black
  tft.setTextSize(3);
  tft.setCursor(50, 50);
  tft.println("DC PIN TEST");
  
  tft.setTextSize(2);
  tft.setCursor(50, 100);
  tft.println("GPIO2 -> Pin 19");
  
  tft.setCursor(50, 130);
  tft.println("Check this text!");
  
  Serial.println("Text drawn - check display!");
  Serial.println();
  
  Serial.println("═══════════════════════════════════");
  Serial.println("        DIAGNOSIS RESULTS");
  Serial.println("═══════════════════════════════════");
  Serial.println("If display is STILL WHITE after all tests:");
  Serial.println();
  Serial.println("🔥 MOST LIKELY: DC pin not connected!");
  Serial.println("   Check: ESP32 GPIO2 ↔ Display Pin 19");
  Serial.println();  
  Serial.println("📋 Wiring checklist:");
  Serial.println("   ✓ ESP32 GPIO2  → Display Pin 19 (DC/RS)");
  Serial.println("   ✓ ESP32 GPIO4  → Display Pin 22 (RESET)");
  Serial.println("   ✓ ESP32 5V     → Display Pin 1,2 (POWER)");
  Serial.println("   ✓ ESP32 GND    → Display Pin 35,36 (GND)");
  Serial.println("   ✓ ESP32 GPIO15 → Display Pin 21 (CS)");
  Serial.println("   ✓ ESP32 GPIO23 → Display Pin 32 (MOSI)");
  Serial.println("   ✓ ESP32 GPIO18 → Display Pin 33 (SCK)");
  Serial.println();
  Serial.println("Starting continuous test...");
}

void loop() {
  // Simple alternating test
  static unsigned long lastToggle = 0;
  static bool state = false;
  
  if (millis() - lastToggle > 1000) {
    if (state) {
      tft.fillScreen(0x001F);  // Blue
      tft.setTextColor(0xFFFF);
      tft.setTextSize(4);
      tft.setCursor(100, 120);
      tft.println("BLUE TEST");
      Serial.println("Display: BLUE with text");
    } else {
      tft.fillScreen(0xF800);  // Red
      tft.setTextColor(0xFFFF);
      tft.setTextSize(4);
      tft.setCursor(100, 120);  
      tft.println("RED TEST");
      Serial.println("Display: RED with text");
    }
    
    state = !state;
    lastToggle = millis();
  }
}