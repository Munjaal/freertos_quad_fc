void initSemph()
{
  sem_i2c = xSemaphoreCreateMutex();
  sem_motor = xSemaphoreCreateMutex();
  sem_ahrs = xSemaphoreCreateMutex();
  sem_batt = xSemaphoreCreateMutex();
  sem_cmd = xSemaphoreCreateMutex();
  sem_wifi = xSemaphoreCreateMutex();
}

void attachMotors()
{
    for(int i = 0; i < 4; i++)
  {
    drone.motor[i].attach(drone.mPins[i]);
    drone.motor[i].writeMicroseconds(MOTOR_MIN_THROTTLE);
  }
}

void initLidar()
{
  if (!lox.init()) {
  // Serial.println(F("Failed to boot VL53L0X"));
    while(1);
  }
}



void initIMU()
{
  if (!bno08x.begin_I2C()) {
  //if (!bno08x.begin_UART(&Serial1)) {  // Requires a device with > 300 byte UART buffer!
  //if (!bno08x.begin_SPI(BNO08X_CS, BNO08X_INT)) {
    Serial.println("Failed to find BNO08x chip, retrying.");
    while (!bno08x.begin_I2C()) { 
      delay(500); 
      Serial.print('.');
    }
  }
  Serial.println("BNO08x Found!");
  sh2_SensorId_t reportType = SH2_ARVR_STABILIZED_RV;
  long reportIntervalUs = 10000;
  // if (! bno08x.enableReport(reportType, reportIntervalUs)) {
  //   Serial.println("Could not enable stabilized remote vector");
  // }

  sh2_SensorId_t reportType2 = SH2_GYROSCOPE_CALIBRATED;
  if (! bno08x.enableReport(reportType2, reportIntervalUs)) {
    Serial.println("Could not enable stabilized gyro vector");
  }
  // setReports(reportType, reportIntervalUs);
}

void initWiFi()
{
  WiFi.begin(STASSID, STAPSK);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(500);
  }
  server.begin();
}