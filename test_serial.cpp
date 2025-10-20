/**
 * Simple ESP32-S3 Serial Test
 * Tests basic serial communication without CAN
 */

#include <Arduino.h>

void setup() {
  // Initialize Serial with USB CDC
  Serial.begin(115200);
  delay(3000); // Long delay for USB CDC
  
  Serial.println();
  Serial.println("========================================");
  Serial.println("ESP32-S3 SERIAL TEST - SUCCESS!");
  Serial.println("========================================");
  Serial.println("Board: Waveshare ESP32-S3");
  Serial.println("Baud Rate: 115200");
  Serial.println("USB CDC: Enabled");
  Serial.println();
}

void loop() {
  static int counter = 0;
  
  Serial.print("Loop counter: ");
  Serial.println(counter++);
  Serial.print("Millis: ");
  Serial.println(millis());
  Serial.println();
  
  delay(2000);
}