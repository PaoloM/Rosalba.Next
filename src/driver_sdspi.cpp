#include "driver_sdspi.h"

void sdspi_init(int sck, int miso, int mosi, int cs) {
    // Initialize SD card SPI
    SPI.begin(sck, miso, mosi, cs);
    
    if (!SD.begin(cs)) {
        Serial.println("SD Card initialization failed!");
        return;
    }
    
    Serial.println("SD Card initialized successfully");
}

bool sdspi_mount() {
    // SD card mount logic
    return true;
}

void sdspi_unmount() {
    // SD card unmount logic
    SD.end();
}