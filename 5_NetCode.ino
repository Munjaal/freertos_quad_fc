int parseCMD(uint8_t sign){
  if (sign == BEAT){
    drone.lastHeartbeatTime = millis();
    client.write('a');
    return 0;
  }
  if(xSemaphoreTake(sem_cmd, (TickType_t) 2 == pdTRUE)){
    drone.currentCMD.cmd = sign;
    uint8_t cmd_len = cmd_size[sign];
    if (cmd_len > 0)
    {  
      if (client.available() != cmd_len){
        client.flush();
        xSemaphoreGive(sem_cmd);
        return -1;
      }
      cmd_len = cmd_len ;
      // Serial.println(sign);
      for (int i=0; i < cmd_len; i++)
      {
        drone.currentCMD.raw[i] = client.read();
        // drone.currentCMD.values[0] = 12.5;
      }
      xSemaphoreGive(sem_cmd);
      return 0;
    }
    else
    {
      xSemaphoreGive(sem_cmd);
      return 0;
    }
  }
  else{
    return -1;
  }
}


void heartBeat(void *parameters)
{
  // Non-blocking call to accept a client
  for(;;){
    
    client = server.accept();
    
    // Serial.println(client);
    if (client) {
      // We have a client
      while (client.connected()) {
        if (client.available()) {
          // Read the incoming byte:
          uint8_t cmd_signature = client.read();
          // Serial.println(cmd_signature);
          if (cmd_signature >= CMD_SIZE){
            continue;            
          }
          if(parseCMD(cmd_signature) != 0){
            continue;
          }
        }
        if (millis() - drone.lastHeartbeatTime > drone.heartbeatInterval) {
          drone.alive = false;
        // Heartbeat missed, set Fault Flag
        }
        else{
          if (!drone.batt_low){
            drone.alive = true;
          }
          else{
            drone.alive = false;
          }
        }
        vTaskDelay(pdMS_TO_TICKS(100));
      }
      client.stop(); // Disconnect the client
    }
    else {
      if (millis() - drone.lastHeartbeatTime > drone.heartbeatInterval) {
          drone.alive = false;
        // Heartbeat missed, set Fault Flag
        }
    }
    vTaskDelay(pdMS_TO_TICKS(100));

    // Check if heartbeat interval has elapsed
    
  }
}
