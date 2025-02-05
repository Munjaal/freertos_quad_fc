// #include "SparkFun_ISM330DHCX.h"

#include <Adafruit_BNO08x.h>

#include "SensorFusion.h" //SF
#include <SparkFun_MMC5983MA_Arduino_Library.h>
#include <FreeRTOS.h>
#include <task.h>
#include <Servo.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include <Wire.h>
#include <VL53L0X_mod.h>
#include "semphr.h"


#define configTICK_RATE_HZ           ( TickType_t ) 1000  // RTOS Scheduler time slice
#define CTRL_FREQ                                   75   // controller frequency (Hz)
#define ALT_TIME_PERIOD                             50    // time in ms between altitude measurements
#define MOTOR_MAX_THROTTLE                          1300  // signal pulse in us for max throttle (50Hz)
#define HOVER_SPEED                                 1150  // signal pulse in us for hover throttle (Calibrate to actual value)
#define MOTOR_MIN_THROTTLE                          1000  // Signal pulse for motor off
#define AHRS_ERR_SUM_MINMAX                         200   // Constrain on the sum(err) for Ki, applied both pos (+) and neg (-)
#define BATT_NUM_CELLS                              3     // XSYP
#define BATT_MAX_V                                  4.2   // ABS MAX PER CELL
#define BATT_MIN_V                                  3.2   // ABS MIN PER CELL
#define REFRESH_INTERVAL                            10000  // SERVO LIBRARY refresh inverval in us (default 20000 = 20ms = 50Hz)
#define GYRO_Z_DRIFT_CORRECTION                     0.0055 // Gyro Drift correction [experimentally tested]
#define BNO08X_RESET                                -1      // BNO085 RESET CONST

// REFRESH AT 5000us = 200Hz 
#define ESC_Calib_Delay                             1000

// WIFI DEFS
#ifndef STASSID
#define STASSID                                     "WIFI SSID"
#define STAPSK                                      "WIFI PASSWORD"
#define WIFI_PORT                                   80
#endif

