# Rosalba.Next - Fiat 500 Abarth CAN Monitor

A minimal ESP32 based CAN bus monitor specialized for 2015+ Fiat 500 Abarth turbo performance monitoring.

## Hardware

- **Freenove FNK0103S ESP32 Development Board**
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

### Freenove FNK0103S to SN65HVD230 CAN Transceiver Wiring:
- **GPIO1 (TXD0)** → SN65HVD230 **D pin** (CAN TX)
- **GPIO3 (RXD0)** → SN65HVD230 **R pin** (CAN RX)
- **3.3V** → SN65HVD230 **VCC**
- **GND** → SN65HVD230 **GND**

### SN65HVD230 to Vehicle OBD-II Connection:
- **CANH** → OBD-II **Pin 6** (CAN High)
- **CANL** → OBD-II **Pin 14** (CAN Low)
- **GND** → OBD-II **Pin 4/5** (Vehicle Ground)

### USB Communication:
- **USB Cable** → PC for Serial Monitor debugging (115200 baud)
- Uses onboard USB-to-serial chip (no conflict with CAN pins)

### RGB LED (Optional Status Indicator):
- **GPIO16** → Blue LED (or repurpose for other functions)
- **GPIO17** → Green LED  
- **GPIO22** → Red LED

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

## Software Configuration

### CAN Library
- **sandeepmistry/CAN** v0.3.1 (compatible with ESP32)
- **CAN Speed**: 500 kbps (OBD-II standard)
- **Pin Configuration**: GPIO1 (TX), GPIO3 (RX)

### Build Environment
- **Board**: `esp32dev` (compatible with Freenove FNK0103S)
- **Framework**: Arduino
- **Monitor Speed**: 115200 baud

## Files Structure

- `src/main.cpp` - Main application
- `include/config.h` - Hardware pin configuration and timing parameters
- `platformio.ini` - Build configuration for ESP32 (Freenove FNK0103S)

## License

MIT License - see LICENSE file for details.

## Important Notes


## Version Notes
