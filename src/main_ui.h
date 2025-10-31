#pragma once
#include <lvgl.h>

typedef struct {
    lv_obj_t *main;
} lv_ui;

void setup_scr_main(lv_ui *ui);
void events_init_main(lv_ui *ui);
