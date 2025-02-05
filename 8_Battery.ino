
void getBattLvl(void *parameters){
  float window[] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
  uint8_t idx = 0;
  float part_sum = 0.0;
  for(;;){
    // Analog Read Battery Here
    if(xSemaphoreTake(sem_batt,portMAX_DELAY) == pdPASS){
      float _bt = float(analogRead(26))/1024;
      // Serial.println(_bt);

      drone.batt = ((_bt*3.3)*(3.6+0.64)/(0.64)); //batt lvl in Volts Divider is (R1 - 3.6k R2 - 0.64k)
      drone.batt_perc = ((drone.batt/BATT_NUM_CELLS) -  BATT_MIN_V)/(BATT_MAX_V - BATT_MIN_V)*100;    // Battery level in %
      part_sum = part_sum + drone.batt_perc - window[idx];
      window[idx] = drone.batt_perc;
      drone.batt_perc = part_sum/8.0;
      
      idx += 1;
      idx = idx & 0b0111;


      if (drone.batt_perc <= 30)
      {
        drone.alive = false;
        drone.batt_low = true;
      }
      else
      {
        drone.batt_low = false;
      }
      // batt = analogRead(26);
      // Serial.println(batt);
      xSemaphoreGive(sem_batt);
      vTaskDelay(pdMS_TO_TICKS(100));
    }
  }
}