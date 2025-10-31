#include "main_ui.h"
#include <Arduino.h>
#include <SD.h>

// Global UI instance
lv_ui guider_main_ui;

void setup_scr_main(lv_ui *ui) {
    // Create main screen (LVGL v9.x API)
    ui->main = lv_obj_create(NULL);
    lv_obj_set_size(ui->main, LV_HOR_RES, LV_VER_RES);
    lv_obj_set_style_bg_color(ui->main, lv_color_black(), LV_PART_MAIN | LV_STATE_DEFAULT);
    
    // Create title label
    lv_obj_t *title_label = lv_label_create(ui->main);
    lv_label_set_text(title_label, "ROSALBA.NEXT");
    lv_obj_set_style_text_color(title_label, lv_color_white(), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(title_label, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_align(title_label, LV_ALIGN_TOP_MID, 0, 20);
       
    // Create bottom status label
    lv_obj_t *status_label = lv_label_create(ui->main);
    lv_label_set_text(status_label, "LVGL v9.4");
    lv_obj_set_style_text_color(status_label, lv_color_make(0x80, 0x80, 0x80), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_align(status_label, LV_ALIGN_BOTTOM_MID, 0, -20);
    
    Serial.println("Main UI setup complete");
}

void events_init_main(lv_ui *ui) {
    // Initialize events for main screen
    // Add event handlers here if needed
}