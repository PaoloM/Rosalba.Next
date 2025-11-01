#include "display.h"
#include "driver_sdspi.h"
#include <LittleFS.h>
#include "lvgl_xml_loader.h"

Display screen;       // Create an instance of the Display class
//lv_ui guider_main_ui; // Global UI instance for LVGL

void show_splash_then_vehicle()
{
  lv_obj_t* splash = lv_obj_create(NULL);
  load_screen_from_xml("/splash.xml", splash);
  lv_scr_load(splash);
  lv_timer_t *timer = lv_timer_create(
      [](lv_timer_t *t)
      {
        lv_timer_del(t);
        lv_obj_t* vehicle = lv_obj_create(NULL);
        load_screen_from_xml("/main_1.xml", vehicle);
        lv_scr_load(vehicle);
      },
      5000, NULL); // 5 seconds
}

void setup()
{
  /* Prepare for possible serial debug */
  Serial.begin(115200);
  while (!Serial)
  {
    delay(10);
  }

  Serial.println("=== ROSALBA.NEXT ===");

  /*** Init drivers ***/

  screen.init();
  Serial.println("Display initialized");

  Serial.print("Mounting LittleFS... ");
  static bool littlefs_mounted = false;
  if (!littlefs_mounted)
  {
    if (!LittleFS.begin())
    {
      Serial.println("FAILED");
    }
    else
    {
      Serial.println("OK");
      littlefs_mounted = true;
    }
  }
  else
  {
    Serial.println("ALREADY MOUNTED");
  }

  show_splash_then_vehicle();
}

void loop()
{
  screen.routine(); /* Let the GUI do its work */
  lv_tick_inc(5);   // Call every 5ms (matches your delay)
  lv_timer_handler();
  delay(5);         /* Small delay to prevent excessive CPU usage */
}