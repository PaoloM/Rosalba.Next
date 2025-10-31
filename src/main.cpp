#include "display.h"
#include "driver_sdspi.h"
#include "main_ui.h"

Display screen;  // Create an instance of the Display class

void setup() {
  /* Prepare for possible serial debug */
  Serial.begin(115200);
  while (!Serial) {
    delay(10);
  }

  Serial.println("=== ROSALBA.NEXT SIMPLE DISPLAY ===");

  /*** Init drivers ***/

  screen.init(); 
  Serial.println("Display initialized");

  // Use the simple, working UI
  setup_scr_main(&guider_main_ui);
  lv_screen_load(guider_main_ui.main);
  Serial.println("Simple UI loaded successfully");

  Serial.println("=== DISPLAY READY - SHOWING SIMPLE SCREEN ===");
}

void loop() {
  screen.routine(); /* Let the GUI do its work */
  delay(5);         /* Small delay to prevent excessive CPU usage */
}