#pragma once

#include <lvgl.h>
#include <Arduino.h>

// Screen types
typedef enum {
    SCREEN_SPLASH = 0,
    SCREEN_MAIN_1,
    SCREEN_MAIN_2,
    SCREEN_MAIN_3,
    SCREEN_SETTINGS,
    SCREEN_COUNT
} screen_type_t;

// Transition types
typedef enum {
    TRANSITION_NONE = 0,
    TRANSITION_SLIDE_HORIZONTAL,
    TRANSITION_SLIDE_VERTICAL
} transition_type_t;

// Screen manager class
class ScreenManager {
public:
    ScreenManager();
    ~ScreenManager();
    
    // Initialize the screen manager
    bool init();
    
    // Screen management
    void showSplashScreen();
    void showMainScreen(uint8_t screen_num);
    void showSettingsScreen();
    void showPreviousScreen();
    
    // Navigation
    void navigateNext();
    void navigatePrevious();
    void toggleSettings();
    
    // XML loading
    bool loadScreenFromXML(screen_type_t screen, const char* xml_path);
    
    // Update function (call in main loop)
    void update();
    
    // Get current screen
    screen_type_t getCurrentScreen() { return current_screen; }
    
private:
    // Screen objects
    lv_obj_t* screens[SCREEN_COUNT];
    lv_obj_t* splash_img;
    
    // Current state
    screen_type_t current_screen;
    screen_type_t previous_screen;
    uint8_t current_main_screen;
    
    // Timing
    unsigned long splash_start_time;
    bool splash_active;
    
    // Transitions
    void startTransition(screen_type_t to_screen, transition_type_t type, lv_dir_t direction);
    void slideTransition(lv_obj_t* from_screen, lv_obj_t* to_screen, lv_dir_t direction, uint32_t duration);
    
    // Screen creation
    void createSplashScreen();
    void createMainScreens();
    void createSettingsScreen();
    
    // Image loading
    bool loadImageFromSD(const char* path, lv_obj_t* img_obj);
    
    // XML parsing
    bool parseXMLScreen(const char* xml_content, lv_obj_t* screen);
};

// Global screen manager instance
extern ScreenManager screenManager;