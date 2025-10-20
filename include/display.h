#pragma once

#include <Arduino.h>
#include "config.h"
#include "obd.h"

// Display functions
void initializeDisplay();
void showAbarthLogo();
void showStartupScreen();
void updateDisplay();
void displayVehicleData(const VehicleData& data);
void displayConnectionStatus(bool connected);

// Logo and graphics functions
void drawAbarthLogo(int x, int y, int scale = 1);
void drawAbarthText(int x, int y, int scale = 1);
void drawScorpion(int x, int y, int scale = 1);

// Display utilities
void clearScreen();
void setTextStyle(uint16_t color, int size = 2);
void drawProgressBar(int x, int y, int width, int height, int percentage, uint16_t color);