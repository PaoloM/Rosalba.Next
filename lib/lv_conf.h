#ifndef LV_CONF_H
#define LV_CONF_H

#include <stdint.h>

/* FreeRTOS includes for tick functions */
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

/*====================
   LVGL CONFIGURATION
 *====================*/

/* Enable LVGL */
#define LV_USE_PERF_MONITOR       1
#define LV_USE_LOG                1
#define LV_LOG_LEVEL              LV_LOG_LEVEL_WARN

/*====================
   HAL SETTINGS
 *====================*/

/* Color depth: ST7796 supports 16-bit color */
#define LV_COLOR_DEPTH            16
#define LV_COLOR_16_SWAP          0

/* Display resolution */
#define LV_HOR_RES_MAX            320
#define LV_VER_RES_MAX            480

/*====================
   MEMORY SETTINGS
 *====================*/

#define LV_MEM_SIZE               (32U * 1024U)   /* 32KB for LVGL heap */
#define LV_MEM_BUF_MAX_NUM       16

/*====================
   FEATURE SETTINGS
 *====================*/

/* Enable drawing with GPU (optional for ESP32) */
#define LV_USE_GPU                0

/* Enable fonts */
#define LV_FONT_MONTSERRAT_12     1
#define LV_FONT_MONTSERRAT_16     1
#define LV_FONT_MONTSERRAT_20     1

/* Enable built-in themes */
#define LV_USE_THEME_DEFAULT      1

/*====================
   MISC SETTINGS
 *====================*/

#define LV_TICK_CUSTOM            1
#define LV_TICK_CUSTOM_SYS_TIME_EXPR (xTaskGetTickCount() * portTICK_PERIOD_MS)

#endif /*LV_CONF_H*/
