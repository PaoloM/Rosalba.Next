#include <Arduino.h>

void setup() {
  Serial.begin(115200); 
  delay(1000);
  Serial.println("Rosalba.Next - Blank Canvas");
  Serial.println("============================");
  Serial.println("ESP32 ready for development");
}

void loop() {
  delay(1000);
  Serial.println("System running...");
}