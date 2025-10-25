#include "driver_sdspi.h"

static int rgb_red_pin = -1;
static int rgb_green_pin = -1;
static int rgb_blue_pin = -1;

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

void rgb_init(int red_pin, int green_pin, int blue_pin) {
    rgb_red_pin = red_pin;
    rgb_green_pin = green_pin;
    rgb_blue_pin = blue_pin;
    
    pinMode(red_pin, OUTPUT);
    pinMode(green_pin, OUTPUT);
    pinMode(blue_pin, OUTPUT);
    
    // Initialize to off
    digitalWrite(red_pin, LOW);
    digitalWrite(green_pin, LOW);
    digitalWrite(blue_pin, LOW);
}

void rgb_set_color(int red, int green, int blue) {
    if (rgb_red_pin >= 0) {
        analogWrite(rgb_red_pin, red);
    }
    if (rgb_green_pin >= 0) {
        analogWrite(rgb_green_pin, green);
    }
    if (rgb_blue_pin >= 0) {
        analogWrite(rgb_blue_pin, blue);
    }
}