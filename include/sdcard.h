#pragma once
#include <Arduino.h>
#include <SD.h>
#include <FS.h>

// SD Card configuration
#define SD_CS_PIN 5  // Connect to display Pin 26 (FLASH_CS)

// SD Card functionality
struct SDCardManager {
    bool initialized = false;
    String currentLogFile;
    
    // Initialize SD card
    bool init() {
        if (SD.begin(SD_CS_PIN)) {
            initialized = true;
            Serial.println("SD Card initialized successfully");
            createDirectories();
            return true;
        } else {
            Serial.println("SD Card initialization failed");
            return false;
        }
    }
    
    // Create directory structure
    void createDirectories() {
        SD.mkdir("/logs");
        SD.mkdir("/images");
        SD.mkdir("/config");
        SD.mkdir("/exports");
    }
    
    // Start a new driving session log
    bool startDrivingSession() {
        if (!initialized) return false;
        
        // Create timestamped log file
        currentLogFile = "/logs/drive_" + getCurrentTimestamp() + ".csv";
        File logFile = SD.open(currentLogFile, FILE_WRITE);
        
        if (logFile) {
            // Write CSV header
            logFile.println("timestamp,rpm,speed,boost_psi,coolant_temp,throttle_pos,sport_mode");
            logFile.close();
            Serial.println("Started new driving session: " + currentLogFile);
            return true;
        }
        return false;
    }
    
    // Log vehicle data to SD card
    bool logVehicleData(VehicleData &data) {
        if (!initialized || currentLogFile.isEmpty()) return false;
        
        File logFile = SD.open(currentLogFile, FILE_APPEND);
        if (logFile) {
            // Write data row
            logFile.print(millis()); logFile.print(",");
            logFile.print(data.rpm); logFile.print(",");
            logFile.print(data.speed); logFile.print(",");
            logFile.print(data.boostPressure); logFile.print(",");
            logFile.print(data.coolantTemp); logFile.print(",");
            logFile.print(data.throttlePosition); logFile.print(",");
            logFile.println(data.sportMode ? "1" : "0");
            logFile.close();
            return true;
        }
        return false;
    }
    
    // Save diagnostic trouble codes
    bool logDTC(String code, String description) {
        if (!initialized) return false;
        
        File dtcFile = SD.open("/logs/dtc_log.txt", FILE_APPEND);
        if (dtcFile) {
            dtcFile.print("[");
            dtcFile.print(getCurrentTimestamp());
            dtcFile.print("] ");
            dtcFile.print(code);
            dtcFile.print(": ");
            dtcFile.println(description);
            dtcFile.close();
            return true;
        }
        return false;
    }
    
    // Load custom configuration
    bool loadConfig(String &config) {
        if (!initialized) return false;
        
        File configFile = SD.open("/config/abarth_config.json", FILE_READ);
        if (configFile) {
            config = configFile.readString();
            configFile.close();
            return true;
        }
        return false;
    }
    
    // Export data for analysis
    bool exportSessionData(String sessionFile) {
        if (!initialized) return false;
        
        // Copy to exports folder with readable name
        String exportName = "/exports/abarth_session_" + getCurrentTimestamp() + ".csv";
        
        File source = SD.open(sessionFile, FILE_READ);
        File dest = SD.open(exportName, FILE_WRITE);
        
        if (source && dest) {
            while (source.available()) {
                dest.write(source.read());
            }
            source.close();
            dest.close();
            Serial.println("Data exported to: " + exportName);
            return true;
        }
        return false;
    }
    
    // Get card info for display
    String getCardInfo() {
        if (!initialized) return "No SD Card";
        
        uint32_t cardSize = SD.cardSize() / (1024 * 1024);
        uint32_t usedSize = (SD.totalBytes() - SD.usedBytes()) / (1024 * 1024);
        
        return "SD: " + String(cardSize) + "MB (" + String(usedSize) + "MB free)";
    }
    
private:
    // Generate timestamp string
    String getCurrentTimestamp() {
        // Simple timestamp - could be enhanced with RTC
        return String(millis() / 1000);
    }
};

// Global SD card manager
extern SDCardManager sdCard;