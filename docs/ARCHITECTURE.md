# Code Architecture and Refactoring

## Overview

The Rosalba.Next codebase has been refactored from a single large `main.cpp` file (430+ lines) into a modular architecture with clear separation of concerns. This improves maintainability, readability, and testability.

## New File Structure

```
src/
├── main.cpp          # Main application entry point (70 lines)
├── obd.cpp           # OBD-II/CAN bus functionality
├── display.cpp       # TFT display management
└── power.cpp         # Power source detection

include/
├── config.h          # Configuration constants and pin definitions
├── obd.h             # OBD-II function declarations and data structures
├── display.h         # Display function declarations
└── power.h           # Power monitoring function declarations
```

## Module Responsibilities

### 1. **main.cpp** (70 lines)
- **Purpose**: Application entry point and main loop
- **Responsibilities**:
  - System initialization orchestration
  - Main program loop with timing control
  - Error handling and system startup
- **Dependencies**: All other modules
- **Key Functions**: `setup()`, `loop()`

### 2. **config.h**
- **Purpose**: Central configuration and constants
- **Contents**:
  - Pin definitions (CAN, Display, Power)
  - Hardware configuration (screen size, CAN speed)
  - Timing intervals (display update, OBD requests)
  - Color definitions for TFT display
- **Benefits**: Single place to modify hardware settings

### 3. **obd.cpp/.h**
- **Purpose**: Vehicle diagnostics and CAN communication
- **Responsibilities**:
  - CAN bus initialization and management
  - OBD-II protocol implementation
  - PID request/response handling
  - Vehicle data parsing and storage
- **Key Functions**:
  - `initializeCAN()` - Setup CAN bus
  - `processCANMessages()` - Handle incoming messages
  - `processOBDResponse()` - Parse OBD-II data
  - `sendOBDRequest()` - Request vehicle parameters

### 4. **display.cpp/.h**
- **Purpose**: User interface and visual output
- **Responsibilities**:
  - TFT display initialization and management
  - Dashboard layout and rendering
  - Parameter visualization with color coding
  - Progress bars and status indicators
- **Key Functions**:
  - `initializeDisplay()` - Setup TFT display
  - `updateDisplay()` - Render dashboard
  - `drawParameter()` - Display individual values
  - `showStartupScreen()` - Boot sequence

### 5. **power.cpp/.h**
- **Purpose**: Power source monitoring and management
- **Responsibilities**:
  - Power source detection (USB vs External)
  - Voltage monitoring via ADC
  - Power status reporting
- **Key Functions**:
  - `initializePowerMonitoring()` - Setup power sensing
  - `detectPowerSource()` - Determine power type
  - `updatePowerStatus()` - Periodic power checks

## Benefits of Refactoring

### **Maintainability** ✅
- **Single Responsibility**: Each module has a clear, focused purpose
- **Easier Debugging**: Issues can be isolated to specific modules
- **Cleaner Code**: Shorter files are easier to navigate and understand

### **Extensibility** ✅
- **Add New Features**: Easy to add new OBD-II PIDs in `obd.cpp`
- **UI Changes**: Display modifications contained in `display.cpp`
- **Hardware Changes**: Pin changes only require `config.h` updates

### **Testability** ✅
- **Unit Testing**: Individual modules can be tested separately
- **Mock Objects**: Dependencies can be easily mocked for testing
- **Isolated Testing**: Power, display, and CAN can be tested independently

### **Code Reuse** ✅
- **Modular Functions**: Display and OBD functions can be reused in other projects
- **Library Potential**: Modules could become standalone libraries
- **Configuration Reuse**: `config.h` can be shared across similar projects

## Compilation

The modular structure is fully compatible with PlatformIO. The build system automatically:
- Compiles all `.cpp` files in `src/`
- Includes headers from `include/`
- Links everything into final binary

**No build configuration changes required!**

## Adding New Features

### **New OBD-II Parameter**
1. Add PID definition to `obd.h`
2. Add data field to `VehicleData` struct in `obd.h`
3. Add processing case in `processOBDResponse()` in `obd.cpp`
4. Add display element in `updateDisplay()` in `display.cpp`

### **New Display Element**
1. Add drawing function to `display.cpp`
2. Declare function in `display.h`
3. Call from `updateDisplay()` function

### **New Configuration**
1. Add constant to `config.h`
2. Use throughout codebase with `#include "config.h"`

## Before vs After

| Aspect | Before Refactoring | After Refactoring |
|--------|-------------------|-------------------|
| **File Count** | 1 large file | 9 focused files |
| **Lines per File** | 430 lines | 70-150 lines each |
| **Responsibilities** | Mixed concerns | Single responsibility |
| **Maintainability** | Difficult | Easy |
| **Testing** | Monolithic | Modular |
| **Code Navigation** | Scroll heavy | File-based |

## Migration Notes

- **Existing functionality**: All features preserved, no behavior changes
- **Pin assignments**: Still configured via PlatformIO build flags
- **Performance**: No performance impact, same compiled output
- **Memory usage**: Identical memory footprint

---

**Last Updated**: October 15, 2025  
**Refactoring Date**: October 15, 2025  
**Benefits**: Improved maintainability, testability, and extensibility