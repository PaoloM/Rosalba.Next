#pragma once
#include <lvgl.h>
#include <tinyxml2.h>
#include <map>

void load_screen_from_xml(const char* xml_path, lv_obj_t* parent);
extern std::map<std::string, lv_obj_t*> widget_map;
// Forward declaration for widget creation
void create_widget_from_xml(tinyxml2::XMLElement* elem, lv_obj_t* parent);
