#pragma once
#include <lvgl.h>
#include <tinyxml2.h>
#include <map>

extern std::map<std::string, lv_obj_t*> widget_map;
extern tinyxml2::XMLDocument* g_screen_xml_doc;
extern tinyxml2::XMLElement* g_screen_xml_root;

void create_widget_from_xml(tinyxml2::XMLElement* elem, lv_obj_t* parent);
void load_screen_from_xml(const char* xml_path, lv_obj_t* parent);
void recalculate_layout_tree(lv_obj_t* obj, tinyxml2::XMLElement* elem, lv_obj_t* parent);