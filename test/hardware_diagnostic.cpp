/**
 * Hardware Diagnostic Test - Step by Step Display Testing
 * This will systematically test each hardware component to isolate the issue
 */

#include <Arduino.h>
#include <TFT_eSPI.h>

TFT_eSPI tft = TFT_eSPI();

void setup() {
  Serial.begin(115200);
  delay(2000);
  
  Serial.println("\n╔══════════════════════════════════════╗");
  Serial.println("║    HARDWARE DIAGNOSTIC TEST          ║");
  Serial.println("║    White Screen Troubleshooting      ║");
  Serial.println("╚══════════════════════════════════════╝\n");
  
  // Test 1: GPIO Pin Control
  Serial.println("TEST 1: GPIO Pin Control");
  Serial.println("═══════════════════════════");
  
  pinMode(2, OUTPUT);  // DC pin
  pinMode(4, OUTPUT);  // Reset pin
  pinMode(15, OUTPUT); // CS pin
  
  Serial.println("Testing DC pin (GPIO2)...");
  digitalWrite(2, LOW);
  delay(100);
  digitalWrite(2, HIGH);
  Serial.println("✓ DC pin toggled");
  
  Serial.println("Testing CS pin (GPIO15)...");
  digitalWrite(15, LOW);
  delay(100);
  digitalWrite(15, HIGH);
  Serial.println("✓ CS pin toggled");
  
  Serial.println("Testing RESET pin (GPIO4) - WATCH DISPLAY!");
  Serial.println("  Display should go DARK when reset is LOW");
  digitalWrite(4, LOW);
  Serial.println("  -> RESET LOW (display should be dark now)");
  delay(3000);
  
  digitalWrite(4, HIGH);
  Serial.println("  -> RESET HIGH (display should show white/random)");
  delay(2000);
  
  Serial.println("Did the display change when reset was toggled? (Y/N)");
  Serial.println("If NO: Reset pin connection is wrong!\n");
  
  // Test 2: SPI Communication Test
  Serial.println("TEST 2: SPI Communication");
  Serial.println("═════════════════════════");
  
  // Initialize SPI manually
  SPI.begin(18, 19, 23, 15);  // SCK, MISO, MOSI, CS
  SPI.setFrequency(1000000);  // Start very slow - 1MHz
  Serial.println("SPI initialized at 1MHz");
  
  // Test basic SPI communication
  digitalWrite(15, LOW);  // Select display
  SPI.transfer(0x00);     // NOP command
  digitalWrite(15, HIGH); // Deselect
  Serial.println("✓ Basic SPI transfer completed");
  
  // Test 3: TFT Library Initialization
  Serial.println("\nTEST 3: TFT Library Test");
  Serial.println("════════════════════════");
  
  Serial.println("Calling tft.init()...");
  tft.init();
  Serial.println("✓ tft.init() completed");
  
  Serial.println("Testing fillScreen function...");
  
  // Test each basic color with delays
  uint16_t colors[] = {0xF800, 0x07E0, 0x001F, 0xFFE0, 0xF81F, 0x07FF, 0xFFFF, 0x0000};
  String colorNames[] = {"RED", "GREEN", "BLUE", "YELLOW", "MAGENTA", "CYAN", "WHITE", "BLACK"};
  
  for (int i = 0; i < 8; i++) {
    Serial.print("  -> ");
    Serial.print(colorNames[i]);
    tft.fillScreen(colors[i]);
    Serial.println(" (3 second delay - check display)");
    delay(3000);
  }
  
  // Test 4: Different rotations
  Serial.println("\nTEST 4: Display Rotation Test");
  Serial.println("═════════════════════════════");
  
  for (int rot = 0; rot < 4; rot++) {
    Serial.print("Rotation ");
    Serial.print(rot);
    tft.setRotation(rot);
    Serial.print(" (");
    Serial.print(tft.width());
    Serial.print("x");
    Serial.print(tft.height());
    Serial.println(")");
    
    tft.fillScreen(0x0000);  // Black
    tft.setTextColor(0xFFFF);  // White
    tft.setTextSize(3);
    tft.setCursor(10, 10);
    tft.print("ROT ");
    tft.println(rot);
    
    delay(2000);
  }
  
  // Test 5: Text and Graphics
  Serial.println("\nTEST 5: Graphics Test");
  Serial.println("════════════════════");
  
  tft.setRotation(3);  // Back to landscape
  tft.fillScreen(0x0000);
  
  Serial.println("Drawing rectangles...");
  tft.fillRect(50, 50, 100, 60, 0xF800);   // Red rectangle
  tft.fillRect(200, 100, 80, 40, 0x07E0);  // Green rectangle
  delay(2000);
  
  Serial.println("Drawing circles...");
  tft.drawCircle(400, 200, 50, 0x001F);    // Blue circle
  tft.fillCircle(100, 200, 30, 0xFFE0);    // Yellow filled circle
  delay(2000);
  
  Serial.println("Drawing text...");
  tft.setTextColor(0xFFFF);
  tft.setTextSize(2);
  tft.setCursor(150, 50);
  tft.println("DISPLAY TEST");
  tft.setCursor(150, 80);
  tft.println("Hardware OK?");
  
  Serial.println("\n╔══════════════════════════════════════╗");
  Serial.println("║            TEST COMPLETE             ║");
  Serial.println("╚══════════════════════════════════════╝");
  Serial.println("\nRESULTS ANALYSIS:");
  Serial.println("1. If reset test didn't work: Check GPIO4 ↔ Pin 22");
  Serial.println("2. If colors not visible: Check DC pin GPIO2 ↔ Pin 19");
  Serial.println("3. If nothing happens: Check power (5V) and SPI wires");
  Serial.println("4. If partial display: Try different driver (ILI9486/9488)");
  Serial.println("\nStarting continuous test...");
}

void loop() {
  // Continuous test - cycle through bright colors
  static unsigned long lastChange = 0;
  static int testPhase = 0;
  
  if (millis() - lastChange > 2000) {
    switch (testPhase % 6) {
      case 0:
        tft.fillScreen(0xF800);  // Red
        Serial.println("Loop test: RED");
        break;
      case 1:
        tft.fillScreen(0x07E0);  // Green
        Serial.println("Loop test: GREEN");
        break;
      case 2:
        tft.fillScreen(0x001F);  // Blue
        Serial.println("Loop test: BLUE");
        break;
      case 3:
        tft.fillScreen(0xFFE0);  // Yellow
        Serial.println("Loop test: YELLOW");
        break;
      case 4:
        tft.fillScreen(0xF81F);  // Magenta
        Serial.println("Loop test: MAGENTA");
        break;
      case 5:
        tft.fillScreen(0x07FF);  // Cyan
        Serial.println("Loop test: CYAN");
        break;
    }
    testPhase++;
    lastChange = millis();
    
    // Add text overlay
    tft.setTextColor(testPhase % 2 ? 0x0000 : 0xFFFF);  // Alternate black/white text
    tft.setTextSize(4);
    tft.setCursor(150, 140);
    tft.println("TESTING");
  }
}