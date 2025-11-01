#include "lvgl_xml_loader.h"
#include <lvgl.h>
#include <LittleFS.h>
#include <Arduino.h>

#include <tinyxml2.h>
using namespace tinyxml2;

void load_screen_from_xml(const char* xml_path, lv_obj_t* parent) {
    File file = LittleFS.open(xml_path, "r");
    if (!file) {
        Serial.println("Failed to open XML file");
        return;
    }
    String xmlContent;
    while (file.available()) {
        xmlContent += (char)file.read();
    }
    file.close();

    XMLDocument doc;
    if (doc.Parse(xmlContent.c_str()) != XML_SUCCESS) {
        Serial.println("Failed to parse XML");
        return;
    }
    XMLElement* screen = doc.FirstChildElement("screen");
    if (!screen) return;

    for (XMLElement* elem = screen->FirstChildElement(); elem; elem = elem->NextSiblingElement()) {
        const char* type = elem->Name();
        if (strcmp(type, "label") == 0) {
            int x = elem->IntAttribute("x", 0);
            int y = elem->IntAttribute("y", 0);
            const char* text = elem->Attribute("text");
            const char* color = elem->Attribute("color");
            lv_obj_t* label = lv_label_create(parent);
            lv_label_set_text(label, text ? text : "");
            lv_obj_set_pos(label, x, y);
            if (color) {
                uint32_t col = (uint32_t)strtol(color+1, NULL, 16); // skip '#'
                lv_obj_set_style_text_color(label, lv_color_hex(col), LV_PART_MAIN | LV_STATE_DEFAULT);
            }
        } else if (strcmp(type, "image") == 0) {
            int x = elem->IntAttribute("x", 0);
            int y = elem->IntAttribute("y", 0);
            const char* src = elem->Attribute("src");
            // Check if file exists in LittleFS
            String fs_path = String(src);
            if (fs_path.startsWith("L:/")) {
                fs_path = fs_path.substring(2); // Remove 'L:' for LittleFS.open
            }
            File img_file = LittleFS.open(fs_path.c_str(), "r");
            if (!img_file) {
                Serial.print("Image file not found: ");
                Serial.println(fs_path);
            } else {
                Serial.print("Image file found: ");
                Serial.println(fs_path);
                img_file.close();
            }
            Serial.print("LVGL: Creating image object at (");
            Serial.print(x);
            Serial.print(", ");
            Serial.print(y);
            Serial.print(") with src: ");
            Serial.println(src ? src : "(null)");

            lv_obj_t* img = lv_image_create(parent);
            Serial.println("LVGL: Calling lv_image_set_src...");
            lv_image_set_src(img, src ? src : "");
            Serial.println("LVGL: lv_image_set_src call finished. Check LVGL logs for errors if image does not display.");
            lv_obj_set_pos(img, x, y);
        }
    }
}
