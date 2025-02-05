void logData(void *parameters){
  for (;;) { // Task Loop
      Serial.print(drone.speeds[0]);
      Serial.print('\t');
      Serial.print(drone.speeds[1]);
      Serial.print('\t');
      Serial.print(drone.speeds[2]);
      Serial.print('\t');
      Serial.print(drone.speeds[3]);
      Serial.print('\t');
      Serial.print(drone.alive);
      // Serial.print('\t');
      // Serial.print(drone.batt);
      // Serial.print('\t');
      // Serial.print(0);
      Serial.print('\t');
      Serial.print(drone.batt_perc);
      // Serial.print('\t');
      // Serial.print(100);
      // Serial.print(drone.alt);
      // Serial.print('\t');
      // Serial.print(',');
      // Serial.print(drone.ypr.roll);
      // Serial.print(',');
      // Serial.print(drone.ypr.pitch);
      // Serial.print(',');
      // Serial.print(drone.ypr.yaw);
      Serial.print('\t');
      Serial.print(drone.globalGyroscope[0]);
      Serial.print('\t');
      Serial.print(drone.globalGyroscope[1]);
      Serial.print('\t');
      Serial.print(drone.globalGyroscope[2]);
      // Serial.print(drone.currentCMD.cmd);
      // Serial.print('\t');
      // for(int i=0;i<16;i++){
      //   Serial.print(drone.currentCMD.raw[15-i]);
      //   Serial.print(' ');
      // }

      // Serial.print('\t');


      // Serial.print(drone.setpoint[2]);
      // Serial.println();

      // for (int i = 0;i<3;i++)
      // {
      //   Serial.print(globalAccelerometer[i]);
      //   Serial.print('\t');
      // }
      // for (int i = 0;i<3;i++)
      // {
      //   Serial.print(globalGyroscope[i]);
      //   Serial.print('\t');
      // }
      Serial.println();
      vTaskDelay(pdMS_TO_TICKS(100));

      
      // if ((millis() > 7000) && (ctrl_EN == false))
      // {
      //   ctrl_EN = true;
      // }

  }
}
