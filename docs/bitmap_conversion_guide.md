# Creating Bitmap Images for TFT Display

## Method 1: Convert Image to XBM Format (Recommended)

### Step 1: Prepare Your Image
1. Open your Abarth logo image in GIMP or similar image editor
2. Resize to desired size (e.g., 64x48 or 100x80 pixels)
3. Convert to grayscale: Image → Mode → Grayscale
4. Adjust threshold: Colors → Threshold (make it black and white)

### Step 2: Export as XBM
1. In GIMP: File → Export As
2. Choose filename with `.xbm` extension (e.g., `abarth_logo.xbm`)
3. Export with default settings

### Step 3: Convert XBM to Arduino Format
The XBM file will look like this:
```c
#define abarth_logo_width 64
#define abarth_logo_height 48
static unsigned char abarth_logo_bits[] = {
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   // ... more data
};
```

### Step 4: Use in Arduino Code
```cpp
// In abarth_logo.h
const unsigned char abarth_logo_mono[] PROGMEM = {
   // Copy the array data from XBM file here
};

// In display.cpp  
tft.drawXBitmap(x, y, abarth_logo_mono, width, height, color);
```

## Method 2: RGB565 Color Bitmap

### Step 1: Prepare Color Image
1. Resize image to desired dimensions
2. Keep in RGB color mode

### Step 2: Convert to RGB565 Array
Use online converter or Python script:
```python
from PIL import Image
import numpy as np

def image_to_rgb565_array(image_path, output_file):
    img = Image.open(image_path)
    img = img.convert('RGB')
    width, height = img.size
    
    # Convert to RGB565 format
    pixels = []
    for y in range(height):
        for x in range(width):
            r, g, b = img.getpixel((x, y))
            # Convert 8-8-8 RGB to 5-6-5 RGB
            r5 = (r >> 3) & 0x1F
            g6 = (g >> 2) & 0x3F  
            b5 = (b >> 3) & 0x1F
            rgb565 = (r5 << 11) | (g6 << 5) | b5
            pixels.append(f"0x{rgb565:04X}")
    
    # Write C array
    with open(output_file, 'w') as f:
        f.write(f"const uint16_t abarth_logo[{len(pixels)}] PROGMEM = {{\n")
        for i, pixel in enumerate(pixels):
            if i % 10 == 0:
                f.write("\n  ")
            f.write(f"{pixel},")
        f.write("\n};\n")

# Usage
image_to_rgb565_array("abarth_logo.png", "abarth_rgb565.h")
```

### Step 3: Use RGB565 Bitmap
```cpp
// In display.cpp
tft.pushImage(x, y, width, height, abarth_logo);
```

## Method 3: Online Tools

### Recommended Tools:
1. **LCD Image Converter**: https://www.riuson.com/lcd-image-converter/
   - Supports many formats including RGB565
   - Can generate C arrays directly
   - Preview functionality

2. **Image2cpp**: https://javl.github.io/image2cpp/
   - Browser-based converter
   - Supports monochrome and color formats
   - Arduino-ready output

### Settings for Image2cpp:
- Canvas size: 64x48 (or your preferred size)
- Background color: Black
- Invert colors: Check if needed
- Output format: Arduino code, Horizontal 1-bit per pixel

## Implementation in Your Code

### Current Implementation Status:
- ✅ Header file created (`abarth_logo.h`)
- ✅ Include added to `display.cpp`
- ✅ Function declarations added to `display.h`
- ✅ Bitmap display functions implemented

### Next Steps:
1. Replace the placeholder bitmap data with your actual logo
2. Adjust dimensions if needed
3. Choose between monochrome (XBM) or color (RGB565) format
4. Test on hardware

### Color Options:
- **Monochrome**: Smaller memory usage, faster display
- **RGB565**: Full color, larger memory usage
- **Recommended**: Start with monochrome for testing

## Memory Considerations:
- **64x48 monochrome**: 384 bytes
- **64x48 RGB565**: 6,144 bytes  
- **100x80 monochrome**: 1,000 bytes
- **100x80 RGB565**: 16,000 bytes

Choose size based on available ESP32 memory and visual requirements.