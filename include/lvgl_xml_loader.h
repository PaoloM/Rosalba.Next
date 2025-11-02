// Needed for lv_obj_t
#include <lvgl.h>
#include <map>
#include <string>
extern std::map<std::string, lv_obj_t*> widget_map;
#pragma once
#include <lvgl.h>

void load_screen_from_xml(const char *xml_path, lv_obj_t *parent);

