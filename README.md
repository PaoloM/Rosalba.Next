# Rosalba.Next - Fiat 500 Abarth CAN Monitor

A minimal ESP32-S3 based CAN bus monitor specialized for 2015+ Fiat 500 Abarth turbo performance monitoring.

## Hardware

- **Waveshare ESP32-S3 3.5" Capacitive Touch Display**
- **SN65HVD230 CAN Transceiver** (for OBD-II connection)
- **External 5V power supply** (recommended for CAN transceiver)

## Features

- **Real-time CAN bus monitoring** for Fiat 500 Abarth
- **Turbo-specific parameters:**
  - Engine RPM
  - Boost Pressure (MAP + Hydraulic Sensor)
  - Throttle Position
  - Intake Air Temperature
  - Coolant Temperature
  - Sport Mode Status
- **Serial output** for data monitoring and debugging
- **Fiat-specific CAN message decoding**

## CAN Bus Connection

Connect SN65HVD230 CAN transceiver to vehicle OBD-II port:
- **CAN_RX** → ESP32-S3 GPIO (configured in obd.cpp)
- **CAN_TX** → ESP32-S3 GPIO (configured in obd.cpp)
- **5V** → External power supply
- **GND** → Common ground
- **CANH/CANL** → OBD-II pins 6/14

## Building

```bash
# Install PlatformIO
pip install platformio

# Clone repository
git clone https://github.com/PaoloM/Rosalba.Next.git
cd Rosalba.Next

# Build and upload
pio run -t upload

# Monitor serial output
pio device monitor
```

## Monitored Parameters

### Standard OBD-II PIDs
- `0x0C` - Engine RPM
- `0x0B` - Manifold Absolute Pressure (boost)
- `0x11` - Throttle Position
- `0x0F` - Intake Air Temperature
- `0x05` - Coolant Temperature

### Fiat-Specific PIDs
- `0x22F189` - Hydraulic Boost Sensor (direct PSI reading)
- `0x22F187` - Wastegate Position
- `0x22F18A` - Charge Air Pressure

### Direct CAN Messages
- `0x316` - Primary engine data (RPM, throttle)
- `0x3D7` - Turbo/boost control data
- `0x4A1` - Abarth performance parameters
- `0x329` - Secondary engine data

## Serial Output Example

```
🏁 RPM: 3250 rpm
💨 Boost: 185.3 kPa (12.4 PSI hydraulic)
🎛️  Throttle: 78%
🌡️  Intake: 45°C
💧 Coolant: 92°C
🏎️  Sport Mode: ON
⏱️  Last Update: 0.1s ago
```

## Files Structure

- `src/main.cpp` - Main application (CAN-only)
- `src/obd.cpp` - CAN bus communication and OBD-II protocol
- `include/obd.h` - Vehicle data structures and function declarations
- `include/config.h` - Configuration parameters
- `platformio.ini` - Build configuration for ESP32-S3

## License

MIT License - see LICENSE file for details.

## Notes

This is a **minimal CAN-only version** focused purely on data acquisition and serial monitoring. Perfect for:
- Data logging applications
- Custom dashboard development
- Performance tuning and analysis
- Integration with other systems

The Waveshare ESP32-S3 display can be used for future GUI development, but this version focuses on reliable CAN communication first.