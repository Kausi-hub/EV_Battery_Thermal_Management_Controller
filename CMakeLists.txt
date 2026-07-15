cmake_minimum_required(VERSION 3.20)

project(
    BatteryThermalController
    VERSION 1.0
    LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(ENABLE_COVERAGE "Enable Coverage" ON)

if(ENABLE_COVERAGE AND NOT MSVC)

    add_compile_options(
        --coverage
        -O0
        -g)

    add_link_options(
        --coverage)

endif()

include(FetchContent)

FetchContent_Declare(
    googletest
    URL
    https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip)

FetchContent_MakeAvailable(googletest)

enable_testing()

include_directories(include)

set(SOURCES

    src/thermal_controller.cpp
    src/pid_controller.cpp
    src/diagnostics.cpp
    src/watchdog.cpp
    src/battery_model.cpp
    src/coolant_model.cpp
    src/ConfigLoader.cpp
    src/fault_manager.cpp
    src/PumpDriver.cpp
    src/BatteryTemperatureSensor.cpp
    src/telemetry_logger.cpp
)

add_library(
    thermal_lib
    ${SOURCES})

add_executable(
    thermal_controller
    app/main.cpp)

add_custom_command(
    TARGET thermal_controller POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${CMAKE_SOURCE_DIR}/thermal_calibration.txt
    $<TARGET_FILE_DIR:thermal_controller>
)

target_link_libraries(
    thermal_controller
    thermal_lib)

add_executable(
    thermal_tests

    tests/test_pid.cpp
    tests/test_controller.cpp
    tests/test_diagnostics.cpp

    ${SOURCES})

target_link_libraries(
    thermal_tests

    GTest::gtest_main)

include(GoogleTest)

gtest_discover_tests(
    thermal_tests)