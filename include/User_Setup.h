// TFT_eSPI setup for Waveshare ESP32-S3-Touch-LCD-3.5B
// Alternative configuration for troubleshooting black screen

// Try different driver options
#define ILI9488_DRIVER    // First choice
// #define ILI9486_DRIVER  // Alternative if ILI9488 doesn't work  
// #define ST7796_DRIVER   // Another alternative

// Display resolution  
#define TFT_WIDTH  480
#define TFT_HEIGHT 320

// Alternative pin assignments for ESP32-S3-Touch-LCD-3.5B
#define TFT_MISO 13  // SPI MISO (may not be connected)
#define TFT_MOSI 11  // SPI MOSI
#define TFT_SCLK 12  // SPI Clock  
#define TFT_CS   10  // Chip select
#define TFT_DC   14  // Data/Command
#define TFT_RST  1   // Reset (may be different)
#define TFT_BL   45  // Backlight control (try GPIO 45)

// SPI frequency settings (try slower speeds)
#define SPI_FREQUENCY       20000000  // 20MHz (slower for stability)
#define SPI_READ_FREQUENCY  10000000  // 10MHz for reading

// Color order
#define TFT_RGB_ORDER TFT_BGR

// Font configuration
#define LOAD_GLCD
#define LOAD_FONT2
#define LOAD_FONT4
#define LOAD_FONT6
#define SMOOTH_FONT

// Enable SPI transactions
#define SUPPORT_TRANSACTIONS