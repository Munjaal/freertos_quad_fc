void getAlt(void *parameters){
  lox.startContinuous(ALT_TIME_PERIOD);
  uint16_t a1t;
  for(;;){
    if(xSemaphoreTake(sem_i2c, (TickType_t) 2 == pdTRUE))
    {
      // Serial.println(lox.isRangeComplete());
      if (lox.readRangeNoBlocking(a1t)) {  // phase failures have incorrect data
        if(xSemaphoreTake(sem_ahrs, (TickType_t) 2 == pdTRUE)){
          if ( (a1t >= 0) && (a1t <= 2000) && (drone.ypr.pitch <= 20) && (drone.ypr.pitch >= -20) && (drone.ypr.roll <= 20) && (drone.ypr.roll >= -20)){
            // alt = a1t;
            drone.alt = a1t/(cos(drone.ypr.pitch*0.0174533)*cos(drone.ypr.roll*0.0174533));
            // Serial.println(drone.alt);
          }
          else{
            drone.alt = -1;
          }
          xSemaphoreGive(sem_ahrs);
          // alt = lox.readRange();
        }

      }

      xSemaphoreGive(sem_i2c);
      vTaskDelay(pdMS_TO_TICKS(1000/ALT_TIME_PERIOD));
    }
    else
    {
      vTaskDelay(1);
    }
  }
}