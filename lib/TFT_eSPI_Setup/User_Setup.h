// User_Setup.h for ILI9486/ILI9488 with ESP32
// Copy this to your TFT_eSPI library folder after installation

// #define ILI9488_DRIVER     // Use this for ILI9488 displays 
#define ILI9486_DRIVER  // Use this for ILI9486 displays (trying this first for white screen fix)

#define TFT_MISO 19
#define TFT_MOSI 23
#define TFT_SCLK 18
#define TFT_CS   15  // Chip select control pin
#define TFT_DC    2  // Data Command control pin
#define TFT_RST   4  // Reset pin (could connect to RST pin)

// Optional - for better performance
#define LOAD_GLCD   // Font 1. Original Adafruit 8 pixel font needs ~1820 bytes in FLASH
#define LOAD_FONT2  // Font 2. Small 16 pixel high font, needs ~3534 bytes in FLASH, 96 characters
#define LOAD_FONT4  // Font 4. Medium 26 pixel high font, needs ~5848 bytes in FLASH, 96 characters
#define LOAD_FONT6  // Font 6. Large 48 pixel font, needs ~2666 bytes in FLASH, only characters 1234567890:-.apm
#define LOAD_FONT7  // Font 7. 7 segment 48 pixel font, needs ~2438 bytes in FLASH, only characters 1234567890:.
#define LOAD_FONT8  // Font 8. Large 75 pixel font needs ~3256 bytes in FLASH, only characters 1234567890:-.
#define LOAD_GFXFF  // FreeFonts. Include access to the 48 Adafruit_GFX free fonts FF1 to FF48 and custom fonts

// SPI frequency - very slow for troubleshooting white screen
#define SPI_FREQUENCY  10000000 // 10MHz for maximum compatibility 
// #define SPI_FREQUENCY  20000000 // Try 20MHz if 10MHz works
// #define SPI_FREQUENCY  40000000 // Try 40MHz if lower speeds work

// Optional reduced SPI frequency for reading TFT
#define SPI_READ_FREQUENCY  16000000