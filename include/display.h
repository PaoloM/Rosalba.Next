#pragma once
#include <TFT_eSPI.h>
#include "config.h"

// Function declarations
void initializeDisplay();
void updateDisplay();
void drawParameter(String label, String value, int x, int y, int textSize, uint16_t color);
void drawProgressBar(int x, int y, int width, int height, float percentage, String label, uint16_t color);
void showStartupScreen();
void showAbarthSplash();
void drawAbarthLogoVector();
void drawAbarthLogoEnhanced();
void drawAbarthLogoBitmap();
void drawAbarthLogoRGB();
void drawAbarthLogoFromSPIFFS();
bool drawBMPFromSPIFFS(const char* filename);
void showErrorScreen(String message);

// Global display object (declared in display.cpp)
extern TFT_eSPI tft;