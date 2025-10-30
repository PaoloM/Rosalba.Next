#include "main_ui.h"
#include <Arduino.h>
#include <SD.h>



// Global UI instance
lv_ui guider_main_ui;

// PNG loading function using LVGL's built-in decoder
bool load_png_from_sd(const char* path, lv_obj_t* parent) {
    Serial.printf("Loading PNG: %s\n", path);
    
    // First check if SD card is working and file exists
    if (!SD.exists(path)) {
        Serial.printf("File does not exist: %s\n", path);
        Serial.println("Checking SD card contents...");
        
        // Try to list root directory
        File root = SD.open("/");
        if (root) {
            Serial.println("SD card root directory contents:");
            File file = root.openNextFile();
            while (file) {
                if (file.isDirectory()) {
                    Serial.printf("  DIR: %s\n", file.name());
                } else {
                    Serial.printf("  FILE: %s (%d bytes)\n", file.name(), file.size());
                }
                file = root.openNextFile();
            }
            root.close();
            
            // Check data directory specifically
            File dataDir = SD.open("/data");
            if (dataDir) {
                Serial.println("/data directory contents:");
                File dataFile = dataDir.openNextFile();
                while (dataFile) {
                    if (dataFile.isDirectory()) {
                        Serial.printf("  DIR: %s\n", dataFile.name());
                    } else {
                        Serial.printf("  FILE: %s (%d bytes)\n", dataFile.name(), dataFile.size());
                    }
                    dataFile = dataDir.openNextFile();
                }
                dataDir.close();
            } else {
                Serial.println("/data directory does not exist!");
            }
        } else {
            Serial.println("Failed to open SD card root directory!");
        }
        return false;
    }
    
    Serial.printf("File exists: %s\n", path);
    
    // Try to load PNG directly using LVGL's file system support
    lv_obj_t* img = lv_image_create(parent);
    if (!img) {
        Serial.println("Failed to create image object");
        return false;
    }
    
    // LVGL might not support direct file paths, so we need to load manually
    Serial.println("Direct path loading - trying LVGL file system support...");
    lv_image_set_src(img, path);
    
    // Check if the image loaded successfully by checking object size
    lv_coord_t img_w = lv_obj_get_width(img);
    lv_coord_t img_h = lv_obj_get_height(img);
    if (img_w > 1 && img_h > 1) {  // Changed from > 0 to > 1 for better detection
        Serial.printf("PNG loaded successfully via direct path: %dx%d\n", img_w, img_h);
        lv_obj_center(img);
        return true;
    }
    
    Serial.println("Direct path failed, trying manual file loading...");
    
    // If direct loading failed, try loading file data manually
    Serial.println("Direct loading failed, trying manual file loading...");
    
    File file = SD.open(path);
    if (!file) {
        Serial.printf("Failed to open PNG file: %s\n", path);
        lv_obj_del(img);
        return false;
    }
    
    size_t file_size = file.size();
    Serial.printf("PNG file size: %d bytes\n", file_size);
    
    // Allocate buffer for PNG data
    uint8_t* png_data = (uint8_t*)malloc(file_size);
    if (!png_data) {
        Serial.println("Failed to allocate memory for PNG data");
        file.close();
        lv_obj_del(img);
        return false;
    }

    // Read PNG file into memory
    if (file.read(png_data, file_size) != file_size) {
        Serial.println("Failed to read PNG file");
        free(png_data);
        file.close();
        lv_obj_del(img);
        return false;
    }

    file.close();

    // Check PNG header for validity
    if (file_size >= 8) {
        Serial.printf("PNG header check: %02X %02X %02X %02X %02X %02X %02X %02X\n",
            png_data[0], png_data[1], png_data[2], png_data[3],
            png_data[4], png_data[5], png_data[6], png_data[7]);
        
        // Valid PNG header should be: 89 50 4E 47 0D 0A 1A 0A
        bool valid_header = (png_data[0] == 0x89 && png_data[1] == 0x50 && 
                           png_data[2] == 0x4E && png_data[3] == 0x47 &&
                           png_data[4] == 0x0D && png_data[5] == 0x0A &&
                           png_data[6] == 0x1A && png_data[7] == 0x0A);
        Serial.printf("PNG header valid: %s\n", valid_header ? "YES" : "NO");
        
        if (!valid_header) {
            Serial.println("Invalid PNG header - file may be corrupted");
            free(png_data);
            lv_obj_del(img);
            return false;
        }
    }

    // Check LVGL PNG decoder support
    #ifdef LV_USE_PNG
        #if LV_USE_PNG
            Serial.println("LVGL PNG support: ENABLED");
        #else
            Serial.println("LVGL PNG support: DISABLED");
        #endif
    #else
        Serial.println("LVGL PNG support: NOT DEFINED");
    #endif

    // Create LVGL image descriptor with PNG format
    static lv_image_dsc_t png_dsc = {
        .header = {
            .magic = LV_IMAGE_HEADER_MAGIC,
            .cf = LV_COLOR_FORMAT_RAW,
            .flags = 0,
            .w = 0,
            .h = 0,
            .stride = 0
        },
        .data_size = file_size,
        .data = png_data
    };

    // Set the image source
    lv_image_set_src(img, &png_dsc);    // Force LVGL to process the image
    lv_obj_invalidate(img);
    lv_refr_now(NULL);  // Force a refresh
    
    // Check if it loaded successfully by checking object size
    img_w = lv_obj_get_width(img);
    img_h = lv_obj_get_height(img);
    if (img_w > 1 && img_h > 1) {  // Changed from > 0 to > 1
        Serial.printf("PNG loaded manually: %dx%d\n", img_w, img_h);
        lv_obj_center(img);
        return true;
    } else {
        Serial.printf("PNG decode failed - image size: %dx%d\n", img_w, img_h);
        Serial.println("Trying alternative color format...");
        
        // Try with different color format
        png_dsc.header.cf = LV_COLOR_FORMAT_ARGB8888;
        lv_image_set_src(img, &png_dsc);
        
        lv_obj_invalidate(img);
        lv_refr_now(NULL);
        
        img_w = lv_obj_get_width(img);
        img_h = lv_obj_get_height(img);
        if (img_w > 1 && img_h > 1) {
            Serial.printf("PNG loaded with ARGB8888 format: %dx%d\n", img_w, img_h);
            lv_obj_center(img);
            return true;
        }
        
        // Try with RGB565 format
        Serial.println("Trying RGB565 format...");
        png_dsc.header.cf = LV_COLOR_FORMAT_RGB565;
        lv_image_set_src(img, &png_dsc);
        
        lv_obj_invalidate(img);
        lv_refr_now(NULL);
        
        img_w = lv_obj_get_width(img);
        img_h = lv_obj_get_height(img);
        if (img_w > 1 && img_h > 1) {
            Serial.printf("PNG loaded with RGB565 format: %dx%d\n", img_w, img_h);
            lv_obj_center(img);
            return true;
        }
        
        Serial.println("All PNG loading attempts failed");
        free(png_data);
        lv_obj_del(img);
        return false;
    }
}





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
    
    Serial.println("Loading PNG logo...");
    
    // Try to load PNG file
    if (load_png_from_sd("/data/abarth_logo.png", ui->main)) {
        Serial.println("Abarth PNG logo loaded successfully!");
        
        // Add a subtle label below the image
        lv_obj_t *logo_label = lv_label_create(ui->main);
        lv_label_set_text(logo_label, "ABARTH");
        lv_obj_set_style_text_color(logo_label, lv_color_make(0xFF, 0x00, 0x00), LV_PART_MAIN);
        lv_obj_set_style_text_font(logo_label, &lv_font_montserrat_12, LV_PART_MAIN);
        lv_obj_align(logo_label, LV_ALIGN_CENTER, 0, 120); // Position below center
    } else {
        Serial.println("Failed to load PNG logo");
    }
    
    // Create bottom status label
    lv_obj_t *status_label = lv_label_create(ui->main);
    lv_label_set_text(status_label, "LVGL v9.4 - PNG Image Display");
    lv_obj_set_style_text_color(status_label, lv_color_make(0x80, 0x80, 0x80), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_align(status_label, LV_ALIGN_BOTTOM_MID, 0, -20);
    
    Serial.println("Main UI setup complete with PNG image display");
}

void events_init_main(lv_ui *ui) {
    // Initialize events for main screen
    // Add event handlers here if needed
}