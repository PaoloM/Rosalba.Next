#include "display.h"
#include "driver_sdspi.h"
#include "main_ui.h"

#define SD_SCK   18
#define SD_MISO  19
#define SD_MOSI  23
#define SD_CS    5

Display screen;  // Create an instance of the Display class

void setup() {
  /* Prepare for possible serial debug */
  Serial.begin(115200);
  while (!Serial) {
    delay(10);
  }

  /*** Init drivers ***/
  sdspi_init(SD_SCK, SD_MISO, SD_MOSI, SD_CS);       //Initialize the SD module

  screen.init();

  setup_scr_main(&guider_main_ui);
  lv_screen_load(guider_main_ui.main);

}

void loop() {
  screen.routine(); /* Let the GUI do its work */  // Handle routine display tasks
  delay(5);                                        // Add a small delay to prevent the loop from running too fast
}