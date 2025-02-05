void controller_PID(void *parameters){
  float err[3] = {0,0,0};
  float prev_err[3] = {0,0,0};
  float diff_err[3] = {0,0,0};
  float sum_err[3] = {0,0,0};
  float setpoint[3] = {0,0,0}; //Target
  float curr_ahrs[3] = {0,0,0};
  float adj[4] = {0,0,0,0};

  for(;;){
    
    if(drone.ctrl_EN == true)
    {
      if(xSemaphoreTake(sem_ahrs, portMAX_DELAY) == pdPASS)
      {
        curr_ahrs[0] = drone.ypr.roll;
        curr_ahrs[1] = drone.ypr.pitch;
        curr_ahrs[2] = drone.ypr.yaw; // DISABLED for Calibration
        setpoint[0] = drone.setpoint[0];
        setpoint[1] = drone.setpoint[1];
        setpoint[2] = drone.setpoint[2];
        xSemaphoreGive(sem_ahrs);

        for(int i=0; i<3; i++)
        {
          prev_err[i] = err[i];
          err[i] =  curr_ahrs[i] - setpoint[i];
          sum_err[i] += err[i]*float(1.0f/CTRL_FREQ);
          sum_err[i] = constrain(sum_err[i],-1*AHRS_ERR_SUM_MINMAX,AHRS_ERR_SUM_MINMAX);
          // diff_err[i] = (err[i] - prev_err[i])/float(1.0f/CTRL_FREQ);
          diff_err[i] = -1*drone.globalGyroscope[i];
          // if (i == 0){
          //   Serial.println(diff_err[i]);
          // }
        }
        // Throttle values are in us from 1000 - 2000 us (MOTOR_MAX_THROTTLE)
        // TODO: [Implement different gains for yaw,pitch,roll], [decide signs of err based on motor positions]

        // adj[0] = drone.Kp*(-err[0] - err[1] + err[2]) + drone.Ki*(-sum_err[0] - sum_err[1] + sum_err[2]) + drone.Kd*(-diff_err[0] - diff_err[1] + diff_err[2]);
        adj[0] = drone.Kp*(-err[0] - err[1]) + drone.Kp_yaw*(err[2]) + drone.Ki*(-sum_err[0] - sum_err[1]) + drone.Ki_yaw*(sum_err[2]) + drone.Kd*(-diff_err[0] - diff_err[1]) + drone.Kd_yaw*(diff_err[2]);
        adj[1] = drone.Kp*(-err[0] + err[1]) + drone.Kp_yaw*(err[2]) + drone.Ki*(-sum_err[0] + sum_err[1]) + drone.Ki_yaw*(sum_err[2]) + drone.Kd*(-diff_err[0] + diff_err[1]) + drone.Kd_yaw*(diff_err[2]);
        adj[2] = drone.Kp*( err[0] - err[1]) + drone.Kp_yaw*(err[2]) + drone.Ki*( sum_err[0] - sum_err[1]) + drone.Ki_yaw*(sum_err[2]) + drone.Kd*( diff_err[0] - diff_err[1]) + drone.Kd_yaw*(diff_err[2]);
        adj[3] = drone.Kp*( err[0] + err[1]) + drone.Kp_yaw*(err[2]) + drone.Ki*( sum_err[0] + sum_err[1]) + drone.Ki_yaw*(sum_err[2]) + drone.Kd*( diff_err[0] + diff_err[1]) + drone.Kd_yaw*(diff_err[2]);

        if(xSemaphoreTake(sem_motor, (TickType_t) 2 == pdTRUE))
        {
          for(int i=0; i<4; i++)
          {
            drone.speeds[i] = constrain((HOVER_SPEED + adj[i]),MOTOR_MIN_THROTTLE,MOTOR_MAX_THROTTLE);
          }
          xSemaphoreGive(sem_motor);
        }
        vTaskDelay(pdMS_TO_TICKS(1000/CTRL_FREQ));
      }
    }
    else
    {
      vTaskDelay(5);
    }
  }
}