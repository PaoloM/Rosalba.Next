# Build and Deployment Guide

## Quick Start Checklist

### Hardware Setup ✅
- [ ] ESP32 NodeMCU-32S board ready
- [ ] SN65HVD230 CAN transceiver wired correctly
- [ ] ILI9486/ILI9488 display connected via SPI
- [ ] OBD-II connector prepared
- [ ] Power supply available (USB or 12V→3.3V)

### Software Setup ✅
- [ ] PlatformIO installed (VS Code extension recommended)
- [ ] Project cloned from repository
- [ ] platformio.ini configured for your display type
- [ ] Build flags set correctly

### Vehicle Connection ✅  
- [ ] Vehicle supports OBD-II (post-1996 cars)
- [ ] Engine running (some cars require this)
- [ ] OBD-II port accessible
- [ ] No other diagnostic tools connected

## Step-by-Step Build Process

### 1. Environment Setup

#### Install VS Code + PlatformIO
```bash
# 1. Download and install VS Code
# 2. Open VS Code
# 3. Go to Extensions (Ctrl+Shift+X)
# 4. Search for "PlatformIO IDE"
# 5. Install the extension
# 6. Restart VS Code
```

#### Clone Repository
```bash
git clone https://github.com/PaoloM/Rosalba.Next.git
cd Rosalba.Next
```

### 2. Hardware Verification

#### Power Test
```bash
# Connect ESP32 via USB
# Verify 3.3V output with multimeter
# Check all ground connections
```

#### Display Test (Optional)
Create simple test to verify display:
```cpp
#include <TFT_eSPI.h>
TFT_eSPI tft = TFT_eSPI();

void setup() {
  tft.init();
  tft.setRotation(3);
  tft.fillScreen(TFT_RED);
  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(2);
  tft.drawString("Display Test", 50, 50);
}
void loop() { }
```

### 3. Configuration

#### Choose Display Driver
Edit `platformio.ini`:
```ini
# For ILI9488
-D ILI9488_DRIVER=1

# OR for ILI9486 (comment out ILI9488, uncomment this)
# -D ILI9486_DRIVER=1
```

#### Verify Pin Assignments
Confirm in `platformio.ini`:
```ini
-D TFT_CS=15     # Chip Select
-D TFT_DC=2      # Data/Command  
-D TFT_RST=4     # Reset
-D TFT_MOSI=23   # SPI MOSI
-D TFT_SCLK=18   # SPI Clock
-D TFT_MISO=19   # SPI MISO
```

### 4. Build Process

#### Open Project in PlatformIO
```bash
# In VS Code:
# File → Open Folder → Select Rosalba.Next directory
# PlatformIO should auto-detect the project
```

#### Build Project
```bash
# Method 1: VS Code Interface
# Click PlatformIO icon → Project Tasks → nodemcu-32s → Build

# Method 2: Command line
cd Rosalba.Next
pio run
```

#### Expected Build Output
```
Processing nodemcu-32s (platform: espressif32; board: nodemcu-32s; framework: arduino)
--------------------------------------------------------------------------------
Library Manager: Installing sandeepmistry/CAN@^0.3.1
Library Manager: Installing bodmer/TFT_eSPI@^2.5.43
Compiling .pio/build/nodemcu-32s/src/main.cpp.o
Linking .pio/build/nodemcu-32s/firmware.elf
Building .pio/build/nodemcu-32s/firmware.bin
RAM:   [==        ]  18.2% (used 59532 bytes from 327680 bytes)
Flash: [======    ]  57.8% (used 757093 bytes from 1310720 bytes)
======================== [SUCCESS] Took 45.67 seconds ========================
```

### 5. Upload and Testing

#### Upload to Device
```bash
# Method 1: VS Code Interface  
# PlatformIO → Project Tasks → nodemcu-32s → Upload

# Method 2: Command line
pio run --target upload

# Method 3: Specify port
pio run --target upload --upload-port COM3  # Windows
pio run --target upload --upload-port /dev/ttyUSB0  # Linux
```

#### Monitor Serial Output
```bash
# Method 1: VS Code Interface
# PlatformIO → Project Tasks → nodemcu-32s → Monitor

# Method 2: Command line
pio device monitor --baud 115200

# Method 3: Specify port
pio device monitor --port COM3 --baud 115200
```

#### Expected Startup Output
```
Rosalba Vehicle Monitor - Starting...
ILI9486/ILI9488 TFT Display initialized
Resolution: 480x320 (landscape)
Note: Pin configuration handled by TFT_eSPI User_Setup.h
Default pins: MOSI=23, MISO=19, SCK=18, CS=15, DC=2, RST=4
CAN bus initialized successfully!
Bus speed: 500 kbps
Monitoring vehicle parameters...
```

### 6. Vehicle Testing

#### Connect to Vehicle
1. **Locate OBD-II port** (usually under dashboard, driver side)
2. **Plug in connector** firmly
3. **Turn on ignition** or start engine
4. **Watch for CAN activity** in serial monitor

#### Expected CAN Output
```
0x7E8 [8] 03 41 0C 1A 40 00 00 00
0x7E8 [8] 03 41 0D 50 00 00 00 00  
0x7E8 [8] 03 41 05 5F 00 00 00 00
RPM: 1668
Speed: 80 km/h
Temperature: 95°C
```

### 7. Troubleshooting Build Issues

#### Common Build Errors

**Error: Library not found**
```bash
Error: CAN.h: No such file or directory
Solution: 
pio lib install "sandeepmistry/CAN"
```

**Error: TFT_eSPI configuration**
```bash
Error: User_Setup file not found
Solution: Verify build_flags in platformio.ini include -D USER_SETUP_LOADED=1
```

**Error: Wrong display driver**
```bash
# Display shows garbage or nothing
Solution: Try switching between ILI9488_DRIVER and ILI9486_DRIVER
```

#### Memory Issues
```bash
# If RAM usage too high:
build_flags = 
    -Os                    # Optimize for size
    -D CORE_DEBUG_LEVEL=0  # Reduce debug output
```

#### Upload Issues
```bash
# If upload fails:
# 1. Check USB cable and port
# 2. Press and hold BOOT button during upload
# 3. Try lower upload speed:
upload_speed = 460800
```

### 8. Performance Optimization

#### Display Performance
```ini
# Increase SPI frequency if stable:
-D SPI_FREQUENCY=40000000

# Enable DMA (if supported):
-D SPI_HAS_TRANSACTION=1
```

#### CAN Bus Optimization
```cpp
// Reduce request frequency if overwhelming:
#define OBD_REQUEST_INTERVAL 1000  // 1 second instead of 500ms

// Filter specific CAN IDs:
// Add filtering code to only process OBD-II responses
```

### 9. Version Management

#### Git Workflow
```bash
# Create feature branch
git checkout -b feature/new-parameter

# Make changes
git add .
git commit -m "Add fuel pressure monitoring"

# Push to repository  
git push origin feature/new-parameter

# Create pull request on GitHub
```

#### Release Tagging
```bash
# Tag stable releases
git tag -a v1.0.0 -m "Initial release"
git push origin v1.0.0
```

### 10. Deployment Options

#### Standalone Device
- Flash firmware to ESP32
- Package in automotive enclosure
- Connect permanently to vehicle

#### Development/Testing
- Keep USB connected for monitoring
- Use breadboard for easy modifications
- Serial monitor for debugging

#### Production Deployment
- Use custom PCB for reliability
- Add automotive-grade connectors
- Consider over-the-air (OTA) updates

## Build Automation

### GitHub Actions CI/CD
```yaml
# .github/workflows/build.yml
name: Build Rosalba.Next

on: [push, pull_request]

jobs:
  build:
    runs-on: ubuntu-latest
    steps:
    - uses: actions/checkout@v3
    - name: Setup PlatformIO
      run: |
        pip install platformio
        pio platform install espressif32
    - name: Build firmware
      run: pio run
    - name: Archive firmware
      uses: actions/upload-artifact@v3
      with:
        name: firmware
        path: .pio/build/nodemcu-32s/firmware.bin
```

### Local Build Script
```bash
#!/bin/bash
# build.sh - Automated local build script

echo "Building Rosalba.Next..."

# Clean previous build
pio run --target clean

# Build project
if pio run; then
    echo "✅ Build successful!"
    echo "📁 Firmware: .pio/build/nodemcu-32s/firmware.bin"
else
    echo "❌ Build failed!"
    exit 1
fi

# Optional: Upload if device connected
read -p "Upload to device? (y/n): " -n 1 -r
echo
if [[ $REPLY =~ ^[Yy]$ ]]; then
    pio run --target upload
fi
```

---
**Last Updated**: October 15, 2025
**Revision**: 1.0