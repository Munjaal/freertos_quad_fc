

// WiFiUDP Udp;
WiFiServer server(WIFI_PORT); // Example server on port 80
WiFiClient client;

// SF Variables
SF fusion;

// IMU
// SparkFun_ISM330DHCX myISM;
Adafruit_BNO08x  bno08x(BNO08X_RESET);

struct euler_t {
  float yaw;
  float pitch;
  float roll;
  };


sh2_SensorValue_t sensorValue;
sh2_SensorId_t reportType = SH2_ARVR_STABILIZED_RV;

long reportIntervalUs = 5000;



void quaternionToEuler(float qr, float qi, float qj, float qk, euler_t* ypr, bool degrees = false) {

    float sqr = sq(qr);
    float sqi = sq(qi);
    float sqj = sq(qj);
    float sqk = sq(qk);

    ypr->yaw = atan2(2.0 * (qi * qj + qk * qr), (sqi - sqj - sqk + sqr));
    ypr->pitch = asin(-2.0 * (qi * qk - qj * qr) / (sqi + sqj + sqk + sqr));
    ypr->roll = atan2(2.0 * (qj * qk + qi * qr), (-sqi - sqj + sqk + sqr));

    if (degrees) {
      ypr->yaw *= RAD_TO_DEG;
      ypr->pitch *= RAD_TO_DEG;
      ypr->roll *= RAD_TO_DEG;
    }
}


void quaternionToEulerRV(sh2_RotationVectorWAcc_t* rotational_vector, euler_t* ypr, bool degrees = false) {
    quaternionToEuler(rotational_vector->real, rotational_vector->i, rotational_vector->j, rotational_vector->k, ypr, degrees);
}

void quaternionToEulerGI(sh2_GyroIntegratedRV_t* rotational_vector, euler_t* ypr, bool degrees = false) {
    quaternionToEuler(rotational_vector->real, rotational_vector->i, rotational_vector->j, rotational_vector->k, ypr, degrees);
}




// LIDAR
VL53L0X_mod lox;



SemaphoreHandle_t sem_i2c, sem_motor, sem_ahrs, sem_batt, sem_cmd, sem_wifi;


//MOTOR PINS
// M0 - RL
// M1 - FL
// M2 - RR
// M3 - FR






struct SystemState {
  // AHRS VARS
  volatile float globalAccelerometer[3];
  volatile float globalGyroscope[3];
  volatile float globalPosition[3];   // If using external position tracking / WiFi Update
  volatile float globalVelocity[3];
  volatile float deltat;
  // volatile float yaw, pitch, roll;
  float pitch_offset = 0;
  float roll_offset = 0;
  float yaw_offset = 0;
  volatile float setpoint[3] = {0,0,0};   // Phi,Tht,Psi, Z(mm) ?
  volatile int32_t alt_setpoint;
  volatile int16_t alt;
  
  euler_t ypr;

  // MOTOR
  const uint8_t mPins[4] = {18,19,20,21};
  volatile uint16_t speeds[4] = {1000,1000,1000,1000};
  Servo motor[4];

  // BATTERY
  float batt, batt_perc;
  volatile bool batt_low = false;
  
  // SAFETY
  volatile bool alive = false;
  volatile bool ctrl_EN = true;
  const unsigned long heartbeatInterval = 1000;
  unsigned long lastHeartbeatTime = 0;

  // COMMAND
  volatile CommandData currentCMD;

  // CONTROLLER GAINS
  volatile float Kp = 1;
  volatile float Ki = 0;
  volatile float Kd = -10;

  const float Kp_yaw = 0;
  const float Ki_yaw = 0;
  const float Kd_yaw = 0;

  // Drone Parameters
  const uint8_t drone_l = 0;
  const uint8_t drone_b = 0;
  const uint8_t drone_k = 0;
};



SystemState drone;
