# Display Troubleshooting Guide - White Screen Issues

## Common Causes of White Display

A fully white display typically indicates one of these issues:

### 1. **Wiring Problems (Most Common)**
- **DC/RS Pin Connection**: Pin 19 (LCD_RS) must connect to GPIO2
- **Reset Pin**: Pin 22 (LCD_RST) must connect to GPIO4  
- **Power Issues**: Display needs 5V on pins 1,2 (not 3.3V)
- **Ground**: Pins 35,36 must connect to ESP32 GND
- **SPI Connections**: Check MOSI(23), MISO(19), SCK(18), CS(15)

### 2. **Display Driver Mismatch**
- Your display might be ILI9486 instead of ILI9488
- Driver setting in `User_Setup.h` needs to match your display

### 3. **Power Supply Issues**
- Display requires stable 5V supply
- ESP32 VIN pin might not provide enough current
- Use external 5V supply if needed

### 4. **SPI Communication Failure**
- Wrong pin assignments
- Loose connections
- Interference on SPI lines

## Diagnostic Steps

### Step 1: Verify Wiring
Check these connections against your display:

| ESP32 Pin | Display Pin | Function | Required |
|-----------|-------------|----------|----------|
| 5V (VIN) | Pin 1,2 | Power | ✓ Critical |
| GND | Pin 35,36 | Ground | ✓ Critical |
| GPIO2 | Pin 19 | DC/RS | ✓ Critical |
| GPIO4 | Pin 22 | Reset | ✓ Critical |
| GPIO15 | Pin 21 | CS | ✓ Critical |
| GPIO23 | Pin 32 | MOSI | ✓ Critical |
| GPIO18 | Pin 33 | SCK | ✓ Critical |
| GPIO19 | Pin 31 | MISO | Optional |

### Step 2: Test Display Driver Type
Your display might be ILI9486 instead of ILI9488. Try both settings:

In `lib/TFT_eSPI_Setup/User_Setup.h`:
```cpp
// Try option 1 first:
#define ILI9488_DRIVER

// If that doesn't work, try option 2:
// #define ILI9488_DRIVER
// #define ILI9486_DRIVER
```

### Step 3: Use Diagnostic Code
Upload the diagnostic sketch to test basic display functionality.

### Step 4: Check Power Supply
- Measure voltage at display pins 1,2 (should be close to 5V)
- Check current draw (display can need 200-300mA)
- Try external 5V power supply if ESP32 VIN insufficient

### Step 5: Test with Minimal Code
Start with basic TFT_eSPI examples before running full application.

## Quick Fixes to Try

1. **Double-check DC pin**: Most common issue is wrong DC/RS connection
2. **Try different driver**: Switch between ILI9486 and ILI9488
3. **Lower SPI speed**: Reduce from 27MHz to 20MHz in User_Setup.h
4. **Add delays**: Some displays need longer initialization delays
5. **Check orientation**: Try different rotation values (0,1,2,3)

## Hardware Verification Commands

Use these commands to test basic connectivity:

```cpp
// Test backlight (should make screen bright white)
pinMode(TFT_RST, OUTPUT);
digitalWrite(TFT_RST, HIGH);

// Test basic SPI communication
tft.init();
tft.fillScreen(TFT_RED);  // Should show red
delay(1000);
tft.fillScreen(TFT_GREEN);  // Should show green  
delay(1000);
tft.fillScreen(TFT_BLUE);   // Should show blue
```

## Next Steps

If display remains white after checking all above:
1. Test display with known-good Arduino code
2. Try different ESP32 board
3. Verify display isn't damaged
4. Check for counterfeit display chips (common issue)