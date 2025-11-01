#include "lvgl_xml_loader.h"
#include "lvgl.h"
#include <LittleFS.h>
#include <Arduino.h>
#include <map>
#include <string>

#include <tinyxml2.h>
using namespace tinyxml2;

// Global widget map for data binding
std::map<std::string, lv_obj_t*> widget_map;

void create_image(tinyxml2::XMLElement *elem, lv_obj_t *parent, const char *id)
{
    int x = elem->IntAttribute("x", 0);
    int y = elem->IntAttribute("y", 0);
    const char *src = elem->Attribute("src");
    // Check if file exists in LittleFS
    String fs_path = String(src);
    if (fs_path.startsWith("L:/"))
    {
        fs_path = fs_path.substring(2); // Remove 'L:' for LittleFS.open
    }
    File img_file = LittleFS.open(fs_path.c_str(), "r");
    if (!img_file)
    {
        Serial.print("Image file not found: ");
        Serial.println(fs_path);
    }
    else
    {
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

    lv_obj_t *img = lv_image_create(parent);
    Serial.println("LVGL: Calling lv_image_set_src...");
    lv_image_set_src(img, src ? src : "");
    Serial.println("LVGL: lv_image_set_src call finished. Check LVGL logs for errors if image does not display.");
    lv_obj_set_pos(img, x, y);
    if (id)
        widget_map[id] = img;
}

void create_line(tinyxml2::XMLElement *elem, lv_obj_t *parent, const char *id)
{
    int x1 = elem->IntAttribute("x1", 0);
    int y1 = elem->IntAttribute("y1", 0);
    int x2 = elem->IntAttribute("x2", 0);
    int y2 = elem->IntAttribute("y2", 0);
    const char *color = elem->Attribute("color");
    int width = elem->IntAttribute("width", 2);
    int min_x = std::min(x1, x2);
    int min_y = std::min(y1, y2);
    int max_x = std::max(x1, x2);
    int max_y = std::max(y1, y2);
    lv_obj_t *line = lv_line_create(parent);
    lv_point_precise_t points[2] = {{x1 - min_x, y1 - min_y}, {x2 - min_x, y2 - min_y}};
    lv_line_set_points(line, points, 2);
    int w = max_x - min_x;
    int h = max_y - min_y;
    if (w < 1)
        w = 1;
    if (h < 1)
        h = 1;
    lv_obj_set_size(line, w, h);
    lv_obj_set_pos(line, min_x, min_y);
    if (color)
    {
        uint32_t col = (uint32_t)strtol(color + 1, NULL, 16); // skip '#'
        lv_obj_set_style_line_color(line, lv_color_hex(col), LV_PART_MAIN | LV_STATE_DEFAULT);
    }
    lv_obj_set_style_line_width(line, width, LV_PART_MAIN | LV_STATE_DEFAULT);
    if (id)
        widget_map[id] = line;
}

void create_label(tinyxml2::XMLElement *elem, lv_obj_t *parent, const char *id)
{
    const char *lv_obj_center = elem->Attribute("x");
    int x = elem->IntAttribute("x", 0);
    int y = elem->IntAttribute("y", 0);
    int w = elem->IntAttribute("width", 0);
    int h = elem->IntAttribute("height", 0);
    const char *text = elem->Attribute("text");
    const char *color = elem->Attribute("color");
    const char *font = elem->Attribute("font");
    lv_obj_t *label = lv_label_create(parent);
    lv_label_set_text(label, text ? text : "");
    lv_obj_set_pos(label, x, y);
    if (w)
        lv_obj_set_width(label, w);
    if (h)
        lv_obj_set_height(label, h);
    if (strcmp(lv_obj_center, "center") == 0)
    {
        lv_obj_set_style_text_align(label, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    }
    if (color)
    {
        uint32_t col = (uint32_t)strtol(color + 1, NULL, 16); // skip '#'
        lv_obj_set_style_text_color(label, lv_color_hex(col), LV_PART_MAIN | LV_STATE_DEFAULT);
    }
    if (font)
    {
        if (strcmp(font, "Montserrat_24") == 0)
        {
            lv_obj_set_style_text_font(label, &lv_font_montserrat_24, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        if (strcmp(font, "Montserrat_32") == 0)
        {
            lv_obj_set_style_text_font(label, &lv_font_montserrat_32, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        if (strcmp(font, "Montserrat_48") == 0)
        {
            lv_obj_set_style_text_font(label, &lv_font_montserrat_48, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
    }
    if (id)
        widget_map[id] = label;
}

void load_screen_from_xml(const char* xml_path, lv_obj_t* parent) {
    // Set background color to black
    lv_obj_set_style_bg_color(parent, lv_color_black(), LV_PART_MAIN | LV_STATE_DEFAULT);

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
        const char* id = elem->Attribute("id");

        if (strcmp(type, "label") == 0) {
            create_label(elem, parent, id);

        } else if (strcmp(type, "line") == 0) {
            create_line(elem, parent, id);
            
        } else if (strcmp(type, "image") == 0) {
            create_image(elem, parent, id);
        }
    }
}

// Example: update label text by id
void update_label_text(const std::string& id, const char* new_text) {
    auto it = widget_map.find(id);
    if (it != widget_map.end()) {
        lv_label_set_text(it->second, new_text);
    }
}
