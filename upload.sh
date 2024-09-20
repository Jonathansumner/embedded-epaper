#!/bin/bash

# Define variables
BUILD_DIR="cmake-build-debug"
STM32_PROGRAMMER_PATH="/home/joe/STMicroelectronics/STM32Cube/STM32CubeProgrammer/bin/STM32_Programmer_CLI"
TARGET_PORT="swd"
FREQUENCY=4000
MODE="NORMAL"
AP=0
RESET="SWrst"
SHARED=0
ELF_FILE="$BUILD_DIR/ePaper.elf"
SPEED="Reliable"

# Function to build the project using CMake
function build_project() {
    if [ ! -d "$BUILD_DIR" ]; then
        mkdir "$BUILD_DIR"
    fi

    cd "$BUILD_DIR" || exit
    cmake ..
    make
    cd .. || exit
}

# Function to check if STM32 Programmer CLI exists
function check_stm32_programmer() {
    if [ ! -f "$STM32_PROGRAMMER_PATH" ]; then
        echo "Error: STM32 Programmer CLI not found at $STM32_PROGRAMMER_PATH"
        exit 1
    fi
}

# Function to program the STM32 device and reset it
function program_device() {
    "$STM32_PROGRAMMER_PATH" -c port="$TARGET_PORT" freq="$FREQUENCY" mode="$MODE" ap="$AP" reset="$RESET" shared="$SHARED" -d "$ELF_FILE" -v speed="$SPEED"

    # Reset the MCU after programming
    "$STM32_PROGRAMMER_PATH" -c port="$TARGET_PORT" reset=SWrst
}

# Main script execution
build_project
check_stm32_programmer
program_device

echo "Programming completed successfully and MCU has been reset."
