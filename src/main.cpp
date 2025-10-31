
#include <LittleFS.h>
#include "display.h"
#include "driver_sdspi.h"
#include "main_ui.h"

#define SD_SCK   18
#define SD_MISO  19
#define SD_MOSI  23
#define SD_CS    5

Display screen;  // Create an instance of the Display class

void setup() {
  // Check if abarth_logo.png exists in LittleFS
  if (LittleFS.exists("/abarth_logo.png")) {
    Serial.println("Found abarth_logo.png in LittleFS!");
  } else {
    Serial.println("abarth_logo.png NOT FOUND in LittleFS!");
  }
  /* Prepare for possible serial debug */
  Serial.begin(115200);
  while (!Serial) {
    delay(10);
  }

  Serial.println("=== ROSALBA.NEXT SIMPLE DISPLAY ===");

  /*** Mount LittleFS before initializing LVGL or loading images ***/
  Serial.print("Mounting LittleFS... ");
  if (!LittleFS.begin()) {
    Serial.println("FAILED");
  } else {
    Serial.println("OK");
  }

  /*** Init drivers ***/
  sdspi_init(SD_SCK, SD_MISO, SD_MOSI, SD_CS);       //Initialize the SD module
  Serial.println("SD card initialized");

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