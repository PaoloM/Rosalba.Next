#include <cctype>
#include <tinyxml2.h>

#include "lvgl_xml_screens.h"
#include <lvgl.h>
#include <map>
#include <string>
#include <cstring>
#include <algorithm>
#include <Arduino.h>
#include <LittleFS.h>


// Global XML document and root element for layout
tinyxml2::XMLDocument* g_screen_xml_doc = nullptr;
tinyxml2::XMLElement* g_screen_xml_root = nullptr;


bool execute_layout = false;

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

// static void apply_common_widget_attributes(lv_obj_t *obj, tinyxml2::XMLElement *elem, lv_obj_t *parent, bool is_label = false)
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

static void set_widget_geometry(lv_obj_t *obj, int x, int y, int w, int h)
{
    lv_obj_set_pos(obj, x, y);
    if (w > 0)
        lv_obj_set_width(obj, w);
    if (h > 0)
        lv_obj_set_height(obj, h);
}

void recalculate_layout_tree(lv_obj_t *obj, tinyxml2::XMLElement *elem, lv_obj_t *parent)
{
    Serial.print("recalculate_layout_tree: type="); Serial.println(elem->Name());

    int x = 0, y = 0, w = 0, h = 0;

    const char *type = elem->Name();
    const char *id = elem->Attribute("id");

    int parent_w = lv_obj_get_width(parent);
    int parent_h = lv_obj_get_height(parent);
    int parent_x = lv_obj_get_x(parent);
    int parent_y = lv_obj_get_y(parent);

    const char *x_attr = elem->Attribute("x");
    const char *y_attr = elem->Attribute("y");
    const char *w_attr = elem->Attribute("width");
    const char *h_attr = elem->Attribute("height");

    // Robust x calculation
    if (x_attr) {
        x = parse_int_or_percent(x_attr, parent_w);
    }
    // Robust y calculation
    if (y_attr) {
        y = parse_int_or_percent(y_attr, parent_h);
    }

    // Robust width calculation
    if (w_attr) {
        w = parse_int_or_percent(w_attr, parent_w);
    } else { // if no width specified, assume 100% of the parent
        w = parent_w;
    }
    // Robust height calculation
    if (h_attr) {
        h = parse_int_or_percent(h_attr, parent_h);
    } else {
        h = parent_h; // if no height specified, assume 100% of the parent
    }

    if (equals_ignore_case(type, "stackpanel")) {
        set_widget_geometry(obj, x + parent_x, y + parent_y, w, h); // set panel geometry
        const char *orientation = elem->Attribute("orientation");
            Serial.print("Stackpanel orientation: "); Serial.println(orientation ? orientation : "(null)");
        if (orientation && equals_ignore_case(orientation, "vertical")) {
            Serial.println("Vertical stackpanel layout triggered");
            int y_offset = 0;
            int child_idx = 0;
            for (tinyxml2::XMLElement *child = elem->FirstChildElement(); child; child = child->NextSiblingElement(), ++child_idx) {
                Serial.print("traversing child elements: type="); Serial.println(child->Name());
                lv_obj_t *child_obj = lv_obj_get_child(obj, child_idx);
                int child_h = 0;
                const char *child_h_attr = child->Attribute("height");
                if (child_h_attr)
                    child_h = parse_int_or_percent(child_h_attr, h);
                else
                    child_h = h; // fallback to panel height
                set_widget_geometry(child_obj, x + parent_x, y + parent_y + y_offset, w, child_h);
                recalculate_layout_tree(child_obj, child, obj); // Recurse into child
                Serial.print("y_offset before increment: "); Serial.println(y_offset);
                y_offset += child_h;
            }
        }
        if (orientation && equals_ignore_case(orientation, "horizontal")) {
            Serial.println("Horizontal stackpanel layout triggered");
            int x_offset = 0; // Only initialize once per container
            int child_idx = 0;
            for (tinyxml2::XMLElement *child = elem->FirstChildElement(); child; child = child->NextSiblingElement(), ++child_idx) {
                Serial.print("traversing child elements: type="); Serial.println(child->Name());
                lv_obj_t *child_obj = lv_obj_get_child(obj, child_idx);
                int child_w = 0;
                const char *child_w_attr = child->Attribute("width");
                if (child_w_attr) {
                    child_w = parse_int_or_percent(child_w_attr, w);
                } else if (equals_ignore_case(child->Name(), "label")) {
                    // Calculate label width from text and font
                    const char *text = child->Attribute("text");
                    if (!text) text = "";
                    const lv_font_t *font = lv_obj_get_style_text_font(child_obj, LV_PART_MAIN);
                    if (!font) font = LV_FONT_DEFAULT;
                    lv_point_t size;
                    lv_txt_get_size(&size, text, font, 0, 0, LV_COORD_MAX, LV_TEXT_FLAG_NONE);
                    child_w = size.x + 8; // add some padding
                    Serial.println(String("Calculated label width: ") + child_w + " for text: " + text);
                } else {
                    child_w = w; // fallback to panel width
                }
                Serial.print("Setting geometry for child "); Serial.print(child_idx); Serial.print(" x="); Serial.print(x + parent_x + x_offset); Serial.print(" w="); Serial.println(child_w);
                set_widget_geometry(child_obj, x + parent_x + x_offset, y + parent_y, child_w, h);
                recalculate_layout_tree(child_obj, child, obj); // Recurse into child
                Serial.print("x_offset before increment: "); Serial.println(x_offset);
                x_offset += child_w; // Accumulate offset for next child
                Serial.print("x_offset after increment: "); Serial.println(x_offset);
            }
        }
    } else if (equals_ignore_case(type, "panel")) {
        set_widget_geometry(obj, x + parent_x, y + parent_y, w, h);
    } else {
        set_widget_geometry(obj, x + parent_x, y + parent_y, w, h);
    }

}
    
    
    
static void apply_common_widget_attributes(lv_obj_t *obj, tinyxml2::XMLElement *elem, lv_obj_t *parent)
{
    bool is_label = equals_ignore_case(elem->Name(), "label");

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

static void create_label(tinyxml2::XMLElement *elem, lv_obj_t *parent, const char *id)
{
    lv_obj_t *label = lv_label_create(parent); // create label object
    lv_obj_set_layout(label, LV_LAYOUT_NONE);
    lv_obj_clear_flag(label, LV_OBJ_FLAG_SCROLLABLE);

    const char *text = elem->Attribute("text"); // get text attribute
    lv_label_set_text(label, text ? text : "");

    const char* font = elem->Attribute("font"); // get font attribute
    if (font) {
        for (const auto& entry : font_map) {
            if (equals_ignore_case(font, entry.name)) {
                lv_obj_set_style_text_font(label, entry.ptr, LV_PART_MAIN | LV_STATE_DEFAULT);
                break;
            }
        }
    }
 
    apply_common_widget_attributes(label, elem, parent); // apply common attributes

    if (id) widget_map[id] = label; // store in widget map if id is present
}

static void create_line(tinyxml2::XMLElement *elem, lv_obj_t *parent, const char *id)
{
    // TODO: need to figure out the geometry here
    int parent_w = lv_obj_get_width(parent);
    int parent_h = lv_obj_get_height(parent);
    int x1 = parse_int_or_percent(elem->Attribute("x1"), parent_w);
    int y1 = parse_int_or_percent(elem->Attribute("y1"), parent_h);
    int x2 = parse_int_or_percent(elem->Attribute("x2"), parent_w);
    int y2 = parse_int_or_percent(elem->Attribute("y2"), parent_h);

    lv_obj_t *line = lv_line_create(parent);
    lv_obj_set_layout(line, LV_LAYOUT_NONE);
    lv_obj_clear_flag(line, LV_OBJ_FLAG_SCROLLABLE);
    lv_point_precise_t points[2] = {{x1, y1}, {x2, y2}};
    lv_line_set_points(line, points, 2);
    
    int stroke = elem->IntAttribute("stroke", 2);
    lv_obj_set_style_line_width(line, stroke, LV_PART_MAIN | LV_STATE_DEFAULT);
    
    apply_common_widget_attributes(line, elem, parent);
    
    if (id) widget_map[id] = line;
}

static void create_image(tinyxml2::XMLElement *elem, lv_obj_t *parent, const char *id)
{
    const char *src = elem->Attribute("src");

    lv_obj_t *img = lv_image_create(parent);
    lv_obj_set_layout(img, LV_LAYOUT_NONE);
    lv_obj_clear_flag(img, LV_OBJ_FLAG_SCROLLABLE);
    lv_image_set_src(img, src ? src : "");
    
    apply_common_widget_attributes(img, elem, parent);
    
    if (id) widget_map[id] = img;
}

static void create_circle(tinyxml2::XMLElement *elem, lv_obj_t *parent, const char *id)
{
    lv_obj_t *circle = lv_obj_create(parent);
    lv_obj_set_layout(circle, LV_LAYOUT_NONE);
    lv_obj_clear_flag(circle, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_radius(circle, 0, LV_PART_MAIN | LV_STATE_DEFAULT); // no round corners

    int radius = elem->IntAttribute("radius"); // get radius attribute (pixels only)
    if (radius > 0)
        lv_obj_set_style_radius(circle, radius, LV_PART_MAIN | LV_STATE_DEFAULT);
    
    const char *fill_color = elem->Attribute("fill-color"); // get fill color attribute
    if (fill_color)
        lv_obj_set_style_bg_color(circle, parse_color(fill_color), LV_PART_MAIN | LV_STATE_DEFAULT);
    
    int stroke = elem->IntAttribute("stroke", 0); // get stroke width
    if (stroke)
        lv_obj_set_style_border_width(circle, stroke, LV_PART_MAIN | LV_STATE_DEFAULT);

    apply_common_widget_attributes(circle, elem, parent); // apply common attributes

    if (id) widget_map[id] = circle; // store in widget map if id is present
}

// ========== Containers creation functions

static void create_stackpanel(tinyxml2::XMLElement *elem, lv_obj_t *parent, const char *id)
{
    lv_obj_t *panel = lv_obj_create(parent); // create panel object

    // Ensure manual positioning works
    lv_obj_set_layout(panel, LV_LAYOUT_NONE);
    lv_obj_clear_flag(panel, LV_OBJ_FLAG_SCROLLABLE);

    const char *orientation = elem->Attribute("orientation"); // get orientation attribute
    lv_obj_set_style_radius(panel, 0, LV_PART_MAIN | LV_STATE_DEFAULT); // no round corners
    // Create children 
    for (tinyxml2::XMLElement *child = elem->FirstChildElement(); child; child = child->NextSiblingElement()) {
        create_widget_from_xml(child, panel);
    }
    apply_common_widget_attributes(panel, elem, parent); // apply common attributes
    if (id) widget_map[id] = panel; // store in widget map if id is present
}

static void create_panel(tinyxml2::XMLElement *elem, lv_obj_t *parent, const char *id)
{
    lv_obj_t *panel = lv_obj_create(parent); // create panel object

    // Ensure manual positioning works
    lv_obj_set_layout(panel, LV_LAYOUT_NONE);
    lv_obj_clear_flag(panel, LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_set_style_radius(panel, 0, LV_PART_MAIN | LV_STATE_DEFAULT); // no round corners
    // Create children 
    for (tinyxml2::XMLElement *child = elem->FirstChildElement(); child; child = child->NextSiblingElement()) {
        create_widget_from_xml(child, panel);
    }
    apply_common_widget_attributes(panel, elem, parent); // apply common attributes
    if (id) widget_map[id] = panel; // store in widget map if id is present
}

void create_widget_from_xml(tinyxml2::XMLElement *elem, lv_obj_t *parent)
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
    //     for (tinyxml2::XMLElement *child = elem->FirstChildElement(); child; child = child->NextSiblingElement())
    //     {
    //         create_widget_from_xml(child, cont);
    //     }
    //     // Re-apply attributes to children after parent is sized
    //     for (tinyxml2::XMLElement *child = elem->FirstChildElement(); child; child = child->NextSiblingElement()) {
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
    execute_layout = false; // reset layout flag

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

    // Free previous doc if any
    if (g_screen_xml_doc) {
        delete g_screen_xml_doc;
        g_screen_xml_doc = nullptr;
        g_screen_xml_root = nullptr;
    }
    g_screen_xml_doc = new tinyxml2::XMLDocument();
    if (g_screen_xml_doc->Parse(xmlContent.c_str()) != tinyxml2::XML_SUCCESS) {
        Serial.println("[LVGL_XML] ERROR: XML parse failed!");
        delete g_screen_xml_doc;
        g_screen_xml_doc = nullptr;
        g_screen_xml_root = nullptr;
        return;
    }
    g_screen_xml_root = g_screen_xml_doc->FirstChildElement("screen");
    if (!g_screen_xml_root) {
        Serial.println("[LVGL_XML] ERROR: <screen> element not found!");
        delete g_screen_xml_doc;
        g_screen_xml_doc = nullptr;
        g_screen_xml_root = nullptr;
        return;
    }
    // There might be multiple top-level widgets, so traverse all
    for (tinyxml2::XMLElement *elem = g_screen_xml_root->FirstChildElement(); elem; elem = elem->NextSiblingElement()) {
        create_widget_from_xml(elem, parent);
    }
    execute_layout = true; // enable layout execution in loop()
}
