#include "main_ui.h"
#include <Arduino.h>
#include <SD.h>


void setup_scr_main(lv_ui *ui) {
    // Create main screen (LVGL v9.x API)
    ui->main = lv_obj_create(NULL);
    lv_obj_set_size(ui->main, LV_HOR_RES, LV_VER_RES);
    lv_obj_set_style_bg_color(ui->main, lv_color_black(), LV_PART_MAIN | LV_STATE_DEFAULT);

    // Load screen definition from XML
    extern void load_screen_from_xml(const char* xml_path, lv_obj_t* parent);
    load_screen_from_xml("/screen.xml", ui->main);

    Serial.println("Main UI loaded from XML");
}

void events_init_main(lv_ui *ui) {
    // Initialize events for main screen
    // Add event handlers here if needed
}