#pragma once

#include <Arduino.h>
#include <SD.h>
#include <SPI.h>

// SD card SPI driver functions
void sdspi_init(int sck, int miso, int mosi, int cs);
bool sdspi_mount();
void sdspi_unmount();
