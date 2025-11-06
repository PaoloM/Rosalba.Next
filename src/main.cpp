#include "display.h"
#include "config.h"
#include "driver_sdspi.h"
#include <LittleFS.h>
#include "lvgl_xml_screens.h"
#include "Arduino.h"
#include <map>
#include <string>

Display screen; // Create an instance of the Display class

std::map<std::string, std::string> binding_map;
std::map<std::string, std::string> fgcolor_map;
std::map<std::string, std::string> bgcolor_map;

extern bool execute_layout;

int simulated_rpm = 0;

void show_splash_then_vehicle()
{
  lv_obj_t* splash = lv_scr_act();
  load_screen_from_xml("/splash.xml", splash);
  // No need to call lv_scr_load(splash) since it's already active
  lv_timer_t *timer = lv_timer_create(
      [](lv_timer_t *t)
      {
        lv_timer_del(t);
        lv_obj_t* vehicle = lv_scr_act();
        // Remove all children from the screen before loading new widgets
        while (lv_obj_get_child_cnt(vehicle) > 0) {
          lv_obj_del(lv_obj_get_child(vehicle, 0));
        }
        load_screen_from_xml("/main_1.xml", vehicle);
        // No need to call lv_scr_load(vehicle) since it's already active
      },
      SPLASH_SCREEN_DURATION_MS, NULL);
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

// initialize data_map, fgcolor_map and bgcolor_map
#if BENCH_TESTING == true
  data_map["mode"] = "BENCH";
#else
  binding_map["mode"] = "ACTIVE";
#endif

  binding_map["rpm"] = "0";
  binding_map["version"] = VERSION;
  binding_map["can"] = LV_SYMBOL_OK;
  fgcolor_map["can"] = "#00aa00";

  // data_map["can"] = LV_SYMBOL_WARNING;
  // fgcolor_map["can"] = "#ff3333";

  show_splash_then_vehicle();
}

void data_collection()
{
#if BENCH_TESTING == true
  // simulated data collection
  simulated_rpm += 100;
  if (simulated_rpm > 7000)
    simulated_rpm = 0;

  data_map["rpm"] = std::to_string(simulated_rpm);
#else
  // Normal data update logic here, getting values directly from CAN bus
#endif
}

void label_data_binding(const std::string &widget_name, const std::string &value)
{
  auto it = widget_map.find(widget_name);
  if (it != widget_map.end())
  {
    lv_obj_t *label = it->second;
    lv_label_set_text(label, value.c_str());
  }
}

void fgcolor_data_binding(const std::string &widget_name, const std::string &value)
{
  auto it = fgcolor_map.find(widget_name);
  if (it != fgcolor_map.end())
  {
    auto w_it = widget_map.find(widget_name);
    if (w_it != widget_map.end())
    {
      if (w_it->second != nullptr)
      {
        lv_obj_t *label = w_it->second;
        const std::string& color_str = it->second;
        // Validate color string: must start with '#' and be 7 chars (#RRGGBB)
        if (color_str.size() == 7 && color_str[0] == '#')
        {
          char *endptr = nullptr;
          uint32_t col = (uint32_t)strtol(color_str.c_str() + 1, &endptr, 16);
          if (endptr == color_str.c_str() + 7)
          {
            lv_obj_set_style_text_color(label, lv_color_hex(col), LV_PART_MAIN | LV_STATE_DEFAULT);
          }
        }
      }
    }
  }
}

void data_binding_update()
{
  // labels text
  for (const auto &pair : binding_map)
  {
    label_data_binding(pair.first, pair.second);
  }

  // foreground colors
  for (const auto &pair : fgcolor_map)
  {
    fgcolor_data_binding(pair.first, pair.second);
  }
}

void Display::routine()
{
  static uint32_t last_update = 0;
  uint32_t now = lv_tick_get();
  if (now - last_update >= SCREEN_REFRESH_INTERVAL_MS) // Update every 100ms
  {
    last_update = now;
    data_binding_update();
  }
}

void loop()
{
  if (execute_layout)
  {
    lv_task_handler(); // Handle LVGL tasks before layout
    extern tinyxml2::XMLElement* g_screen_xml_root;
    lv_obj_t *parent = lv_scr_act();
    if (g_screen_xml_root && parent) {
        uint32_t child_cnt = lv_obj_get_child_cnt(parent);
        tinyxml2::XMLElement *elem = g_screen_xml_root->FirstChildElement();
        for (uint32_t i = 0; i < child_cnt && elem; ++i) {
            lv_obj_t *child_obj = lv_obj_get_child(parent, i);
            recalculate_layout_tree(child_obj, elem, parent);
            elem = elem->NextSiblingElement();
        }
    }
    lv_task_handler(); // Handle LVGL tasks after layout
    execute_layout = false;
  }
  data_collection();
  screen.routine(); /* Let the GUI do its work */
  lv_tick_inc(5);   // Call every 5ms (matches your delay)
  lv_timer_handler();
  delay(5); /* Small delay to prevent excessive CPU usage */
}