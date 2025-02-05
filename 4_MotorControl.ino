void motorControl(void *parameters){
  for(;;)
  {
    if(xSemaphoreTake(sem_motor, (TickType_t) 2 == pdTRUE))
    {
      //  && (drone.batt_low == false)
      if ((drone.alive)){
        for(int i = 0; i < 4; i++)
        {
          drone.motor[i].writeMicroseconds(drone.speeds[i]);
        }
      }
      else{
        for(int i = 0; i < 4; i++)
        {
          drone.speeds[i] = MOTOR_MIN_THROTTLE;
          drone.motor[i].writeMicroseconds(MOTOR_MIN_THROTTLE);   // MOTORS DISARMED if not alive
        }
      }
      xSemaphoreGive(sem_motor);
      vTaskDelay(pdMS_TO_TICKS(1000/CTRL_FREQ));
    }
    else
    {
      vTaskDelay(1);
    }
  }
}
