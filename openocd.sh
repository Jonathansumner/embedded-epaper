#!/bin/bash

# Define variables
OPENOCD_PATH="/usr/local/bin/openocd"
CONFIG_PATH="/home/joe/Desktop/Projects/ePaper/st_nucleo_f3.cfg"
GDB_PORT=3333
TELNET_PORT=4444

# Function to start the OpenOCD server
function start_openocd_server() {
    "$OPENOCD_PATH" -d2 -f "$CONFIG_PATH" -c "gdb_port $GDB_PORT" -c "telnet_port $TELNET_PORT"
}

# Main script execution
start_openocd_server

echo "OpenOCD server started successfully."
