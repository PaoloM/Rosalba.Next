#pragma once

#include <lvgl.h>

// UI structure
typedef struct {
    lv_obj_t *main;
    // Add other UI objects here as needed
} lv_ui;

// Global UI instance
extern lv_ui guider_main_ui;

// UI functions
void setup_scr_main(lv_ui *ui);
void events_init_main(lv_ui *ui);

// Image loading functions
bool load_png_from_sd(const char* path, lv_obj_t* parent);