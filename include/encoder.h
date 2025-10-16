#pragma once
#include <Arduino.h>

// KY-040 Rotary Encoder pins
#define ENCODER_CLK_PIN 12   // Clock pin (A)
#define ENCODER_DT_PIN 14    // Data pin (B) 
#define ENCODER_SW_PIN 27    // Switch pin (button)

// Encoder states
enum EncoderDirection {
    ENCODER_NO_CHANGE = 0,
    ENCODER_CLOCKWISE = 1,
    ENCODER_COUNTER_CLOCKWISE = -1
};

// Menu system states
enum MenuState {
    MENU_MAIN_DASHBOARD = 0,
    MENU_PERFORMANCE = 1,
    MENU_DIAGNOSTICS = 2,
    MENU_DATA_LOGS = 3,
    MENU_SETTINGS = 4,
    MENU_COUNT = 5
};

// Display modes within each menu
enum DisplayMode {
    DISPLAY_GAUGES = 0,
    DISPLAY_NUMBERS = 1,
    DISPLAY_GRAPH = 2,
    DISPLAY_MODE_COUNT = 3
};

// Settings menu options
enum SettingsOption {
    SETTING_BRIGHTNESS = 0,
    SETTING_UNITS = 1,
    SETTING_BOOST_WARNING = 2,
    SETTING_TEMP_WARNING = 3,
    SETTING_EXPORT_DATA = 4,
    SETTINGS_COUNT = 5
};

class RotaryEncoder {
private:
    volatile int lastCLK = HIGH;
    volatile int lastDT = HIGH;
    volatile unsigned long lastButtonPress = 0;
    volatile bool buttonPressed = false;
    volatile int encoderPosition = 0;
    
    // Debouncing
    unsigned long lastRotationTime = 0;
    static const unsigned long ROTATION_DEBOUNCE = 5; // 5ms debounce
    static const unsigned long BUTTON_DEBOUNCE = 200; // 200ms debounce
    
public:
    // Current menu and display states
    MenuState currentMenu = MENU_MAIN_DASHBOARD;
    DisplayMode currentDisplayMode = DISPLAY_GAUGES;
    SettingsOption currentSetting = SETTING_BRIGHTNESS;
    
    // Settings values
    int displayBrightness = 128; // 0-255
    bool useMetricUnits = true;
    float boostWarningThreshold = 20.0; // PSI
    float tempWarningThreshold = 110.0; // °C
    
    void init() {
        pinMode(ENCODER_CLK_PIN, INPUT_PULLUP);
        pinMode(ENCODER_DT_PIN, INPUT_PULLUP);
        pinMode(ENCODER_SW_PIN, INPUT_PULLUP);
        
        // Read initial states
        lastCLK = digitalRead(ENCODER_CLK_PIN);
        lastDT = digitalRead(ENCODER_DT_PIN);
        
        Serial.println("🎛️ KY-040 Rotary Encoder initialized");
        Serial.println("Controls:");
        Serial.println("- Rotate: Navigate menus/adjust values");
        Serial.println("- Press: Select/confirm");
    }
    
    // Check for rotation (call from main loop)
    EncoderDirection checkRotation() {
        int currentCLK = digitalRead(ENCODER_CLK_PIN);
        int currentDT = digitalRead(ENCODER_DT_PIN);
        
        unsigned long currentTime = millis();
        
        // Check for state change and debounce
        if (currentCLK != lastCLK && (currentTime - lastRotationTime) > ROTATION_DEBOUNCE) {
            lastRotationTime = currentTime;
            
            if (currentCLK == LOW) {
                // Determine direction
                if (currentDT == HIGH) {
                    encoderPosition++;
                    lastCLK = currentCLK;
                    return ENCODER_CLOCKWISE;
                } else {
                    encoderPosition--;
                    lastCLK = currentCLK;
                    return ENCODER_COUNTER_CLOCKWISE;
                }
            }
        }
        
        lastCLK = currentCLK;
        return ENCODER_NO_CHANGE;
    }
    
    // Check for button press
    bool checkButtonPress() {
        int buttonState = digitalRead(ENCODER_SW_PIN);
        unsigned long currentTime = millis();
        
        if (buttonState == LOW && !buttonPressed && (currentTime - lastButtonPress) > BUTTON_DEBOUNCE) {
            buttonPressed = true;
            lastButtonPress = currentTime;
            return true;
        } else if (buttonState == HIGH) {
            buttonPressed = false;
        }
        
        return false;
    }
    
    // Handle menu navigation
    void handleMenuNavigation(EncoderDirection direction) {
        if (direction == ENCODER_CLOCKWISE) {
            currentMenu = (MenuState)((currentMenu + 1) % MENU_COUNT);
        } else if (direction == ENCODER_COUNTER_CLOCKWISE) {
            currentMenu = (MenuState)((currentMenu - 1 + MENU_COUNT) % MENU_COUNT);
        }
        
        Serial.print("📱 Menu: ");
        Serial.println(getMenuName(currentMenu));
    }
    
    // Handle display mode cycling within current menu
    void handleDisplayModeToggle() {
        currentDisplayMode = (DisplayMode)((currentDisplayMode + 1) % DISPLAY_MODE_COUNT);
        
        Serial.print("🎨 Display Mode: ");
        Serial.println(getDisplayModeName(currentDisplayMode));
    }
    
    // Handle settings adjustment
    void handleSettingsAdjustment(EncoderDirection direction) {
        switch (currentSetting) {
            case SETTING_BRIGHTNESS:
                if (direction == ENCODER_CLOCKWISE && displayBrightness < 255) {
                    displayBrightness += 10;
                } else if (direction == ENCODER_COUNTER_CLOCKWISE && displayBrightness > 10) {
                    displayBrightness -= 10;
                }
                Serial.println("💡 Brightness: " + String(displayBrightness));
                break;
                
            case SETTING_UNITS:
                useMetricUnits = !useMetricUnits;
                Serial.println("📏 Units: " + String(useMetricUnits ? "Metric" : "Imperial"));
                break;
                
            case SETTING_BOOST_WARNING:
                if (direction == ENCODER_CLOCKWISE && boostWarningThreshold < 25.0) {
                    boostWarningThreshold += 0.5;
                } else if (direction == ENCODER_COUNTER_CLOCKWISE && boostWarningThreshold > 10.0) {
                    boostWarningThreshold -= 0.5;
                }
                Serial.println("🚨 Boost Warning: " + String(boostWarningThreshold) + " PSI");
                break;
                
            case SETTING_TEMP_WARNING:
                if (direction == ENCODER_CLOCKWISE && tempWarningThreshold < 130.0) {
                    tempWarningThreshold += 1.0;
                } else if (direction == ENCODER_COUNTER_CLOCKWISE && tempWarningThreshold > 80.0) {
                    tempWarningThreshold -= 1.0;
                }
                Serial.println("🌡️ Temp Warning: " + String(tempWarningThreshold) + "°C");
                break;
        }
    }
    
    // Get menu name for display
    const char* getMenuName(MenuState menu) {
        switch (menu) {
            case MENU_MAIN_DASHBOARD: return "Main Dashboard";
            case MENU_PERFORMANCE: return "Performance";
            case MENU_DIAGNOSTICS: return "Diagnostics"; 
            case MENU_DATA_LOGS: return "Data Logs";
            case MENU_SETTINGS: return "Settings";
            default: return "Unknown";
        }
    }
    
    // Get display mode name
    const char* getDisplayModeName(DisplayMode mode) {
        switch (mode) {
            case DISPLAY_GAUGES: return "Gauges";
            case DISPLAY_NUMBERS: return "Numbers";
            case DISPLAY_GRAPH: return "Graph";
            default: return "Unknown";
        }
    }
    
    // Get current encoder position
    int getPosition() { return encoderPosition; }
};

// Global encoder instance
extern RotaryEncoder encoder;