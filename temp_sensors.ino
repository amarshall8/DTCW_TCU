void readTemps(){
  int currentTime = millis() / 750;
  int deltaTime = currentTime - oldTime;
  oldTime = currentTime;
  if (deltaTime >= 1){
    Temp1 = sensors.getTempC(Temp1Addr) + Temp1Cal;
    Temp2 = sensors.getTempC(Temp2Addr) + Temp2Cal;
    Temp3 = sensors.getTempC(Temp3Addr) + Temp3Cal; 
    sensors.requestTemperatures();
    tempAvg = (Temp1 + Temp2 + Temp3)/3.0;
    if (avg_temp_send_en == true){
      send_serial_command("T", 4, tempAvg, true);
      Serial.print("Current Average Temperature: ");
      Serial.println(tempAvg);
      Serial.print("Current PID PWM Value: ");
      Serial.println(Output);
      Serial.print("Current PID Direction: ");
      Serial.println(dtcwPID.GetDirection());
      Input = tempAvg;
    }
    else if (three_temp_send_en == true){
      send_serial_command("T", 1, Temp1, true);
      send_serial_command("T", 2, Temp2, true);
      send_serial_command("T", 3, Temp3, true);
      Serial.print("Current Diagnostic Temperatures: A: ");
      Serial.print(Temp1);
      Serial.print(" B: ");
      Serial.print(Temp2);
      Serial.print(" C: ");
      Serial.println(Temp3);
    }
    //printTemps();
    deltaTime = 0;
  }
}
