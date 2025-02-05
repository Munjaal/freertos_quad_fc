void clearRaw(){
  for (int i=0; i<16; i++){
    drone.currentCMD.raw[i] = 0;
  }
}

void commandHandler(void *parameters)
{
  for(;;){

    if(xSemaphoreTake(sem_cmd, portMAX_DELAY) == pdPASS) {
      switch (drone.currentCMD.cmd){
        case ZER_IMU:
        drone.currentCMD.cmd = 0;
        clearRaw();
        calibrateIMU();
        xSemaphoreGive(sem_cmd);
        break;

        case FLY_TKO:
        drone.currentCMD.cmd = 0;
        drone.alt_setpoint = drone.currentCMD.intValue;
        clearRaw();
        xSemaphoreGive(sem_cmd);
        break;

        case SET_PSI:
        // Serial.println("SET_PSI");
        drone.currentCMD.cmd = 0;
        // TAKE SEMAPHORE FIRST
        if(xSemaphoreTake(sem_ahrs,portMAX_DELAY) == pdPASS){
          drone.setpoint[2] = drone.currentCMD.values[3];
          xSemaphoreGive(sem_ahrs);
        }
        // GIVE HERE
        clearRaw();
        xSemaphoreGive(sem_cmd);
        break;

        case SET_OMG:
        drone.currentCMD.cmd = 0;
        if(xSemaphoreTake(sem_motor,portMAX_DELAY) == pdPASS){
          for(int i = 0; i < 4; i++){
            drone.speeds[i] = constrain(drone.currentCMD.intArr[i],MOTOR_MIN_THROTTLE, MOTOR_MAX_THROTTLE);   // MOTORS DISARMED if not alive
          }
          xSemaphoreGive(sem_motor);
        }
        clearRaw();
        xSemaphoreGive(sem_cmd);
        break;

        case SET_KP:
        drone.currentCMD.cmd = 0;
        drone.Kp = drone.currentCMD.values[0];
        // Serial.print(drone.Kp);
        // Serial.print('\t');
        // Serial.print(drone.Ki);
        // Serial.print('\t');
        // Serial.print(drone.Kd);
        // Serial.println();
        clearRaw();
        xSemaphoreGive(sem_cmd);
        break;

        case SET_KI:
        drone.currentCMD.cmd = 0;
        drone.Ki = drone.currentCMD.values[0];
        clearRaw();
        xSemaphoreGive(sem_cmd);
        break;

        case SET_KD:
        drone.currentCMD.cmd = 0;
        drone.Kd = drone.currentCMD.values[0];
        // Serial.print(drone.Kp);
        // Serial.print('\t');
        // Serial.print(drone.Ki);
        // Serial.print('\t');
        // Serial.print(drone.Kd);
        // Serial.println();
        clearRaw();
        xSemaphoreGive(sem_cmd);
        break;

        case REBOOT:
        clearRaw();
        rp2040.reboot();
        break;

        default:
        xSemaphoreGive(sem_cmd);
        break;
      }
      vTaskDelay(500);
    }
  }
}

// Take Off 
void flight_takeOff();

// Hover
// Land
void flight_land();

