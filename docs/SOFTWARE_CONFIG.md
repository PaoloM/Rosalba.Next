# Software Configuration Guide

## PlatformIO Setup

### Project Structure
```
Rosalba.Next/
├── platformio.ini          # Build configuration
├── src/
│   └── main.cpp            # Main application code
├── lib/
│   └── TFT_eSPI_Setup/     # Display configuration
├── include/                # Header files (if any)
├── test/                   # Unit tests (if any)
└── docs/                   # Documentation
    ├── WIRING_GUIDE.md     # Hardware connections
    └── SOFTWARE_CONFIG.md  # This file
```

### platformio.ini Configuration

Complete configuration file with all required settings:

```ini
; PlatformIO Project Configuration File
;
; Build options: build flags, source filter
; Upload options: custom upload port, speed and extra flags
; Library options: dependencies, extra library storages
; Advanced options: extra scripting

[env:nodemcu-32s]
platform = espressif32
board = nodemcu-32s
framework = arduino

; Library dependencies
lib_deps = 
    sandeepmistry/CAN@^0.3.1        ; CAN bus communication
    bodmer/TFT_eSPI@^2.5.43         ; Optimized TFT display library

; Serial monitor configuration
monitor_speed = 115200
monitor_filters = esp32_exception_decoder

; Build flags for TFT_eSPI configuration
build_flags = 
    ; TFT_eSPI setup - loaded via build flags instead of User_Setup.h
    -D USER_SETUP_LOADED=1
    
    ; Display driver selection (choose one)
    -D ILI9488_DRIVER=1             ; For ILI9488 displays
    ; -D ILI9486_DRIVER=1           ; Uncomment for ILI9486 displays
    
    ; SPI pin configuration
    -D TFT_MISO=19                  ; SPI MISO pin
    -D TFT_MOSI=23                  ; SPI MOSI pin
    -D TFT_SCLK=18                  ; SPI Clock pin
    -D TFT_CS=15                    ; Chip Select pin
    -D TFT_DC=2                     ; Data/Command pin
    -D TFT_RST=4                    ; Reset pin
    
    ; Font loading options
    -D LOAD_GLCD=1                  ; Original Adafruit 8 pixel font
    -D LOAD_FONT2=1                 ; Small 16 pixel high font
    -D LOAD_FONT4=1                 ; Medium 26 pixel high font
    -D LOAD_GFXFF=1                 ; FreeFonts support
    
    ; SPI frequency (adjust if display is unstable)
    -D SPI_FREQUENCY=27000000       ; 27MHz (try 40000000 if stable)
    -D SPI_READ_FREQUENCY=20000000  ; 20MHz for reading operations
    
    ; Optional: Enable smooth fonts
    ; -D SMOOTH_FONT=1
    
    ; Optional: Enable touch screen support
    ; -D TOUCH_CS=21

; Upload configuration
upload_speed = 921600
upload_port = COM3                  ; Adjust to your COM port

; Debug configuration (optional)
debug_tool = esp-prog
debug_init_break = tbreak setup
```

## Library Configurations

### CAN Library (sandeepmistry/CAN)

The CAN library is configured in code with these key settings:

```cpp
// CAN bus speed - common automotive rates
#define CAN_SPEED 500E3  // 500 kbps (most common)
// Alternative speeds:
// #define CAN_SPEED 250E3  // 250 kbps
// #define CAN_SPEED 1000E3 // 1 Mbps

// CAN pins (must match hardware wiring)
#define CAN_TX_PIN 21    // ESP32 CAN TX to SN65HVD230 D pin
#define CAN_RX_PIN 22    // ESP32 CAN RX to SN65HVD230 R pin
```

### TFT_eSPI Library Configuration

#### Display Driver Selection
Choose the correct driver for your display:

**For ILI9488 displays:**
```ini
-D ILI9488_DRIVER=1
```

**For ILI9486 displays:**
```ini
-D ILI9486_DRIVER=1
```

#### Pin Configuration
Standard ESP32 SPI pins (recommended):
```ini
-D TFT_MISO=19   # SPI Master In Slave Out
-D TFT_MOSI=23   # SPI Master Out Slave In  
-D TFT_SCLK=18   # SPI Clock
-D TFT_CS=15     # Chip Select
-D TFT_DC=2      # Data/Command
-D TFT_RST=4     # Reset
```

#### Alternative Pin Configurations
If you need to use different pins:
```ini
; Custom pin configuration example
-D TFT_CS=5      ; Alternative CS pin
-D TFT_DC=17     ; Alternative DC pin
-D TFT_RST=16    ; Alternative Reset pin
```

#### SPI Frequency Tuning
```ini
; Conservative (most stable)
-D SPI_FREQUENCY=20000000    ; 20MHz

; Standard (recommended)
-D SPI_FREQUENCY=27000000    ; 27MHz

; Aggressive (if system is stable)
-D SPI_FREQUENCY=40000000    ; 40MHz
```

## Code Configuration Options

### OBD-II PID Configuration

Available PIDs in the current implementation:
```cpp
// Engine parameters
#define PID_ENGINE_RPM 0x0C          // Engine RPM
#define PID_VEHICLE_SPEED 0x0D       // Vehicle speed
#define PID_ENGINE_COOLANT_TEMP 0x05 // Coolant temperature
#define PID_THROTTLE_POSITION 0x11   // Throttle position
#define PID_ENGINE_LOAD 0x04         // Engine load
#define PID_INTAKE_TEMP 0x0F         // Intake air temperature
#define PID_MAF_FLOW 0x10            // Mass air flow
#define PID_FUEL_LEVEL 0x2F          // Fuel tank level
```

### Adding Custom PIDs

To add new OBD-II parameters:

1. **Define the PID:**
```cpp
#define PID_FUEL_PRESSURE 0x0A       // Fuel pressure
```

2. **Add to vehicle data structure:**
```cpp
struct VehicleData {
  // Existing parameters...
  float fuelPressure = 0;           // Add new parameter
  // ...
};
```

3. **Add processing in processOBDResponse():**
```cpp
case PID_FUEL_PRESSURE:
  if (length >= 1) {
    vehicle.fuelPressure = data[0] * 3;  // Formula depends on PID
  }
  break;
```

4. **Add to display in updateDisplay():**
```cpp
drawParameter("FUEL PSI", String((int)vehicle.fuelPressure), x, y, 2, TFT_ORANGE);
```

5. **Include in PID rotation:**
```cpp
uint8_t getCurrentPID(uint8_t index) {
  uint8_t pids[] = {
    PID_ENGINE_RPM, 
    PID_VEHICLE_SPEED, 
    PID_ENGINE_COOLANT_TEMP, 
    PID_THROTTLE_POSITION,
    PID_FUEL_PRESSURE  // Add new PID
  };
  return pids[index % (sizeof(pids)/sizeof(pids[0]))];
}
```

### Display Customization

#### Color Definitions
```cpp
// Standard colors (already defined)
#define TFT_BLACK    0x0000
#define TFT_BLUE     0x001F
#define TFT_RED      0xF800
#define TFT_GREEN    0x07E0
#define TFT_CYAN     0x07FF
#define TFT_MAGENTA  0xF81F
#define TFT_YELLOW   0xFFE0
#define TFT_WHITE    0xFFFF

// Custom colors (you can add these)
#define TFT_ORANGE   0xFD20
#define TFT_GRAY     0x8410
#define TFT_DARKGRAY 0x4208
#define TFT_NAVY     0x000F
#define TFT_PURPLE   0x780F
```

#### Warning Threshold Customization
```cpp
// RPM warning levels (modify these values as needed)
vehicle.rpm > 6000 ? TFT_RED :          // Over-rev warning
(vehicle.rpm > 4000 ? TFT_YELLOW :      // High RPM caution
TFT_GREEN)                              // Normal operation

// Temperature warning levels
vehicle.coolantTemp > 100 ? TFT_RED :    // Overheating
(vehicle.coolantTemp > 90 ? TFT_YELLOW : // Running hot
TFT_GREEN)                              // Normal temp

// Speed warning levels  
vehicle.speed > 120 ? TFT_RED :          // Excessive speed
(vehicle.speed > 80 ? TFT_YELLOW :       // Fast driving
TFT_GREEN)                              // Normal speed
```

### Timing Configuration

```cpp
// Update frequencies (milliseconds)
#define DISPLAY_UPDATE_INTERVAL 200   // Display refresh rate
#define OBD_REQUEST_INTERVAL 500      // How often to request new data
#define CAN_TIMEOUT 2000             // Connection timeout

// In loop():
static unsigned long lastDisplayUpdate = 0;
if (millis() - lastDisplayUpdate > DISPLAY_UPDATE_INTERVAL) {
  updateDisplay();
  lastDisplayUpdate = millis();
}
```

## Build Process

### Manual Build Steps

1. **Install PlatformIO:**
```bash
# Via pip
pip install platformio

# Via VS Code extension (recommended)
# Install "PlatformIO IDE" extension
```

2. **Build project:**
```bash
cd Rosalba.Next
pio run
```

3. **Upload to device:**
```bash
pio run --target upload
```

4. **Monitor serial output:**
```bash
pio device monitor
```

### Automated Build (CI/CD)

Example GitHub Actions workflow:
```yaml
name: PlatformIO Build

on: [push, pull_request]

jobs:
  build:
    runs-on: ubuntu-latest
    steps:
    - uses: actions/checkout@v3
    - name: Setup Python
      uses: actions/setup-python@v4
      with:
        python-version: '3.9'
    - name: Install PlatformIO
      run: pip install platformio
    - name: Build project
      run: pio run
```

## Troubleshooting Build Issues

### Common Compilation Errors

#### TFT_eSPI Configuration
```
Error: TFT_eSPI setup not found
Solution: Ensure build_flags include -D USER_SETUP_LOADED=1
```

#### Missing Libraries
```
Error: CAN.h not found
Solution: Check lib_deps in platformio.ini
```

#### Wrong Display Driver
```
Error: Display not working
Solution: Verify ILI9488_DRIVER vs ILI9486_DRIVER setting
```

### Library Version Conflicts
```ini
; Force specific versions if needed
lib_deps = 
    sandeepmistry/CAN@0.3.1         ; Specific version
    bodmer/TFT_eSPI@^2.5.43         ; Compatible version
```

### Memory Issues
```ini
; Increase available memory if needed
build_flags = 
    -D CORE_DEBUG_LEVEL=0           ; Reduce debug info
    -Os                             ; Optimize for size
```

---
**Last Updated**: October 15, 2025
**Revision**: 1.0