# FreeRTOS-Based Quadcopter Flight Controller

A real-time flight controller implementation for a quadcopter using FreeRTOS on a Raspberry Pi Pico W. This project implements a complete flight control system with wireless connectivity, sensor fusion, and PID control.

## Hardware Components

- **Main Controller**: Raspberry Pi Pico W
- **IMU**: BNO085 for orientation sensing
- **Altitude Sensor**: VL53L0X single point LIDAR
- **Motor Control**: 4-in-1 ESC
- **Motors**: 4x Brushless DC Motors

## Features

- FreeRTOS task-based architecture for real-time operation
- WiFi-based command and control interface
- Sensor fusion for accurate orientation estimation
- PID control for stable flight
- Real-time altitude sensing
- Battery monitoring
- Emergency failsafe mechanisms
- Configurable PID gains over WiFi

## System Architecture

The system is built on multiple FreeRTOS tasks:

- `IMUReadAndFusion`: Handles sensor reading and fusion
- `heartBeat`: Manages WiFi connection and system health
- `motorControl`: Controls motor outputs
- `controller_PID`: Implements the PID control loop
- `getBattLvl`: Monitors battery status
- `logData`: Handles system data logging
- `commandHandler`: Processes incoming commands
- `getAlt`: Manages altitude measurements

## Communication Protocol

The system uses a TCP-based communication protocol with the following command structure:

- Command ID (1 byte)
- Optional payload (varies by command)

Supported commands (WIP) include:
- System control (takeoff, land, emergency stop)
- PID tuning (set Kp, Ki, Kd)
- Orientation control
- Motor speed control
- System status requests

## Safety Features

- Heartbeat monitoring for connection health
- Battery voltage monitoring with low-battery failsafe
- Motor disarm on connection loss
- Attitude limits for safe operation

## Getting Started

1. Clone this repository
2. Upload the Arduino code to your Raspberry Pi Pico W
3. Configure your WiFi credentials in `pico_W_FC_RTOS_4.ino`
4. Connect the hardware components according to the pin definitions
5. Run the Python control script (`hB2_Latest_UI.py`) with your Pico W's IP address

## Dependencies

### Hardware
- Raspberry Pi Pico W
- BNO085 IMU
- VL53L0X LIDAR
- 4-in-1 ESC
- Suitable power distribution system

### Software
- Arduino IDE with Raspberry Pi Pico support
- FreeRTOS
- Required Arduino libraries:
  - Adafruit_BNO08x
  - WiFi
  - Servo
  - Wire
  - VL53L0X_mod

### Python Dependencies
- socket
- threading
- numpy
- struct

## Configuration

Key parameters can be configured in the code:

```cpp
#define CTRL_FREQ                75    // Controller frequency (Hz)
#define ALT_TIME_PERIOD          50    // Altitude measurement period (ms)
#define MOTOR_MAX_THROTTLE       1300  // Maximum motor throttle (μs)
#define HOVER_SPEED             1150   // Hover throttle setting (μs)
#define MOTOR_MIN_THROTTLE      1000   // Minimum motor throttle (μs)
```

## Contributing

This is a work in progress. Contributions, suggestions, and improvements are welcome. Please feel free to submit issues and pull requests.


## Safety Warning

This is an experimental flight controller. Always follow proper safety procedures when testing and flying. Never operate the quadcopter near people or property that could be harmed.

## Status

This project is currently a Work In Progress (WIP). Some features may not be fully implemented or tested.