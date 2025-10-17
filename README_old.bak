# Rosalba.Next - Vehicle CAN Bus Monitor

A real-time vehicle parameter monitoring system using ESP32 NodeMCU-32S with CAN bus interface and TFT display.

## 🚗 Overview

Rosalba.Next is an embedded system that reads vehicle data from the CAN bus via OBD-II interface and displays real-time parameters on a color TFT screen. Perfect for automotive diagnostics, performance monitoring, and custom dashboard applications.

## 📋 Features

- **Real-time OBD-II parameter monitoring**
- **Color-coded dashboard display**
- **500 kbps CAN bus communication**
- **Professional automotive-style interface**
- **Configurable warning thresholds**
- **Power source detection** (USB vs External power)
- **Serial logging for debugging**

### Monitored Parameters
- Engine RPM (with over-rev warnings)
- Vehicle Speed (km/h)
- Engine Coolant Temperature (°C)
- Throttle Position (%)
- Engine Load (%)
- Intake Air Temperature (°C)

## 🔧 Hardware Requirements

### Main Components
- **ESP32 NodeMCU-32S** - Main microcontroller
- **SN65HVD230 CAN Transceiver** - CAN bus interface
- **ILI9486/ILI9488 TFT Display** - 480x320 color display
- **OBD-II connector** - Vehicle interface

### Hardware Connections

#### CAN Interface (SN65HVD230)
```
SN65HVD230    →    ESP32 NodeMCU-32S
VCC           →    3.3V
GND           →    GND
D (TX)        →    GPIO21 (CAN TX)
R (RX)        →    GPIO22 (CAN RX)
CANH/CANL     →    Vehicle OBD-II Port
```

#### TFT Display (ILI9486/ILI9488 SPI)
```
Display       →    ESP32 NodeMCU-32S
VCC           →    3.3V (or 5V depending on module)
GND           →    GND
CS            →    GPIO15
DC            →    GPIO2
RST           →    GPIO4
MOSI          →    GPIO23
SCK           →    GPIO18
MISO          →    GPIO19 (optional, for touch)
```

## ⚙️ Software Setup

### Development Environment
- **PlatformIO** (recommended) or Arduino IDE
- **ESP32 Arduino Framework**

### Dependencies
- `sandeepmistry/CAN` - CAN bus communication
- `bodmer/TFT_eSPI` - Optimized TFT display library

### Build Configuration

The project uses PlatformIO with the following configuration in `platformio.ini`:

```ini
[env:nodemcu-32s]
platform = espressif32
board = nodemcu-32s
framework = arduino
lib_deps = 
    sandeepmistry/CAN@^0.3.1
    bodmer/TFT_eSPI@^2.5.43
monitor_speed = 115200
build_flags = 
    -D USER_SETUP_LOADED=1
    -D ILI9488_DRIVER=1
    -D TFT_MISO=19
    -D TFT_MOSI=23  
    -D TFT_SCLK=18
    -D TFT_CS=15
    -D TFT_DC=2
    -D TFT_RST=4
    -D LOAD_GLCD=1
    -D LOAD_FONT2=1
    -D LOAD_FONT4=1
    -D SPI_FREQUENCY=27000000
```

### Display Configuration Notes
- **For ILI9486**: Change `-D ILI9488_DRIVER=1` to `-D ILI9486_DRIVER=1`
- **SPI Frequency**: 27MHz (can try 40MHz if stable)
- **Resolution**: 480x320 in landscape mode

## 🚀 Installation

1. **Clone the repository**
   ```bash
   git clone https://github.com/PaoloM/Rosalba.Next.git
   cd Rosalba.Next
   ```

2. **Open in PlatformIO**
   - Install PlatformIO extension in VS Code
   - Open project folder
   - PlatformIO will auto-install dependencies

3. **Build and Upload**
   ```bash
   pio run --target upload
   ```

4. **Monitor Serial Output**
   ```bash
   pio device monitor
   ```

## 📊 Display Interface

### Layout Description
- **Header**: System title with connection status indicator
- **RPM**: Large display with color-coded warnings (Green→Yellow→Red)
- **Speed**: Vehicle speed in km/h with speed-based coloring
- **Temperature**: Coolant temperature with overheating warnings
- **Throttle**: Current throttle position with progress bar
- **Engine Load**: Current load percentage with progress bar
- **Intake Temp**: Air intake temperature
- **Status Bar**: CAN bus status and connection indicator

### Color Coding
- **🟢 Green**: Normal operation
- **🟡 Yellow**: Caution/elevated values  
- **🔴 Red**: Warning/dangerous values
- **🔵 Blue/Cyan**: Information/secondary parameters

## 🔍 Troubleshooting

### Common Issues

#### CAN Bus Not Connecting
- Check SN65HVD230 wiring (GPIO21/22)
- Verify 3.3V power supply
- Ensure vehicle is running (some cars require engine on)
- Check OBD-II connector pinout

#### Display Not Working
- Verify SPI connections (GPIO15,2,4,23,18,19)
- Check display power (3.3V or 5V depending on module)
- Confirm correct driver (ILI9486 vs ILI9488)
- Try lower SPI frequency if unstable

#### No OBD-II Data
- Some vehicles require engine running
- Check if vehicle supports standard OBD-II PIDs
- Verify CAN bus speed (some use 250kbps instead of 500kbps)

### Serial Debug Output
Monitor at 115200 baud for:
- Raw CAN messages
- OBD-II responses
- System status
- Error messages

## 🛠️ Customization

### Adding New Parameters
1. Add PID definition in `main.cpp`
2. Update `processOBDResponse()` function
3. Modify display layout in `updateDisplay()`
4. Add to PID rotation in `getCurrentPID()`

### Changing Warning Thresholds
Edit the color conditions in `updateDisplay()`:
```cpp
vehicle.rpm > 6000 ? TFT_RED : (vehicle.rpm > 4000 ? TFT_YELLOW : TFT_GREEN)
```

### Display Layout Modifications
- Adjust coordinates in `updateDisplay()` function
- Modify `drawParameter()` for different layouts
- Add new UI elements using TFT_eSPI functions

## 📝 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## 🤝 Contributing

1. Fork the repository
2. Create your feature branch (`git checkout -b feature/AmazingFeature`)
3. Commit your changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request

## 📚 References

- [ESP32 CAN Documentation](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/peripherals/twai.html)
- [OBD-II PID List](https://en.wikipedia.org/wiki/OBD-II_PIDs)
- [TFT_eSPI Library Documentation](https://github.com/Bodmer/TFT_eSPI)
- [SN65HVD230 Datasheet](https://www.ti.com/lit/ds/symlink/sn65hvd230.pdf)

---

**Project Status**: Active Development  
**Last Updated**: October 15, 2025  
**Version**: 1.0.0
