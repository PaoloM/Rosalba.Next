#pragma once

#include <Arduino.h>
#include <SD.h>
#include <SPI.h>

// SD card SPI driver functions
void sdspi_init(int sck, int miso, int mosi, int cs);
bool sdspi_mount();
void sdspi_unmount();

// RGB LED functions
void rgb_init(int red_pin, int green_pin, int blue_pin);
void rgb_set_color(int red, int green, int blue);