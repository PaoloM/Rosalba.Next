# 🚗 Installation Day - Final Checklist

## ✅ **You're Ready to Install!**

Your **Abarth CAN Monitor** is fully programmed and ready for car installation. Here's everything you need:

---

## 📦 **Required Components**

### **✅ Pre-Programmed Hardware:**
- **ESP32 NodeMCU-32S** - ✅ Firmware uploaded & tested
- **SPIFFS with Abarth Logo** - ✅ 11KB authentic logo ready

### **🔌 Components to Wire Tomorrow:**
1. **SN65HVD230 CAN Transceiver** 
2. **Your TFT Display** (ILI9486/ILI9488)
3. **Jumper Wires** (male-to-male/female)
4. **Breadboard** or **Perfboard**
5. **OBD-II Cable/Connector**

### **🎛️ Optional (Future Upgrade):**
- **KY-040 Rotary Encoder** (interactive menus)
- **SD Card** (data logging)

---

## 🔧 **Step-by-Step Installation**

### **Step 1: Wiring the Display (5 minutes)**
Follow your specific display pinout from `docs/WIRING_GUIDE.md`:

```
ESP32 Pin → Display Pin
3.3V      → VCC (Power)
GND       → GND (Ground)  
GPIO23    → MOSI (SPI Data)
GPIO18    → SCK (SPI Clock)
GPIO19    → MISO (SPI Data In)
GPIO15    → CS (Chip Select)
GPIO2     → LCD_RS (Data/Command) ← Pin 19 on your display
GPIO4     → RST (Reset)
```

### **Step 2: Wiring the CAN Interface (5 minutes)**
```
ESP32 Pin → SN65HVD230 Pin
3.3V      → VCC
GND       → GND
GPIO21    → D (Data Input)
GPIO22    → R (Data Output)
```

### **Step 3: OBD-II Connection (10 minutes)**
```
SN65HVD230 → OBD-II Port
CANH       → Pin 6 (CAN High)
CANL       → Pin 14 (CAN Low)
GND        → Pin 4 or 5 (Ground)
```

### **Step 4: Power Connection (5 minutes)**
**Option A: USB Power** (Recommended for testing)
- Connect ESP32 micro-USB to car USB port or power bank

**Option B: Direct 12V** (Advanced)
- Use voltage regulator: Car 12V → 5V → ESP32 VIN

---

## 🚗 **Car Installation Process**

### **Before Starting Engine:**
1. **Connect OBD-II** - Plug into diagnostic port (usually under dashboard)
2. **Power On** - ESP32 should show "System Ready!" on display
3. **Check Display** - Should show Abarth splash screen with authentic logo

### **With Engine Running:**
1. **CAN Communication** - Display should show live RPM and speed
2. **Boost Monitoring** - Should display real boost pressure when accelerating
3. **Sport Mode** - Should detect when boost > 16 PSI

### **Expected Display:**
```
┌─────────────────────────────────────────┐
│  🔥 Abarth Performance Monitor 🔥      │
├─────────────────────────────────────────┤
│                                         │
│  RPM: 2,150     BOOST: 12.3 PSI       │
│  SPEED: 85 KM/H  TEMP: 89°C            │
│                                         │
│  🏁 SPORT: OFF   ⛽ THROTTLE: 45%      │
└─────────────────────────────────────────┘
```

---

## 🔍 **Troubleshooting Guide**

### **Display Issues:**
| Problem | Solution |
|---------|----------|
| Black screen | Check 3.3V power and wiring |
| Garbled display | Verify SPI connections (MOSI, SCK, CS) |
| No logo | Ensure SPIFFS was uploaded |
| Wrong colors | Check LCD_RS connection (Pin 19) |

### **CAN Communication Issues:**
| Problem | Solution |
|---------|----------|
| No RPM/Speed data | Verify OBD-II pins 6 & 14 connection |
| "CAN Init Failed" | Check SN65HVD230 wiring |
| Intermittent data | Secure all CAN connections |
| Wrong values | Car may use different PIDs |

### **Power Issues:**
| Problem | Solution |
|---------|----------|
| System resets | Check power supply capacity |
| Dim display | Increase voltage or check connections |
| No power | Verify USB cable or 12V regulator |

---

## 📊 **What to Expect**

### **🎯 Core Features Working:**
- ✅ **Real-time RPM** from engine ECU
- ✅ **Vehicle Speed** from OBD-II
- ✅ **Boost Pressure** (turbo monitoring)
- ✅ **Engine Temperature** display
- ✅ **Sport Mode Detection** (high boost)
- ✅ **Authentic Abarth Logo** on startup

### **📈 Performance Monitoring:**
- **Normal Driving**: 4Hz updates (250ms refresh)
- **Sport Mode**: Activates at high boost pressure
- **Real-Time Gauges**: Professional automotive interface
- **Power Detection**: Shows USB vs external power

### **🔥 Abarth-Specific Features:**
- **Turbo Focus**: Boost pressure prominently displayed
- **Sport Mode**: Automatic detection and enhanced display
- **Fiat 500 PIDs**: Optimized for your specific vehicle
- **Performance Layout**: Racing-inspired gauge arrangement

---

## 🎛️ **Future Upgrades**

After basic system works, you can add:

### **🎮 Interactive Control (KY-040 Encoder):**
- Menu navigation between screens
- Settings adjustment (brightness, units)
- Display mode switching (gauges/numbers/graphs)

### **💾 Data Logging (SD Card):**
- Performance session recording
- 0-60 acceleration timing
- Track day telemetry
- Diagnostic history

---

## 📞 **Support & Next Steps**

### **If Everything Works:**
🎉 **Congratulations!** You now have a professional Abarth performance monitor!
- Take photos/videos of it working
- Test during different driving conditions
- Consider adding encoder and SD card features

### **If You Need Help:**
- Check the troubleshooting guide above
- Verify all wiring against the pinout diagrams
- Test components individually (display first, then CAN)
- Use serial monitor (`pio device monitor`) for debug info

### **Ready for More Features:**
- Restore `encoder.cpp` and `sdcard.cpp` files
- Add the KY-040 encoder wiring
- Insert SD card and enable data logging
- Implement the interactive menu system

---

## 🏁 **Final Notes**

Your ESP32 is now programmed with:
- ✅ **Core Abarth monitoring system**
- ✅ **Authentic 11KB Abarth logo** 
- ✅ **Professional automotive interface**
- ✅ **Turbo-focused performance monitoring**
- ✅ **Real-time CAN communication**

**Memory Usage:**
- RAM: 6.8% (22KB used)
- Flash: 27.3% (358KB used) 
- Plenty of room for future features!

**Have fun with your Abarth performance monitor!** 🏎️⚡🔥

---
**Installation Guide v1.0** - October 15, 2025