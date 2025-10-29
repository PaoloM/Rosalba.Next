#include "main_ui.h"
#include <Arduino.h>

// Global UI instance
lv_ui guider_main_ui;

void setup_scr_main(lv_ui *ui) {
    // Create main screen (LVGL v9.x API)
    ui->main = lv_obj_create(NULL);
    lv_obj_set_size(ui->main, LV_HOR_RES, LV_VER_RES);
    lv_obj_set_style_bg_color(ui->main, lv_color_black(), LV_PART_MAIN | LV_STATE_DEFAULT);
    
    // Create a label
    lv_obj_t *label = lv_label_create(ui->main);
    lv_label_set_text(label, "Hello Arduino LVGL v9.4!");
    lv_obj_set_style_text_color(label, lv_color_white(), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_center(label);
    
    Serial.println("Main UI setup complete with LVGL v9.4");
}

void events_init_main(lv_ui *ui) {
    // Initialize events for main screen
    // Add event handlers here if needed
}