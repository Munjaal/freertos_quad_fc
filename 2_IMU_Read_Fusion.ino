void IMUReadAndFusion(void *parameters) {
  TickType_t xLastWakeTime;
  const TickType_t xFrequency = 1000/CTRL_FREQ;
  BaseType_t xWasDelayed;
  xLastWakeTime = xTaskGetTickCount();
  float prev_yaw = 0;
  for (;;) { // Task loop
    // int32_t accelerometer[3];
    // int32_t gyroscope[3];
    // vTaskDelayUntil(&xLastWakeTime,xFrequency);
    if(xSemaphoreTake(sem_i2c, (TickType_t) 2 == pdTRUE))
    {
      // sfe_ism_data_t accelData;
      // sfe_ism_data_t gyroData;
      if(xSemaphoreTake(sem_ahrs, (TickType_t) 2 == pdTRUE))
      {
        if (bno08x.getSensorEvent(&sensorValue)) {
          switch (sensorValue.sensorId) {
            case SH2_ARVR_STABILIZED_RV:
              quaternionToEulerRV(&sensorValue.un.arvrStabilizedRV, &drone.ypr, true);
              break;
            case SH2_GYROSCOPE_CALIBRATED:
              drone.globalGyroscope[0] = sensorValue.un.gyroscope.x;
              drone.globalGyroscope[1] = sensorValue.un.gyroscope.y;
              drone.globalGyroscope[2] = sensorValue.un.gyroscope.z;
              break;
          }
        }
        
        else{
          Serial.println("No Sensor Event!");
        }
        // if (myISM.checkAccelStatus())
        // {
        //     myISM.getAccel(&accelData); // Read the latest accel data
        //     drone.globalAccelerometer[0] = accelData.xData; // Store X value
        //     drone.globalAccelerometer[1] = accelData.yData; // Store Y value
        //     drone.globalAccelerometer[2] = accelData.zData; // Store Z value
        // }

        // if (myISM.checkGyroStatus())
        // {
        //     myISM.getGyro(&gyroData); // Read the latest gyro data
        //     drone.globalGyroscope[0] = gyroData.xData * 0.0174533 / 1000; // Store X value
        //     drone.globalGyroscope[1] = gyroData.yData * 0.0174533 / 1000; // Store Y value
        //     drone.globalGyroscope[2] = gyroData.zData * 0.0174533 / 1000; // Store Z value
        // }



        // float deltat = fusion.deltatUpdate();

        // fusion.MahonyUpdate(drone.globalGyroscope[0], drone.globalGyroscope[1], drone.globalGyroscope[2]-GYRO_Z_DRIFT_CORRECTION,
                            // drone.globalAccelerometer[0], drone.globalAccelerometer[1], drone.globalAccelerometer[2], deltat);

      // Update global orientation variables (pitch, roll, yaw)
      // Ensure thread-safe access if these variables are shared
      
        // drone.pitch = fusion.getPitch() - drone.pitch_offset;
        // drone.pitch = drone.ypr.pitch;
        // drone.roll = fusion.getRoll() - drone.roll_offset;
        // drone.roll = drone.ypr.roll;
        // drone.yaw = fusion.getYaw() - drone.yaw_offset;
        // drone.yaw = drone.ypr.yaw;
        xSemaphoreGive(sem_ahrs);
      }
      else
      {
        //TODO: WHAT IF SEMAPHORE BUSY ? (Ideally shouldn't happen fore more than 2 ticks)
        Serial.println("SEM_AHRS Unavailable");
      }
      
      // Add a delay to control task execution frequency
      // Use vTaskDelay for FreeRTOS delay
      xSemaphoreGive(sem_i2c);
      vTaskDelay(pdMS_TO_TICKS(1000/CTRL_FREQ)); 
    }  
    else
    {
      // Serial.println("SEM_I2C Unavailable");
      vTaskDelay(1);
    }
    // vTaskDelay(pdMS_TO_TICKS(20)); 
  }
}


void calibrateIMU()
{
  if(xSemaphoreTake(sem_ahrs, portMAX_DELAY) == pdPASS)
  {
    drone.pitch_offset = drone.ypr.pitch;
    drone.roll_offset = drone.ypr.roll;
    drone.yaw_offset = drone.ypr.yaw;
    xSemaphoreGive(sem_ahrs);
  }
}


// void initIMU()
// {
//   Wire.begin();
//   Wire.setClock(400000);
//   while (!myISM.begin())
//   {
//       Serial.println("ISM did not begin. Please check the wiring...");
//       delay(1000);
//   }

//   myISM.deviceReset();
//   while (!myISM.getDeviceReset())
//   {
//       delay(1);
//   }

//   myISM.setDeviceConfig();
//   myISM.setBlockDataUpdate();

//   myISM.setAccelDataRate(ISM_XL_ODR_416Hz);
//   myISM.setAccelFullScale(ISM_8g);

//   myISM.setGyroDataRate(ISM_GY_ODR_416Hz);
//   myISM.setGyroFullScale(ISM_2000dps);

//   myISM.setAccelFilterLP2();
//   myISM.setAccelSlopeFilter(ISM_LP_ODR_DIV_100);

//   myISM.setGyroFilterLP1();
//   myISM.setGyroLP1Bandwidth(ISM_MEDIUM);
// }