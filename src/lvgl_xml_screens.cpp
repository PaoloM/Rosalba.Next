#include <cctype>

#include "lvgl_xml_screens.h"
#include <lvgl.h>
#include <map>
#include <string>
#include <cstring>
#include <algorithm>
#include <tinyxml2.h>
#include <Arduino.h>
#include <LittleFS.h>
using namespace tinyxml2;

// Widget map for data binding
std::map<std::string, lv_obj_t *> widget_map;

// Font map for labels
struct FontMap { const char* name; const lv_font_t* ptr; };
static const FontMap font_map[] = {
    { "Montserrat_12", &lv_font_montserrat_12 },
    { "Montserrat_14", &lv_font_montserrat_14 },
    { "Montserrat_16", &lv_font_montserrat_16 },
    { "Montserrat_20", &lv_font_montserrat_20 },
    { "Montserrat_24", &lv_font_montserrat_24 },
    { "Montserrat_32", &lv_font_montserrat_32 },
    { "Montserrat_48", &lv_font_montserrat_48 }
};

// Helper: parse color string like #RRGGBB
static lv_color_t parse_color(const char *color)
{
    if (!color || strlen(color) != 7 || color[0] != '#')
        return lv_color_black();
    uint32_t col = (uint32_t)strtol(color + 1, NULL, 16);
    return lv_color_hex(col);
}

// Helper: parse int or percentage
static int parse_int_or_percent(const char *value, int parent_size)
{
    if (!value)
        return parent_size; // if this is null, return parent size
    size_t len = strlen(value);
    if (len > 1 && value[len - 1] == '%')
    {
        char buf[8];
        size_t n = (len < sizeof(buf)) ? len - 1 : sizeof(buf) - 1;
        strncpy(buf, value, n);
        buf[n] = '\0';
        int percent = atoi(buf);
        return (parent_size * percent) / 100;
    }
    return atoi(value);
}

// Helper: case-insensitive string compare
static bool equals_ignore_case(const char* a, const char* b) {
    if (!a || !b) return false;
    while (*a && *b) {
        if (tolower((unsigned char)*a) != tolower((unsigned char)*b)) return false;
        ++a; ++b;
    }
    return *a == *b;
}

// Common attributes

// static void apply_common_widget_attributes(lv_obj_t *obj, XMLElement *elem, lv_obj_t *parent, bool is_label = false)
// {
//     int parent_w = lv_obj_get_width(parent);
//     int parent_h = lv_obj_get_height(parent);
//     int w = 0, h = 0;
//     const char *w_attr = elem->Attribute("width");
//     const char *h_attr = elem->Attribute("height");
//     // Robust width calculation
//     if (w_attr) {
//         if (strcmp(w_attr, "100%") == 0) {
//             if (parent_w < 1) parent_w = lv_obj_get_width(lv_scr_act());
//             w = parent_w;
//         } else if (w_attr[strlen(w_attr)-1] == '%') {
//             w = parse_int_or_percent(w_attr, parent_w);
//         } else {
//             w = atoi(w_attr);
//         }
//     } else {
//         w = elem->IntAttribute("width", 0);
//     }
//     // Robust height calculation
//     if (h_attr) {
//         if (strcmp(h_attr, "100%") == 0) {
//             if (parent_h < 1) parent_h = lv_obj_get_height(lv_scr_act());
//             h = parent_h;
//         } else if (h_attr[strlen(h_attr)-1] == '%') {
//             h = parse_int_or_percent(h_attr, parent_h);
//         } else {
//             h = atoi(h_attr);
//         }
//     } else {
//         h = elem->IntAttribute("height", 0);
//     }
//     // Fallbacks for zero width/height
//     if (w < 1 && parent_w > 0) w = parent_w;
//     if (h < 1 && parent_h > 0) h = parent_h;
//     if (is_label)
//     {
//         const char *text = elem->Attribute("text");
//         if (w < 1 && text)
//             w = 20 + strlen(text) * 8;
//         if (w < 1)
//             w = 20;
//         if (h < 1)
//             h = 24;
//     }
//     int actual_w = lv_obj_get_width(obj);
//     int actual_h = lv_obj_get_height(obj);
//     // Position
//     int x = 0, y = 0;
//     const char *x_attr = elem->Attribute("x");
//     const char *y_attr = elem->Attribute("y");
//     const char *align = elem->Attribute("align");
//     const char *valign = elem->Attribute("valign");
//     if (x_attr)
//     {
//         if (strcmp(x_attr, "center") == 0)
//             x = std::max(0, (parent_w - actual_w) / 2);
//         else if (strcmp(x_attr, "right") == 0)
//             x = std::max(0, parent_w - actual_w);
//         else
//             x = parse_int_or_percent(x_attr, parent_w);
//     }
//     else if (align)
//     {
//         if (strcmp(align, "center") == 0)
//             x = std::max(0, (parent_w - actual_w) / 2);
//         else if (strcmp(align, "right") == 0)
//             x = std::max(0, parent_w - actual_w);
//     }
//     if (y_attr)
//     {
//         if (strcmp(y_attr, "center") == 0)
//             y = std::max(0, (parent_h - actual_h) / 2);
//         else if (strcmp(y_attr, "bottom") == 0)
//             y = std::max(0, parent_h - actual_h);
//         else
//             y = parse_int_or_percent(y_attr, parent_h);
//     }
//     else if (valign)
//     {
//         if (strcmp(valign, "center") == 0)
//             y = std::max(0, (parent_h - actual_h) / 2);
//         else if (strcmp(valign, "bottom") == 0)
//             y = std::max(0, parent_h - actual_h);
//     }
//     lv_obj_set_pos(obj, x, y);
//     // Only set width/height if > 0 and do not overwrite a nonzero value with zero
//     if (w > 0 && lv_obj_get_width(obj) != w)
//         lv_obj_set_width(obj, w);
//     if (h > 0 && lv_obj_get_height(obj) != h)
//         lv_obj_set_height(obj, h);
//     // Color attributes
//     const char *background_color = elem->Attribute("background-color");
//     if (background_color)
//         lv_obj_set_style_bg_color(obj, parse_color(background_color), LV_PART_MAIN | LV_STATE_DEFAULT);
//     const char *color = elem->Attribute("color");
//     if (color) {
//         if (is_label) {
//             lv_obj_set_style_text_color(obj, parse_color(color), LV_PART_MAIN | LV_STATE_DEFAULT);
//         } else {
//             // For lines, circles, etc: use as stroke color
//             lv_obj_set_style_border_color(obj, parse_color(color), LV_PART_MAIN | LV_STATE_DEFAULT);
//             lv_obj_set_style_line_color(obj, parse_color(color), LV_PART_MAIN | LV_STATE_DEFAULT);
//         }
//     }
//     // Border
//     int border = elem->IntAttribute("border", -1);
//     if (border >= 0)
//         lv_obj_set_style_border_width(obj, border, LV_PART_MAIN | LV_STATE_DEFAULT);
//     else
//         lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
//     const char *border_color = elem->Attribute("border-color");
//     if (border_color)
//         lv_obj_set_style_border_color(obj, parse_color(border_color), LV_PART_MAIN | LV_STATE_DEFAULT);
//     // Margin & Padding
//     int margin = elem->IntAttribute("margin", -1);
//     int padding = elem->IntAttribute("padding", -1);
//     if (margin >= 0)
//         lv_obj_set_style_margin_all(obj, margin, LV_PART_MAIN | LV_STATE_DEFAULT);
//     else
//         lv_obj_set_style_margin_all(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
//     if (padding >= 0)
//         lv_obj_set_style_pad_all(obj, padding, LV_PART_MAIN | LV_STATE_DEFAULT);
//     else
//         lv_obj_set_style_pad_all(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
// }

static void apply_common_widget_attributes(lv_obj_t *obj, XMLElement *elem, lv_obj_t *parent)
{
    // get parent dimensions and position
    int parent_w = lv_obj_get_width(parent);
    int parent_h = lv_obj_get_height(parent);
    int parent_x = lv_obj_get_x(parent);
    int parent_y = lv_obj_get_y(parent);

    int w = 0, h = 0;

    // get width and height attributes
    const char *w_attr = elem->Attribute("width");
    const char *h_attr = elem->Attribute("height");

    bool is_label = equals_ignore_case(elem->Name(), "label");

    w = w_attr ? parse_int_or_percent(w_attr, parent_w) : parent_w;
    h = h_attr ? parse_int_or_percent(h_attr, parent_h) : parent_h;
  
    if (is_label)
    {
        h = lv_obj_get_style_text_font(obj, LV_PART_MAIN)->line_height;
    }
    int actual_w = lv_obj_get_width(obj);
    int actual_h = lv_obj_get_height(obj);

    // Position
    int x = 0, y = 0;
    const char *x_attr = elem->Attribute("x");
    const char *y_attr = elem->Attribute("y");
    const char *align = elem->Attribute("align");
    const char *valign = elem->Attribute("valign");
    if (x_attr)
    {
        if (equals_ignore_case(x_attr, "center"))
            x = std::max(0, (parent_w - actual_w) / 2);
        else if (equals_ignore_case(x_attr, "right"))
            x = std::max(0, parent_w - actual_w);
        else
            x = parse_int_or_percent(x_attr, parent_w);
    }
    else if (align)
    {
        if (equals_ignore_case(align, "center"))
            x = std::max(0, (parent_w - actual_w) / 2);
        else if (equals_ignore_case(align, "right"))
            x = std::max(0, parent_w - actual_w);
    }
    if (y_attr)
    {
        if (equals_ignore_case(y_attr, "center"))
            y = std::max(0, (parent_h - actual_h) / 2);
        else if (equals_ignore_case(y_attr, "bottom"))
            y = std::max(0, parent_h - actual_h);
        else
            y = parse_int_or_percent(y_attr, parent_h);
    }
    else if (valign)
    {
        if (equals_ignore_case(valign, "center"))
            y = std::max(0, (parent_h - actual_h) / 2);
        else if (equals_ignore_case(valign, "bottom"))
            y = std::max(0, parent_h - actual_h);
    }
    lv_obj_set_pos(obj, x, y);

    // Only set width/height if > 0 and do not overwrite a nonzero value with zero
    if (w > 0 && lv_obj_get_width(obj) != w)
        lv_obj_set_width(obj, w);
    if (h > 0 && lv_obj_get_height(obj) != h)
        lv_obj_set_height(obj, h);
    
    
    
    Serial.print("[LVGL_XML] Applying attributes to "); Serial.print(elem->Name());
    Serial.print("  Parent WxH: "); Serial.print(parent_w); Serial.print("x"); Serial.print(parent_h);
    Serial.print("  Desired WxH: "); Serial.print(w); Serial.print("x"); Serial.print(parent_h);
    Serial.print("  Widget WxH: "); Serial.print(actual_w); Serial.print("x"); Serial.println(actual_h);


    
    
    
    // Background color
    const char *background_color = elem->Attribute("background-color");
    if (background_color)
        lv_obj_set_style_bg_color(obj, parse_color(background_color), LV_PART_MAIN | LV_STATE_DEFAULT);

    // Text or Stroke color
    const char *color = elem->Attribute("color");
    if (color) {
        if (is_label) {
            lv_obj_set_style_text_color(obj, parse_color(color), LV_PART_MAIN | LV_STATE_DEFAULT);
        } else {
            // For lines, circles, etc: use as stroke color
            lv_obj_set_style_border_color(obj, parse_color(color), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_line_color(obj, parse_color(color), LV_PART_MAIN | LV_STATE_DEFAULT);
        }
    }

    // Border thickness and color
    int border = elem->IntAttribute("border", -1);
    if (border > 0)
        lv_obj_set_style_border_width(obj, border, LV_PART_MAIN | LV_STATE_DEFAULT);
    else
        lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    
    const char *border_color = elem->Attribute("border-color");
    if (border_color)
        lv_obj_set_style_border_color(obj, parse_color(border_color), LV_PART_MAIN | LV_STATE_DEFAULT);
    
    // Margin & Padding (in integer pixels only)
    int margin = elem->IntAttribute("margin", -1);
    int padding = elem->IntAttribute("padding", -1);
    if (margin > 0)
        lv_obj_set_style_margin_all(obj, margin, LV_PART_MAIN | LV_STATE_DEFAULT);
    else
        lv_obj_set_style_margin_all(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    if (padding > 0)
        lv_obj_set_style_pad_all(obj, padding, LV_PART_MAIN | LV_STATE_DEFAULT);
    else
        lv_obj_set_style_pad_all(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
}

// ========== Widget creation functions

static void create_label(XMLElement *elem, lv_obj_t *parent, const char *id)
{
    const char *text = elem->Attribute("text");
    
    lv_obj_t *label = lv_label_create(parent);
    lv_label_set_text(label, text ? text : "");

    const char* font = elem->Attribute("font");
    if (font) {
        for (const auto& entry : font_map) {
            if (equals_ignore_case(font, entry.name)) {
                lv_obj_set_style_text_font(label, entry.ptr, LV_PART_MAIN | LV_STATE_DEFAULT);
                break;
            }
        }
    }
 
    apply_common_widget_attributes(label, elem, parent);
  
    if (id) widget_map[id] = label;
}

static void create_line(XMLElement *elem, lv_obj_t *parent, const char *id)
{
    int parent_w = lv_obj_get_width(parent);
    int parent_h = lv_obj_get_height(parent);
    int x1 = parse_int_or_percent(elem->Attribute("x1"), parent_w);
    int y1 = parse_int_or_percent(elem->Attribute("y1"), parent_h);
    int x2 = parse_int_or_percent(elem->Attribute("x2"), parent_w);
    int y2 = parse_int_or_percent(elem->Attribute("y2"), parent_h);

    lv_obj_t *line = lv_line_create(parent);
    lv_point_precise_t points[2] = {{x1, y1}, {x2, y2}};
    lv_line_set_points(line, points, 2);
    
    int stroke = elem->IntAttribute("stroke", 2);
    lv_obj_set_style_line_width(line, stroke, LV_PART_MAIN | LV_STATE_DEFAULT);
    
    apply_common_widget_attributes(line, elem, parent);
    
    if (id) widget_map[id] = line;
}

static void create_image(XMLElement *elem, lv_obj_t *parent, const char *id)
{
    const char *src = elem->Attribute("src");

    lv_obj_t *img = lv_image_create(parent);
    lv_image_set_src(img, src ? src : "");
    
    apply_common_widget_attributes(img, elem, parent);
    
    if (id) widget_map[id] = img;
}

static void create_circle(XMLElement *elem, lv_obj_t *parent, const char *id)
{
    int parent_w = lv_obj_get_width(parent);
    int parent_h = lv_obj_get_height(parent);
    int radius = parse_int_or_percent(elem->Attribute("radius"), std::min(parent_w, parent_h));

    lv_obj_t *circle = lv_obj_create(parent);
    
    if (radius > 0)
        lv_obj_set_style_radius(circle, radius, LV_PART_MAIN | LV_STATE_DEFAULT);
    
    const char *fill_color = elem->Attribute("fill-color");
    if (fill_color)
        lv_obj_set_style_bg_color(circle, parse_color(fill_color), LV_PART_MAIN | LV_STATE_DEFAULT);
    
    int stroke = elem->IntAttribute("stroke", 0);
    if (stroke)
        lv_obj_set_style_border_width(circle, stroke, LV_PART_MAIN | LV_STATE_DEFAULT);

    apply_common_widget_attributes(circle, elem, parent);

    if (id) widget_map[id] = circle;
}

// ========== Containers creation functions

static void create_stackpanel(XMLElement *elem, lv_obj_t *parent, const char *id)
{
    // Moving cursor for auto layout in stackpanels
    int cursor_x = 0;
    int cursor_y = 0;

    const char *orientation = elem->Attribute("orientation");

    lv_obj_t *panel = lv_obj_create(parent);

    apply_common_widget_attributes(panel, elem, parent);
    
    lv_obj_set_style_radius(panel, 0, LV_PART_MAIN | LV_STATE_DEFAULT); // no round corners
    
    // Create children 
    for (XMLElement *child = elem->FirstChildElement(); child; child = child->NextSiblingElement()) {
        // apply layout positioning here based on orientation
        create_widget_from_xml(child, panel);
    }

    if (id) widget_map[id] = panel;
}

static void create_panel(XMLElement *elem, lv_obj_t *parent, const char *id)
{
    lv_obj_t *panel = lv_obj_create(parent);

    lv_obj_set_style_radius(panel, 0, LV_PART_MAIN | LV_STATE_DEFAULT); // no round corners
    
    apply_common_widget_attributes(panel, elem, parent);
    
    // Create children 
    for (XMLElement *child = elem->FirstChildElement(); child; child = child->NextSiblingElement()) {
        create_widget_from_xml(child, panel);
    }
    
    if (id) widget_map[id] = panel;
}

void create_widget_from_xml(XMLElement *elem, lv_obj_t *parent)
{
    const char *type = elem->Name();
    const char *id = elem->Attribute("id");
    // if (strcmp(type, "container") == 0)
    // {
    //     lv_obj_t *cont = lv_obj_create(parent);
    //     // Set width/height before creating children
    //     int parent_w = lv_obj_get_width(parent);
    //     int parent_h = lv_obj_get_height(parent);
    //     const char *w_attr = elem->Attribute("width");
    //     const char *h_attr = elem->Attribute("height");
    //     if (w_attr && strcmp(w_attr, "100%") == 0) {
    //         if (parent_w < 1) parent_w = lv_obj_get_width(lv_scr_act());
    //         lv_obj_set_width(cont, parent_w);
    //     }
    //     if (h_attr && strcmp(h_attr, "100%") == 0) {
    //         if (parent_h < 1) parent_h = lv_obj_get_height(lv_scr_act());
    //         lv_obj_set_height(cont, parent_h);
    //     }
    //     // Ensure parent has valid size before creating children
    //     if (lv_obj_get_width(cont) < 1) lv_obj_set_width(cont, lv_obj_get_width(lv_scr_act()));
    //     if (lv_obj_get_height(cont) < 1) lv_obj_set_height(cont, lv_obj_get_height(lv_scr_act()));
    //     apply_common_widget_attributes(cont, elem, parent);
    //     lv_obj_set_style_radius(cont, 0, LV_PART_MAIN | LV_STATE_DEFAULT); // No rounded corners
    //     if (id)
    //         widget_map[id] = cont;
    //     // Create children
    //     for (XMLElement *child = elem->FirstChildElement(); child; child = child->NextSiblingElement())
    //     {
    //         create_widget_from_xml(child, cont);
    //     }
    //     // Re-apply attributes to children after parent is sized
    //     for (XMLElement *child = elem->FirstChildElement(); child; child = child->NextSiblingElement()) {
    //         const char *child_id = child->Attribute("id");
    //         lv_obj_t *child_obj = nullptr;
    //         if (child_id && widget_map.count(child_id)) child_obj = widget_map[child_id];
    //         else child_obj = lv_obj_get_child(cont, lv_obj_get_child_cnt(cont) - 1);
    //         if (child_obj) apply_common_widget_attributes(child_obj, child, cont);
    //     }
    //     // ...existing code...
    // }
    // else 
    if (equals_ignore_case(type, "stackpanel"))
    {
        create_stackpanel(elem, parent, id);
    }
    else if (equals_ignore_case(type, "label"))
    {
        create_label(elem, parent, id);
    }
    else if (equals_ignore_case(type, "line"))
    {
        create_line(elem, parent, id);
    }
    else if (equals_ignore_case(type, "image"))
    {
        create_image(elem, parent, id);
    }
    else if (equals_ignore_case(type, "circle"))
    {
        create_circle(elem, parent, id);
    }
}

void load_screen_from_xml(const char *xml_path, lv_obj_t *parent)
{
    if (!parent) parent = lv_scr_act();
    lv_obj_set_style_bg_color(parent, lv_color_black(), LV_PART_MAIN | LV_STATE_DEFAULT); // set screen to black background
    widget_map.clear(); 

    File file = LittleFS.open(xml_path, "r");
    if (!file) {
        Serial.println("[LVGL_XML] ERROR: XML file not found!");
        return;
    }
    
    String xmlContent;
    while (file.available())
        xmlContent += (char)file.read();
    file.close();
    
    XMLDocument doc;
    if (doc.Parse(xmlContent.c_str()) != XML_SUCCESS) {
        Serial.println("[LVGL_XML] ERROR: XML parse failed!");
        return;
    }
    
    XMLElement *screen = doc.FirstChildElement("screen");
    if (!screen) {
        Serial.println("[LVGL_XML] ERROR: <screen> element not found!");
        return;
    }

    // cursor_x = 0; // reset the moving cursor to the top left of the screen
    // cursor_y = 0;

    for (XMLElement *elem = screen->FirstChildElement(); elem; elem = elem->NextSiblingElement()) {
        create_widget_from_xml(elem, parent);
    }
}
