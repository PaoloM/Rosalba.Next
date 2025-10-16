# Hardware Wiring Guide

## Complete Wiring Diagram

### ESP32 NodeMCU-32S Pinout Reference
```
                     ESP32 NodeMCU-32S
                    ┌─────────────────┐
                    │                 │
             3V3 ───┤ 3V3         VIN ├─── 5V Input
             GND ───┤ GND         GND ├─── GND
             TX2 ───┤ TX2    GPIO 13  ├─── 
             RX2 ───┤ RX2    GPIO 12  ├─── 
            GPIO22 ─┤ 22     GPIO 14  ├─── 
            GPIO21 ─┤ 21     GPIO 27  ├─── 
            GPIO17 ─┤ 17     GPIO 26  ├─── 
            GPIO16 ─┤ 16     GPIO 25  ├─── 
             GPIO4 ─┤ 4      GPIO 33  ├─── 
             GPIO0 ─┤ 0      GPIO 32  ├─── 
             GPIO2 ─┤ 2      GPIO 35  ├─── 
            GPIO15 ─┤ 15     GPIO 34  ├─── 
             GPIO8 ─┤ 8          VN   ├─── 
             GPIO7 ─┤ 7          VP   ├─── 
             GPIO6 ─┤ 6      GPIO 39  ├─── 
             GPIO5 ─┤ 5      GPIO 36  ├─── 
            GPIO18 ─┤ 18     GPIO  3  ├─── RX0
            GPIO19 ─┤ 19     GPIO  1  ├─── TX0
            GPIO23 ─┤ 23         RST  ├─── RESET
                    │                 │
    Power Sense ────┤ GPIO35 (ADC)    │ ← Optional: VIN voltage monitoring
                    │                 │
                    └─────────────────┘
```

## 1. SN65HVD230 CAN Transceiver Wiring

### SN65HVD230 Module Pinout
```
    SN65HVD230 CAN Transceiver
    ┌─────────────────────────┐
    │  3V3  │  VCC            │
    │  GND  │  GND            │
    │  CTX  │  D (Data Input) │ ← Connect to ESP32 GPIO21
    │  CRX  │  R (Data Output)│ ← Connect to ESP32 GPIO22
    │ CANH  │  CAN High       │ ← Connect to OBD-II Pin 6
    │ CANL  │  CAN Low        │ ← Connect to OBD-II Pin 14
    └─────────────────────────┘
```

### Wiring Table
| SN65HVD230 Pin | ESP32 Pin | Wire Color | Notes |
|----------------|-----------|------------|-------|
| VCC | 3.3V | Red | Power supply |
| GND | GND | Black | Ground |
| D (CTX) | GPIO21 | Blue | CAN TX from ESP32 |
| R (CRX) | GPIO22 | Green | CAN RX to ESP32 |
| CANH | OBD-II Pin 6 | White/Blue | CAN High signal |
| CANL | OBD-II Pin 14 | White/Brown | CAN Low signal |

## 2. TFT Display SPI Connections

### SPI Interface Wiring (Your Display)
```
    ESP32 NodeMCU-32S          Your TFT Display
    ┌─────────────────┐        ┌─────────────────────────┐
    │                 │        │                         │
    │ 5V ─────────────┼────────┤ Pin 1,2: 5V Power       │
    │ GND ────────────┼────────┤ Pin 35,36: GND          │
    │                 │        │                         │
    │ GPIO23 (MOSI) ──┼────────┤ Pin 32: SPI_MOSI        │
    │ GPIO18 (SCK) ───┼────────┤ Pin 33: SPI_CLK         │
    │ GPIO19 (MISO) ──┼────────┤ Pin 31: SPI_MISO        │
    │ GPIO15 (CS) ────┼────────┤ Pin 21: LCD_CS          │
    │ GPIO2  (DC) ────┼────────┤ Pin 19: LCD_RS          │
    │ GPIO4  (RST) ───┼────────┤ Pin 22: LCD_RST         │
    │                 │        │                         │
    └─────────────────┘        └─────────────────────────┘
```

### Your Display's SPI Connections
| ESP32 Pin | Display Pin | Pin# | Function | Wire Color |
|-----------|-------------|------|----------|------------|
| 5V | 5V | 1,2 | Power | Red |
| GND | GND | 35,36 | Ground | Black |
| GPIO23 | SPI_MOSI | 32 | SPI Data Out | Blue |
| GPIO18 | SPI_CLK | 33 | SPI Clock | Green |
| GPIO19 | SPI_MISO | 31 | SPI Data In | White |
| GPIO15 | LCD_CS | 21 | Chip Select | Orange |
| GPIO2 | LCD_RS | 19 | Register Select (Data/Command) | Purple |
| GPIO4 | LCD_RST | 22 | Reset | Yellow |

### Key Points:
- **Pin 19 (LCD_RS)** is your Data/Command control - connect to ESP32 GPIO2
- **5V Power** - This display requires 5V, connect to ESP32 VIN pin
- **SPI Pins 31-33** - Use these for SPI communication, ignore parallel pins 3-18
- **Pin 26 (FLASH_CS)** - SD card chip select (connect to ESP32 GPIO5 for SD functionality)

## 3. SD Card Integration (Optional Enhancement)

### SD Card Socket Wiring
| ESP32 Pin | Display Pin | Pin# | Function | Wire Color |
|-----------|-------------|------|----------|------------|
| GPIO5 | FLASH_CS | 26 | SD Card Chip Select | Brown |
| GPIO23 | SPI_MOSI | 32 | SD Data (shared with display) | Blue |
| GPIO18 | SPI_CLK | 33 | SD Clock (shared with display) | Green |
| GPIO19 | SPI_MISO | 31 | SD Data (shared with display) | White |

### What You Can Do with SD Card:

#### 🏎️ **Data Logging & Analytics:**
- **Trip Recording**: Log RPM, boost, speed for entire drives
- **Performance Analysis**: Track 0-60 times, quarter-mile runs
- **Fault Code History**: Store OBD-II error codes with timestamps
- **Fuel Economy**: Log consumption patterns and driving efficiency

#### 📊 **Enhanced Graphics:**
- **Multiple Logos**: Store different Abarth logos (vintage, modern, custom)
- **Custom Themes**: Color schemes for day/night/sport modes
- **Icons & Graphics**: Weather icons, status indicators, warning symbols
- **Fonts**: Additional font files for different display styles

#### 🔧 **Configuration & Updates:**
- **Settings Storage**: User preferences, calibration data
- **Firmware Backup**: Store configuration backups
- **Data Export**: Export logs as CSV files for analysis
- **Remote Updates**: Load new features via SD card

#### 📈 **Advanced Features:**
- **Track Day Mode**: Detailed telemetry recording
- **Comparison Data**: Compare current vs. previous sessions
- **Maintenance Logs**: Track service intervals, part replacements
- **Diagnostic Reports**: Detailed system health reports

## 4. KY-040 Rotary Encoder (User Input)

### Encoder Wiring
| ESP32 Pin | Encoder Pin | Function | Wire Color |
|-----------|-------------|----------|------------|
| 3.3V | VCC | Power | Red |
| GND | GND | Ground | Black |
| GPIO12 | CLK | Clock Signal | Green |
| GPIO14 | DT | Data Signal | Blue |
| GPIO27 | SW | Button Switch | Yellow |

### Interactive Features with Encoder:
#### 🎛️ **Menu Navigation:**
- **Rotate**: Navigate between screens (Dashboard → Performance → Diagnostics → Data Logs → Settings)
- **Press**: Select current option or toggle display modes

#### 📊 **Display Control:**
- **Main Dashboard**: Switch between gauges, numbers, and graph views
- **Performance Screen**: Start/stop timing runs and track day logging
- **Settings Menu**: Adjust brightness, units, warning thresholds

#### ⚙️ **Real-Time Adjustments:**
- **Brightness**: Rotate to adjust display brightness (10-255)
- **Units**: Toggle between metric/imperial measurements
- **Warnings**: Set custom boost and temperature alert thresholds
- **Data Export**: Export driving sessions to SD card

## 3. Power Source Detection (Optional)

### Purpose
The system can automatically detect whether it's powered via USB or external supply and display this information on screen.

### Wiring for Power Detection
| Connection | ESP32 Pin | Wire Color | Notes |
|------------|-----------|------------|-------|
| VIN Monitor | GPIO35 (ADC) | Yellow | Optional voltage sensing |

### Detection Method
- **GPIO35** is connected internally to a voltage divider that monitors the VIN pin
- **USB Power**: Detected when voltage is ~5.0V
- **External Power**: Detected when voltage is different from 5V
- **3.3V Direct**: When powering directly to 3.3V pin

### Display Indicators
- **Power Icon**: Color-coded square indicator
  - 🟦 **Cyan**: USB powered (5V)
  - 🟨 **Yellow**: External power (non-5V)
  - 🟠 **Orange**: 3.3V direct or unknown
- **Status Text**: Shows "USB (5V)", "External (X.XV)", or "3.3V Direct"
- **Voltage Reading**: Actual measured voltage displayed

### No Additional Wiring Required
The NodeMCU-32S has built-in voltage monitoring capabilities, so no external components are needed for basic power source detection.

## 4. OBD-II Connector Pinout

### Standard OBD-II Port (J1962)
```
    OBD-II Diagnostic Port (Female)
    ┌─────────────────────────┐
    │  1   2   3   4   5   6  │
    │   7   8   9  10  11  12 │
    │    13  14  15  16       │
    └─────────────────────────┘
```

### Pin Functions
| Pin | Function | Color | Connection |
|-----|----------|-------|------------|
| 1 | - | - | Not used |
| 2 | J1850 Bus+ | - | Not used |
| 3 | - | - | Not used |
| 4 | Chassis Ground | Black | Connect to GND |
| 5 | Signal Ground | Black | Connect to GND |
| 6 | CAN High | White/Blue | To SN65HVD230 CANH |
| 7 | K-Line | - | Not used |
| 8 | - | - | Not used |
| 9 | - | - | Not used |
| 10 | J1850 Bus- | - | Not used |
| 11 | - | - | Not used |
| 12 | - | - | Not used |
| 13 | - | - | Not used |
| 14 | CAN Low | White/Brown | To SN65HVD230 CANL |
| 15 | L-Line | - | Not used |
| 16 | Battery + | Red | +12V (optional power) |

## 4. Power Supply Considerations

### Power Options
1. **USB Power**: Power ESP32 via USB (5V → 3.3V regulator)
2. **External 3.3V**: Direct 3.3V supply to ESP32 and modules
3. **Vehicle Power**: 12V from OBD-II pin 16 with voltage regulator

### Power Requirements
- **ESP32**: 3.3V, ~500mA peak
- **SN65HVD230**: 3.3V, ~10mA
- **ILI9486/ILI9488**: 3.3V or 5V, ~100-200mA
- **Total**: ~600-700mA at 3.3V

## 5. Assembly Tips

### Recommended Assembly Order
1. **Breadboard/PCB Layout**: Plan component placement
2. **Power Connections**: Wire all VCC and GND first
3. **CAN Interface**: Wire SN65HVD230 to ESP32
4. **Display Interface**: Wire TFT to ESP32 SPI pins
5. **OBD-II Cable**: Prepare connector cable
6. **Testing**: Test each subsystem before final assembly

### Wire Management
- Use different colors for power, ground, and signals
- Keep CAN wires (CANH/CANL) twisted and short
- Separate power wires from signal wires
- Use proper gauge wire (22-26 AWG for signals)

### Enclosure Considerations
- Provide ventilation for ESP32 (can get warm)
- Mount display for easy viewing
- Protect connections from vibration
- Consider automotive environment (temperature, humidity)

## 6. Testing Procedures

### Step-by-Step Testing
1. **Power Test**: Verify 3.3V at all modules
2. **Display Test**: Upload simple TFT test code
3. **CAN Interface**: Test with CAN analyzer or second device
4. **OBD-II Connection**: Test with vehicle (engine running)
5. **Full System**: Upload complete Rosalba.Next code

### Multimeter Checks
- **3.3V at ESP32**: Between 3.3V pin and GND
- **3.3V at SN65HVD230**: Between VCC and GND  
- **3.3V at Display**: Between VCC and GND
- **Continuity**: All signal connections
- **CAN Differential**: ~2.5V between CANH and CANL (idle)

## 7. Common Wiring Issues

### Troubleshooting Checklist
- [ ] All ground connections secure
- [ ] Correct 3.3V supply voltage
- [ ] SPI pins correctly connected
- [ ] CAN transceiver orientation correct
- [ ] OBD-II pins 6 and 14 connected
- [ ] No short circuits between pins
- [ ] Proper wire gauge for current requirements
- [ ] Secure connections (no loose wires)

---
**Last Updated**: October 15, 2025
**Revision**: 1.0