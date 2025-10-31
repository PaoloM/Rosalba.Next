#ifndef LV_CONF_H
#define LV_CONF_H

#include <stdint.h>

/* FreeRTOS includes for tick functions */
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

/*====================
   COLOR SETTINGS
 *====================*/

/** Color depth: 1 (I1), 8 (L8), 16 (RGB565), 24 (RGB888), 32 (XRGB8888) */
#define LV_COLOR_DEPTH 16

/*=========================
   STDLIB WRAPPER SETTINGS
 *=========================*/

/** Use LVGL's built-in implementation for memory management */
#define LV_USE_STDLIB_MALLOC    LV_STDLIB_BUILTIN
#define LV_USE_STDLIB_STRING    LV_STDLIB_BUILTIN
#define LV_USE_STDLIB_SPRINTF   LV_STDLIB_BUILTIN

/*====================
   MEMORY SETTINGS
 *====================*/

/** Size of the memory available for `lv_malloc()` in bytes (>= 2kB) */
#define LV_MEM_SIZE (32U * 1024U)   /* 32KB for LVGL heap */

/** Set an address for the memory pool instead of allocating it as a normal array. */
#define LV_MEM_ADR 0     /*0: unused*/

/** Automatically defrag on free */
#define LV_MEM_AUTO_DEFRAG 1

/*====================
   HAL SETTINGS
 *====================*/

/** Default display refresh period. LVG will redraw changed areas with this period time */
#define LV_DEF_REFR_PERIOD 30   /*[ms]*/

/** Default DPI. Used to initialize default sizes such as widgets sized, style paddings.
 * (Not so important, you can adjust it to modify default sizes and spaces) */
#define LV_DPI_DEF 130     /*[px/inch]*/

/*====================
   LVGL FILE SYSTEM SETTINGS
 *====================*/

#define LV_USE_FS_LITTLEFS 0
#define LV_USE_FS_ARDUINO_ESP_LITTLEFS 1
#define LV_FS_ARDUINO_ESP_LITTLEFS_LETTER 'L'
#define LV_FS_ARDUINO_ESP_LITTLEFS_PATH ""
/*====================
   LVGL FILE SYSTEM SETTINGS
 *====================*/

#define LV_USE_FS_ARDUINO_SD 1
#define LV_FS_ARDUINO_SD_LETTER 'S'
#define LV_FS_ARDUINO_SD_PATH ""
/*====================
   FEATURE CONFIGURATION
 *====================*/

/** 1: Enable the log module */
#define LV_USE_LOG 1
#if LV_USE_LOG

/** How important log should be added:
 * LV_LOG_LEVEL_TRACE       A lot of logs to give detailed information
 * LV_LOG_LEVEL_INFO        Log important events
 * LV_LOG_LEVEL_WARN        Log if something unwanted happened but didn't cause a problem
 * LV_LOG_LEVEL_ERROR       Only critical issue, when the system may fail
 * LV_LOG_LEVEL_USER        Only logs added by the user
 * LV_LOG_LEVEL_NONE        Do not log anything
 */
#define LV_LOG_LEVEL LV_LOG_LEVEL_WARN

/** 1: Print the log with 'printf'; 0: User need to register a callback with `lv_log_register_print_cb()` */
#define LV_LOG_PRINTF 0

/** Enable/disable LV_LOG_TRACE in modules that produces a huge number of logs */
#define LV_LOG_TRACE_MEM        1
#define LV_LOG_TRACE_TIMER      1
#define LV_LOG_TRACE_INDEV      1
#define LV_LOG_TRACE_DISP_REFR  1
#define LV_LOG_TRACE_EVENT      1
#define LV_LOG_TRACE_OBJ_CREATE 1
#define LV_LOG_TRACE_LAYOUT     1
#define LV_LOG_TRACE_ANIM       1
#define LV_LOG_TRACE_CACHE      1

#endif  /*LV_USE_LOG*/

/** 1: Enable performance monitor */
#define LV_USE_PERF_MONITOR 0
#if LV_USE_PERF_MONITOR
    #define LV_USE_PERF_MONITOR_POS LV_ALIGN_BOTTOM_RIGHT
#endif

/*====================
   FONT USAGE
 *====================*/

/** Montserrat fonts with ASCII range and some symbols */
#define LV_FONT_MONTSERRAT_8  0
#define LV_FONT_MONTSERRAT_10 0
#define LV_FONT_MONTSERRAT_12 1
#define LV_FONT_MONTSERRAT_14 1
#define LV_FONT_MONTSERRAT_16 1
#define LV_FONT_MONTSERRAT_18 0
#define LV_FONT_MONTSERRAT_20 1
#define LV_FONT_MONTSERRAT_22 0
#define LV_FONT_MONTSERRAT_24 0
#define LV_FONT_MONTSERRAT_26 0
#define LV_FONT_MONTSERRAT_28 0
#define LV_FONT_MONTSERRAT_30 0
#define LV_FONT_MONTSERRAT_32 0
#define LV_FONT_MONTSERRAT_34 0
#define LV_FONT_MONTSERRAT_36 0
#define LV_FONT_MONTSERRAT_38 0
#define LV_FONT_MONTSERRAT_40 0
#define LV_FONT_MONTSERRAT_42 0
#define LV_FONT_MONTSERRAT_44 0
#define LV_FONT_MONTSERRAT_46 0
#define LV_FONT_MONTSERRAT_48 0

/** Declare the type of the user data of fonts
 * can be e.g. `void *`, `int`, `struct` */
typedef void * lv_font_user_data_t;

/*====================
   THEME SETTINGS
 *====================*/

/** A simple, impressive and very complete theme */
#define LV_USE_THEME_DEFAULT 1
#if LV_USE_THEME_DEFAULT

/** 0: Light mode; 1: Dark mode */
#define LV_THEME_DEFAULT_DARK 0

/** 1: Enable grow on press */
#define LV_THEME_DEFAULT_GROW 1

/** Default transition time in [ms] */
#define LV_THEME_DEFAULT_TRANSITION_TIME 80
#endif /*LV_USE_THEME_DEFAULT*/

/** A very simple theme that is a good starting point for a custom theme */
#define LV_USE_THEME_SIMPLE 1

/** A theme designed for monochrome displays */
#define LV_USE_THEME_MONO 1

/*====================
   LAYOUT USAGE
 *====================*/

/** A layout similar to Flexbox in CSS. */
#define LV_USE_FLEX 1

/** A layout similar to Grid in CSS. */
#define LV_USE_GRID 1

/*====================
   WIDGET USAGE
 *====================*/

/** Documentation of the widgets: https://docs.lvgl.io/latest/en/html/widgets/index.html */

#define LV_USE_ANIMIMG    1
#define LV_USE_ARC        1
#define LV_USE_BAR        1
#define LV_USE_BTN        1
#define LV_USE_BTNMATRIX  1
#define LV_USE_CALENDAR   1
#define LV_USE_CANVAS     1
#define LV_USE_CHART      1
#define LV_USE_CHECKBOX   1
#define LV_USE_COLORWHEEL 1
#define LV_USE_DROPDOWN   1
#define LV_USE_IMG        1
#define LV_USE_IMGBTN     1
#define LV_USE_KEYBOARD   1
#define LV_USE_LABEL      1
#define LV_USE_LED        1
#define LV_USE_LINE       1
#define LV_USE_LIST       1
#define LV_USE_LOTTIE     0
#define LV_USE_MENU       1
#define LV_USE_METER      1
#define LV_USE_MSGBOX     1
#define LV_USE_ROLLER     1
#define LV_USE_SCALE      1
#define LV_USE_SLIDER     1
#define LV_USE_SPAN       1
#define LV_USE_SPINBOX    1
#define LV_USE_SPINNER    1
#define LV_USE_SWITCH     1
#define LV_USE_TABLE      1
#define LV_USE_TABVIEW    1
#define LV_USE_TEXTAREA   1
#define LV_USE_TILEVIEW   1
#define LV_USE_WIN        1

/*====================
   OPERATING SYSTEM
 *====================*/

/** Select an operating system to use. Possible options:
 * - LV_OS_NONE
 * - LV_OS_PTHREAD
 * - LV_OS_FREERTOS
 * - LV_OS_CMSIS_RTOS2
 * - LV_OS_RTTHREAD
 * - LV_OS_WINDOWS
 * - LV_OS_CUSTOM
 */
#define LV_USE_OS LV_OS_FREERTOS

#if LV_USE_OS == LV_OS_FREERTOS
    #define LV_OS_FREERTOS_INCLUDE_PATH "freertos/FreeRTOS.h"
#endif

/*===================
 * TICK CONFIGURATION
 *==================*/

/** 1: use a custom tick source that tells the elapsed time in milliseconds.
 * It removes the need to manually update the tick with `lv_tick_inc()`) */
#define LV_TICK_CUSTOM 1
#if LV_TICK_CUSTOM
    #define LV_TICK_CUSTOM_SYS_TIME_EXPR (xTaskGetTickCount() * portTICK_PERIOD_MS)
#endif   /*LV_TICK_CUSTOM*/

/*====================
   OTHERS
 *====================*/

/** 1: Enable the runtime performance profiler */
#define LV_USE_PROFILER 0

/** 1: Enable Monkey test */
#define LV_USE_MONKEY 0

/** 1: Enable grid navigation */
#define LV_USE_GRIDNAV 0

/** 1: Enable lv_obj fragment */
#define LV_USE_FRAGMENT 0

/** 1: Support using images as font in label or span widgets */
#define LV_USE_IMGFONT 0

/*==================
 * IMAGE DECODERS
 *==================*/

/** PNG decoder library */
#define LV_USE_PNG 1

/** BMP decoder library */
#define LV_USE_BMP 1

/** JPG + split JPG decoder library.
 * Split JPG is a custom format optimized for embedded systems. */
#define LV_USE_SJPG 0

/** GIF decoder library */
#define LV_USE_GIF 0

/** QR code library */
#define LV_USE_QRCODE 0

/** Barcode code library */
#define LV_USE_BARCODE 0

/** 1: Enable a published subscriber based messaging system */
#define LV_USE_MSG 0

/** 1: Enable Pinyin input method */
#define LV_USE_IME_PINYIN 0

/** 1: Enable file explorer */
#define LV_USE_FILE_EXPLORER 0

/*==================
 * PLATFORM-SPECIFIC
 *==================*/

/** Disable ARM Helium optimizations for ESP32 */
#define LV_USE_DRAW_SW_ASM LV_DRAW_SW_ASM_NONE
#define LV_USE_NATIVE_HELIUM_ASM 0

/*==================
 *   DEVICE DRIVERS
 *==================*/

/** 1: Enable SDL display device driver (monitor and mouse) */
#define LV_USE_SDL 0

/*==================
 * EXAMPLES
 *==================*/

/** Enable the examples to be built with the library */
#define LV_BUILD_EXAMPLES 0

/*--END OF LV_CONF_H--*/

#endif /*LV_CONF_H*/
