#include <stdint.h>
#include "display.h"

// Generic ESP32 configuration - define your screen resolution here
#define TFT_SCREEN_WIDTH 320
#define TFT_SCREEN_HEIGHT 480

// Touch screen is optional - comment out if not used
#ifndef _TFT_Touch_H
  #include "TFT_Touch.h"
  #if !defined(_TFT_Touch_H)
    #error "Please install TFT_Touch library before using this library!"
  #endif
#endif

// Touch screen pin definitions - adjust for your hardware
#define DOUT 39 /* Data out pin (T_DO) of touch screen */
#define DIN 32  /* Data in pin (T_DIN) of touch screen */
#define DCS 33  /* Chip select pin (T_CS) of touch screen */
#define DCLK 25 /* Clock pin (T_CLK) of touch screen */

TFT_Touch touch = TFT_Touch(DCS, DCLK, DIN, DOUT);

static const uint16_t screenWidth = TFT_SCREEN_WIDTH;
static const uint16_t screenHeight = TFT_SCREEN_HEIGHT;
static const uint16_t screenHeightBuf = TFT_SCREEN_HEIGHT / 10;
static lv_draw_buf_t draw_buf;
static lv_color_t draw_buf1[screenWidth * screenHeightBuf];
//static lv_color_t draw_buf2[screenWidth * screenHeightBuf];

TFT_eSPI tft = TFT_eSPI(screenWidth, screenHeight); /* TFT instance */

#if LV_USE_LOG != 0
/* Serial debugging */
void my_print(const char *buf) {
  Serial.printf(buf);
  Serial.flush();
}
#endif

/* Display flushing */
void my_disp_flush(lv_display_t *disp, const lv_area_t *area, uint8_t *px_map) {
  uint32_t w = (area->x2 - area->x1 + 1);
  uint32_t h = (area->y2 - area->y1 + 1);
  tft.startWrite();
  tft.setAddrWindow(area->x1, area->y1, w, h);
  tft.pushColors((uint16_t *)px_map, w * h, true);
  tft.endWrite();
  lv_display_flush_ready(disp);
}

/*Read the touchpad*/
void my_touchpad_read(lv_indev_t *indev_driver, lv_indev_data_t *data) {
  uint16_t touchX, touchY;
  bool touched;
  
  // Try to read touch using external touch library first
  touched = touch.Pressed();
  if (touched) {
    touchX = touch.X();
    touchY = touch.Y();
  } else {
    // Fallback to TFT_eSPI built-in touch if available
    touched = tft.getTouch(&touchX, &touchY, 600);
  }
  
  if (!touched) {
    data->state = LV_INDEV_STATE_REL;
  } else {
    data->state = LV_INDEV_STATE_PR;
    
    // Generic touch coordinate mapping - adjust based on TFT_DIRECTION
    #ifndef TFT_DIRECTION
    #define TFT_DIRECTION 0
    #endif
    
    #if (TFT_DIRECTION == 0)
    data->point.x = touchX;
    data->point.y = touchY;
    #elif (TFT_DIRECTION == 1)
    data->point.x = map(touchY, 0, tft.height(), 0, tft.width());
    data->point.y = map((tft.width() - touchX), 0, tft.width(), 0, tft.height());
    #elif (TFT_DIRECTION == 2)
    data->point.x = tft.width() - touchX;
    data->point.y = tft.height() - touchY;
    #elif (TFT_DIRECTION == 3)
    data->point.x = map((tft.height() - touchY), 0, tft.height(), 0, tft.width());
    data->point.y = map(touchX, 0, tft.width(), 0, tft.height());
    #endif
    
    // Optional debug output
    // Serial.printf("Touch: x=%d, y=%d\n", data->point.x, data->point.y);
  }
}

void Display::init(void) {
  // Initialize touch calibration - adjust these values for your specific touch screen
  touch.setCal(526, 3443, 750, 3377, screenHeight, screenWidth, 1);
  
  // Alternative: TFT_eSPI built-in touch calibration
  uint16_t calData[5] = { 286, 3534, 283, 3600, 6 };
  tft.setTouch(calData);

#if LV_USE_LOG != 0
  // Note: LVGL v9.x log callback signature changed
  // lv_log_register_print_cb(my_print); /* register print function for debugging */
#endif

  Serial.println("Initializing TFT display...");
  tft.begin();                    /* TFT init */
  
  // Set rotation - define TFT_DIRECTION in your build flags or here
  #ifndef TFT_DIRECTION
  #define TFT_DIRECTION 0
  #endif
  tft.setRotation(TFT_DIRECTION); /* Set display orientation */

  Serial.println("Initializing LVGL...");
  lv_init();
  
  /*Initialize the display*/
  lv_display_t *display;
  #if (TFT_DIRECTION % 2 == 0)
  display = lv_display_create(screenWidth, screenHeight);
  #else
  display = lv_display_create(screenHeight, screenWidth);
  #endif
  
  if (display == NULL) {
    Serial.println("ERROR: Failed to create LVGL display!");
    return;
  }
  
  lv_display_set_flush_cb(display, my_disp_flush);
  lv_display_set_buffers(display, draw_buf1, NULL, sizeof(draw_buf1), LV_DISPLAY_RENDER_MODE_PARTIAL);

  /*Initialize the input device*/
  lv_indev_t *indev = lv_indev_create();
  if (indev == NULL) {
    Serial.println("ERROR: Failed to create LVGL input device!");
    return;
  }
  
  lv_indev_set_type(indev, LV_INDEV_TYPE_POINTER);
  lv_indev_set_read_cb(indev, my_touchpad_read);
  
  Serial.println("Display initialization complete");
}

void Display::routine(void) {
  lv_timer_handler();
}
