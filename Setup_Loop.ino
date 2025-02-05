
void setup() {
  // Initialize serial for output
  Serial.begin();
  
  pinMode(23,OUTPUT);
  digitalWrite(23,HIGH);  // LOW Efficiency, LOW Noise Mode (PWM)

  delay(ESC_Calib_Delay);
  Serial.println(REFRESH_INTERVAL);

  // Serial.println("Initializing ESC");

  // ATTACH MOTORS
  attachMotors();
  initSemph();

  delay(7000);

  // Serial.println("Connecting to WiFi");

  initWiFi();

  Serial.println(WiFi.localIP());

  // Serial.println("Initializing IMU");

  initIMU();

  // Serial.println("Initializing Lidar");
  initLidar();

  // Serial.print("KP");
  // Serial.print('\t');
  // Serial.print("KI");
  // Serial.print('\t');
  // Serial.print("KD");
  // Serial.println();

  // Serial.print(drone.Kp);
  // Serial.print('\t');
  // Serial.print(drone.Ki);
  // Serial.print('\t');
  // Serial.print(drone.Kd);

  // Serial.println();
  

  //
  // TASKS
  // Serial.println("Starting Tasks");


  xTaskCreate(IMUReadAndFusion, // Task function
            "IMUReadAndFusion", // Task name
            1024,               // Stack size (adjust as necessary)
            NULL,               // Task input parameter
            1,                  // Priority
            NULL);              // Task handle

  xTaskCreate(heartBeat,
            "heartBeat",
            1024,
            NULL,
            1,
            NULL);

  xTaskCreate(motorControl,
            "motorControl",
            1024,
            NULL,
            1,
            NULL);

  xTaskCreate(controller_PID,
            "pidControl",
            1024,
            NULL,
            1,
            NULL);

  xTaskCreate(getBattLvl,
            "getBattLvl",
            1024,
            NULL,
            1,
            NULL);
  
  xTaskCreate(logData,         
            "logData",         
            1024,              
            NULL,              
            1,                  
            NULL);    

  xTaskCreate(commandHandler,
            "commandHandler",
            1024,
            NULL,
            1,
            NULL);          


  xTaskCreate(getAlt,
            "getAlt",
            512,
            NULL,
            1,
            NULL);

}

void loop() {
  //Keep Empty
}