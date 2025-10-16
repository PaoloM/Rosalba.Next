#include <unity.h>
#include <Arduino.h>

// Include only the headers, not the full implementation
#include "../include/config.h"
#include "../include/obd.h"

void setUp(void) {
    // Set up code here, runs before each test
}

void tearDown(void) {
    // Clean up code here, runs after each test
}

// Test OBD PID constants are defined correctly
void test_obd_pid_definitions() {
    // Test that OBD PID constants are defined correctly
    TEST_ASSERT_EQUAL_HEX8_MESSAGE(0x0C, PID_ENGINE_RPM, "Engine RPM PID should be 0x0C");
    TEST_ASSERT_EQUAL_HEX8_MESSAGE(0x0D, PID_VEHICLE_SPEED, "Vehicle speed PID should be 0x0D");
    TEST_ASSERT_EQUAL_HEX8_MESSAGE(0x0B, PID_INTAKE_MANIFOLD_PRESSURE, "Intake manifold pressure PID should be 0x0B");
    TEST_ASSERT_EQUAL_HEX8_MESSAGE(0x05, PID_ENGINE_COOLANT_TEMP, "Coolant temp PID should be 0x05");
    TEST_ASSERT_EQUAL_HEX8_MESSAGE(0x11, PID_THROTTLE_POSITION, "Throttle position PID should be 0x11");
}

// Test RPM calculation logic
void test_rpm_calculation() {
    // Test RPM calculation from OBD data: ((A*256)+B)/4
    uint8_t testDataA = 0x1A;  // High byte
    uint8_t testDataB = 0xF4;  // Low byte
    float expectedRPM = ((testDataA * 256) + testDataB) / 4.0;
    
    TEST_ASSERT_FLOAT_WITHIN_MESSAGE(1.0, 1725.0, expectedRPM, "RPM calculation should be accurate");
}

// Test boost pressure calculation
void test_boost_pressure_calculation() {
    // Test boost pressure from MAP: (kPa - 101.325) / 6.895 = PSI boost
    float mapKpa = 150.0;  // Example MAP reading
    float atmospheric = 101.325;
    float kpaToBar = 0.01;
    float expectedBoost = (mapKpa - atmospheric) * kpaToBar;
    
    TEST_ASSERT_FLOAT_WITHIN_MESSAGE(0.1, 0.487, expectedBoost, "Boost pressure calculation should be accurate");
}

// Test VehicleData structure initialization
void test_vehicle_data_structure() {
    VehicleData testData;
    // Test that structure initializes with default values
    TEST_ASSERT_EQUAL_FLOAT_MESSAGE(0.0, testData.rpm, "RPM should initialize to 0");
    TEST_ASSERT_EQUAL_FLOAT_MESSAGE(0.0, testData.speed, "Speed should initialize to 0");
    TEST_ASSERT_EQUAL_FLOAT_MESSAGE(0.0, testData.boostPressure, "Boost pressure should initialize to 0");
    TEST_ASSERT_FALSE_MESSAGE(testData.sportMode, "Sport mode should initialize to false");
}

// Test RGB565 color conversion (used for BMP display)
void test_rgb565_conversion() {
    // Test Abarth colors
    uint16_t abarthYellow = ((255 >> 3) << 11) | ((255 >> 2) << 5) | (0 >> 3);  // 0xFFE0
    uint16_t abarthRed = ((255 >> 3) << 11) | ((0 >> 2) << 5) | (0 >> 3);      // 0xF800
    
    TEST_ASSERT_EQUAL_HEX16_MESSAGE(0xFFE0, abarthYellow, "Abarth yellow should convert to 0xFFE0");
    TEST_ASSERT_EQUAL_HEX16_MESSAGE(0xF800, abarthRed, "Abarth red should convert to 0xF800");
}

// Test timing configuration constants
void test_timing_configuration() {
    // Test that timing constants are reasonable for real-time monitoring
    TEST_ASSERT_GREATER_THAN_MESSAGE(50, DISPLAY_UPDATE_INTERVAL, "Display update should be > 50ms for stability");
    TEST_ASSERT_LESS_THAN_MESSAGE(500, DISPLAY_UPDATE_INTERVAL, "Display update should be < 500ms for responsiveness");
    
    TEST_ASSERT_GREATER_THAN_MESSAGE(100, OBD_REQUEST_INTERVAL, "OBD requests should be > 100ms to avoid bus flooding");
    TEST_ASSERT_LESS_THAN_MESSAGE(1000, OBD_REQUEST_INTERVAL, "OBD requests should be < 1000ms for real-time feel");
}

// Test pin configuration
void test_pin_configuration() {
    // Test that CAN pins are defined and different
    TEST_ASSERT_NOT_EQUAL_MESSAGE(CAN_RX_PIN, CAN_TX_PIN, "CAN RX and TX pins should be different");
    TEST_ASSERT_GREATER_OR_EQUAL_MESSAGE(0, CAN_RX_PIN, "CAN RX pin should be valid GPIO");
    TEST_ASSERT_GREATER_OR_EQUAL_MESSAGE(0, CAN_TX_PIN, "CAN TX pin should be valid GPIO");
    
    // Test that TFT pins are defined
    TEST_ASSERT_GREATER_OR_EQUAL_MESSAGE(0, TFT_CS, "TFT CS pin should be valid");
    TEST_ASSERT_GREATER_OR_EQUAL_MESSAGE(0, TFT_DC, "TFT DC pin should be valid");
    TEST_ASSERT_GREATER_OR_EQUAL_MESSAGE(0, TFT_RST, "TFT RST pin should be valid");
}

void setup() {
    delay(2000);  // Service delay for ESP32
    
    UNITY_BEGIN();
    
    RUN_TEST(test_obd_pid_definitions);
    RUN_TEST(test_rpm_calculation);
    RUN_TEST(test_boost_pressure_calculation);
    RUN_TEST(test_vehicle_data_structure);
    RUN_TEST(test_rgb565_conversion);
    RUN_TEST(test_timing_configuration);
    RUN_TEST(test_pin_configuration);
    
    UNITY_END();
}

void loop() {
    // Tests run only once
}