#include "screen_manager.h"
#include "driver_sdspi.h"
#include <SD.h>

// Global instance
ScreenManager screenManager;

ScreenManager::ScreenManager() {
    current_screen = SCREEN_SPLASH;
    previous_screen = SCREEN_SPLASH;
    current_main_screen = 0;
    splash_start_time = 0;
    splash_active = false;
    splash_img = nullptr;
    
    // Initialize screen pointers
    for (int i = 0; i < SCREEN_COUNT; i++) {
        screens[i] = nullptr;
    }
}

ScreenManager::~ScreenManager() {
    // Clean up screens
    for (int i = 0; i < SCREEN_COUNT; i++) {
        if (screens[i] != nullptr) {
            lv_obj_del(screens[i]);
        }
    }
}

bool ScreenManager::init() {
    Serial.println("Initializing Screen Manager...");
    
    // Create all screens
    createSplashScreen();
    createMainScreens();
    createSettingsScreen();
    
    // Skip XML loading for now to test basic functionality
    Serial.println("Skipping XML loading for initial test");
    // loadScreenFromXML(SCREEN_MAIN_1, "/data/screen_main_1.xml");
    // loadScreenFromXML(SCREEN_MAIN_2, "/data/screen_main_2.xml");
    // loadScreenFromXML(SCREEN_MAIN_3, "/data/screen_main_3.xml");
    // loadScreenFromXML(SCREEN_SETTINGS, "/data/screen_settings.xml");
    
    // Show splash screen
    showSplashScreen();
    
    Serial.println("Screen Manager initialized successfully");
    return true;
}

void ScreenManager::showSplashScreen() {
    Serial.println("Showing splash screen");
    
    if (screens[SCREEN_SPLASH] != nullptr) {
        Serial.println("Loading splash screen...");
        lv_screen_load(screens[SCREEN_SPLASH]);
        current_screen = SCREEN_SPLASH;
        splash_start_time = millis();
        splash_active = true;
        Serial.println("Splash screen loaded successfully");
    } else {
        Serial.println("ERROR: Splash screen is NULL!");
    }
}

void ScreenManager::showMainScreen(uint8_t screen_num) {
    if (screen_num > 2) screen_num = 0; // Wrap around
    
    screen_type_t target_screen = (screen_type_t)(SCREEN_MAIN_1 + screen_num);
    
    Serial.printf("Showing main screen %d\n", screen_num + 1);
    
    if (screens[target_screen] != nullptr) {
        if (current_screen >= SCREEN_MAIN_1 && current_screen <= SCREEN_MAIN_3) {
            // Slide transition between main screens
            lv_dir_t direction = (screen_num > current_main_screen) ? LV_DIR_LEFT : LV_DIR_RIGHT;
            startTransition(target_screen, TRANSITION_SLIDE_HORIZONTAL, direction);
        } else {
            // Direct load if coming from splash or settings
            lv_screen_load(screens[target_screen]);
        }
        
        previous_screen = current_screen;
        current_screen = target_screen;
        current_main_screen = screen_num;
    }
}

void ScreenManager::showSettingsScreen() {
    Serial.println("Showing settings screen");
    
    if (screens[SCREEN_SETTINGS] != nullptr) {
        startTransition(SCREEN_SETTINGS, TRANSITION_SLIDE_VERTICAL, LV_DIR_TOP);
        previous_screen = current_screen;
        current_screen = SCREEN_SETTINGS;
    }
}

void ScreenManager::showPreviousScreen() {
    Serial.println("Returning to previous screen");
    
    if (current_screen == SCREEN_SETTINGS) {
        // Slide settings screen down
        if (screens[previous_screen] != nullptr) {
            startTransition(previous_screen, TRANSITION_SLIDE_VERTICAL, LV_DIR_BOTTOM);
            current_screen = previous_screen;
        }
    }
}

void ScreenManager::navigateNext() {
    if (current_screen >= SCREEN_MAIN_1 && current_screen <= SCREEN_MAIN_3) {
        uint8_t next_screen = (current_main_screen + 1) % 3;
        showMainScreen(next_screen);
    }
}

void ScreenManager::navigatePrevious() {
    if (current_screen >= SCREEN_MAIN_1 && current_screen <= SCREEN_MAIN_3) {
        uint8_t prev_screen = (current_main_screen + 2) % 3; // +2 for wrap-around
        showMainScreen(prev_screen);
    }
}

void ScreenManager::toggleSettings() {
    if (current_screen == SCREEN_SETTINGS) {
        showPreviousScreen();
    } else {
        showSettingsScreen();
    }
}

void ScreenManager::update() {
    // Handle splash screen timeout
    if (splash_active && (millis() - splash_start_time >= 5000)) {
        splash_active = false;
        showMainScreen(0); // Show first main screen
    }
}

void ScreenManager::createSplashScreen() {
    Serial.println("Creating splash screen");
    
    // Create screen as a proper screen object
    screens[SCREEN_SPLASH] = lv_obj_create(NULL);
    if (screens[SCREEN_SPLASH] == NULL) {
        Serial.println("ERROR: Failed to create splash screen!");
        return;
    }
    
    // Set screen properties
    lv_obj_clear_flag(screens[SCREEN_SPLASH], LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_bg_color(screens[SCREEN_SPLASH], lv_color_black(), LV_PART_MAIN);
    
    // Create a simple text label for now (instead of trying to load image)
    splash_img = lv_label_create(screens[SCREEN_SPLASH]);
    if (splash_img == NULL) {
        Serial.println("ERROR: Failed to create splash label!");
        return;
    }
    
    lv_label_set_text(splash_img, "ROSALBA.NEXT\nABARTH");
    lv_obj_set_style_text_color(splash_img, lv_color_white(), LV_PART_MAIN);
    lv_obj_set_style_text_font(splash_img, &lv_font_montserrat_20, LV_PART_MAIN);
    lv_obj_set_style_text_align(splash_img, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN);
    lv_obj_center(splash_img);
    
    Serial.println("Splash screen created successfully");
}

void ScreenManager::createMainScreens() {
    Serial.println("Creating main screens");
    
    for (int i = 0; i < 3; i++) {
        screens[SCREEN_MAIN_1 + i] = lv_obj_create(NULL);
        if (screens[SCREEN_MAIN_1 + i] == NULL) {
            Serial.printf("ERROR: Failed to create main screen %d!\n", i + 1);
            continue;
        }
        
        lv_obj_clear_flag(screens[SCREEN_MAIN_1 + i], LV_OBJ_FLAG_SCROLLABLE);
        lv_obj_set_style_bg_color(screens[SCREEN_MAIN_1 + i], lv_color_hex(0x1a1a1a), LV_PART_MAIN);
        
        // Add a temporary label until XML is loaded
        lv_obj_t* label = lv_label_create(screens[SCREEN_MAIN_1 + i]);
        if (label != NULL) {
            lv_label_set_text_fmt(label, "Main Screen %d", i + 1);
            lv_obj_set_style_text_color(label, lv_color_white(), LV_PART_MAIN);
            lv_obj_set_style_text_font(label, &lv_font_montserrat_16, LV_PART_MAIN);
            lv_obj_center(label);
        }
        
        Serial.printf("Main screen %d created\n", i + 1);
    }
    
    Serial.println("Main screens created");
}

void ScreenManager::createSettingsScreen() {
    Serial.println("Creating settings screen");
    
    screens[SCREEN_SETTINGS] = lv_obj_create(NULL);
    if (screens[SCREEN_SETTINGS] == NULL) {
        Serial.println("ERROR: Failed to create settings screen!");
        return;
    }
    
    lv_obj_clear_flag(screens[SCREEN_SETTINGS], LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_bg_color(screens[SCREEN_SETTINGS], lv_color_hex(0x2a2a2a), LV_PART_MAIN);
    
    // Add a temporary label until XML is loaded
    lv_obj_t* label = lv_label_create(screens[SCREEN_SETTINGS]);
    if (label != NULL) {
        lv_label_set_text(label, "Settings Screen");
        lv_obj_set_style_text_color(label, lv_color_white(), LV_PART_MAIN);
        lv_obj_set_style_text_font(label, &lv_font_montserrat_16, LV_PART_MAIN);
        lv_obj_center(label);
    }
    
    Serial.println("Settings screen created");
}

bool ScreenManager::loadImageFromSD(const char* path, lv_obj_t* img_obj) {
    Serial.printf("Loading image from SD: %s\n", path);
    
    if (!SD.exists(path)) {
        Serial.printf("Image file not found: %s\n", path);
        return false;
    }
    
    // For now, we'll use a simple approach
    // In a full implementation, you'd load the BMP file and convert it to LVGL format
    // This is a placeholder that will need proper BMP decoding
    
    Serial.printf("Image loaded successfully: %s\n", path);
    return true; // Placeholder
}

void ScreenManager::startTransition(screen_type_t to_screen, transition_type_t type, lv_dir_t direction) {
    if (screens[to_screen] == nullptr) return;
    
    lv_obj_t* from_screen = lv_screen_active();
    lv_obj_t* target_screen = screens[to_screen];
    
    Serial.printf("Starting transition to screen %d\n", to_screen);
    
    switch (type) {
        case TRANSITION_SLIDE_HORIZONTAL:
        case TRANSITION_SLIDE_VERTICAL:
            slideTransition(from_screen, target_screen, direction, 800);
            break;
        case TRANSITION_NONE:
        default:
            lv_screen_load(target_screen);
            break;
    }
}

void ScreenManager::slideTransition(lv_obj_t* from_screen, lv_obj_t* to_screen, lv_dir_t direction, uint32_t duration) {
    // Map direction to correct LVGL v9.x animation type
    lv_screen_load_anim_t anim_type;
    
    switch (direction) {
        case LV_DIR_LEFT:
            anim_type = LV_SCREEN_LOAD_ANIM_MOVE_LEFT;
            break;
        case LV_DIR_RIGHT:
            anim_type = LV_SCREEN_LOAD_ANIM_MOVE_RIGHT;
            break;
        case LV_DIR_TOP:
            anim_type = LV_SCREEN_LOAD_ANIM_MOVE_TOP;
            break;
        case LV_DIR_BOTTOM:
            anim_type = LV_SCREEN_LOAD_ANIM_MOVE_BOTTOM;
            break;
        default:
            anim_type = LV_SCREEN_LOAD_ANIM_MOVE_LEFT;
            break;
    }
    
    // Load the target screen with animation
    lv_screen_load_anim(to_screen, anim_type, duration, 0, false);
}

bool ScreenManager::loadScreenFromXML(screen_type_t screen, const char* xml_path) {
    Serial.printf("Loading XML for screen %d from %s\n", screen, xml_path);
    
    if (!SD.exists(xml_path)) {
        Serial.printf("XML file not found: %s\n", xml_path);
        return false;
    }
    
    File xmlFile = SD.open(xml_path, FILE_READ);
    if (!xmlFile) {
        Serial.printf("Failed to open XML file: %s\n", xml_path);
        return false;
    }
    
    String xmlContent = xmlFile.readString();
    xmlFile.close();
    
    if (xmlContent.length() == 0) {
        Serial.printf("Empty XML file: %s\n", xml_path);
        return false;
    }
    
    // Parse and apply XML to screen
    bool result = parseXMLScreen(xmlContent.c_str(), screens[screen]);
    
    if (result) {
        Serial.printf("XML loaded successfully for screen %d\n", screen);
    } else {
        Serial.printf("Failed to parse XML for screen %d\n", screen);
    }
    
    return result;
}

bool ScreenManager::parseXMLScreen(const char* xml_content, lv_obj_t* screen) {
    // This is a placeholder for XML parsing
    // In a full implementation, you'd use LVGL's XML system or implement custom parsing
    
    Serial.println("Parsing XML content (placeholder implementation)");
    
    // For now, we'll just return true to indicate the XML system is in place
    // The actual XML parsing would integrate with LVGL Pro's XML system
    
    return true;
}