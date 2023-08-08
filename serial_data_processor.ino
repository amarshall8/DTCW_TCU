void processData(){

/*/////////////////////////////
   Temperature Send Enable
/////////////////////////////*/

  if (strcmp(commandFromLCD, "TON") == 0){
    if (integerFromLCD == 1){
      Serial.println("Temp monitoring online!");
      Serial.println();
      avg_temp_send_en = true;
    }
    else if (integerFromLCD == 0){
      Serial.println("Temp monitoring offline!");
      Serial.println();
      avg_temp_send_en = false;
      three_temp_send_en = false;
    }
    else if (integerFromLCD = 2){
      Serial.println("Sending all sensor temps for diagnostics");
      Serial.println();
      three_temp_send_en = true;
    }
  }

/*/////////////////////////////
        Control Enable
/////////////////////////////*/

  else if (strcmp(commandFromLCD, "EN") == 0){
    if (integerFromLCD == 1){
      Serial.print("Temperature control started!");
      Serial.print(" Setpoint value is: ");
      Serial.println(floatFromLCD,1);
      Serial.println();
      dtcwPID.SetMode(MANUAL);
      Output = 0;
      Setpoint = floatFromLCD;
      dtcwPID.SetMode(AUTOMATIC);
      pid_enable = true;
      temp_ctrl_fans_on = true;
    }
    else if (integerFromLCD == 0){
      shutdown:
      Serial.println("Temperature control ended!");
      Serial.println();
      dtcwPID.SetMode(MANUAL);
      Output = 0;
      pid_enable = false;
      Setpoint = 25.0;
      hbControl(0);
      temp_ctrl_fans_on = false;
    }
  }

/*/////////////////////////////
    Calibration Data Send
/////////////////////////////*/

  else if (strcmp(commandFromLCD, "CRCV") == 0 && integerFromLCD == 1){
    Serial.println("Current calibration values are:");
    Serial.print("T1: ");
    Serial.print(Temp1Cal);
    Serial.print(" T2 ");
    Serial.print(Temp2Cal);
    Serial.print(" T3 ");
    Serial.println(Temp3Cal);
    Serial.println();
    Serial.println("Sending calibration values over to LCD!");
    Serial.println();
    send_serial_command("T1CAL", 0, Temp1Cal, true);
    send_serial_command("T2CAL", 0, Temp2Cal, true);
    send_serial_command("T3CAL", 0, Temp3Cal, true);
    send_serial_command("CALF", 0, NULL, true);
  }

/*/////////////////////////////
    Calibration Data Recv
/////////////////////////////*/

  else if (strcmp(commandFromLCD, "T1CAL") == 0 && integerFromLCD == 1){
    Temp1Cal = floatFromLCD;
    Serial.print("Temperature Sensor 1 calibration value is: ");
    Serial.println(Temp1Cal);
    Serial.println();
  }
  else if (strcmp(commandFromLCD, "T2CAL") == 0 && integerFromLCD == 1){
    Temp2Cal = floatFromLCD;
    Serial.print("Temperature Sensor 2 calibration value is: ");
    Serial.println(Temp2Cal);
    Serial.println();
  }
  else if (strcmp(commandFromLCD, "T3CAL") == 0 && integerFromLCD == 1){
    Temp3Cal = floatFromLCD;
    Serial.print("Temperature Sensor 3 calibration value is: ");
    Serial.println(Temp3Cal);
    Serial.println();
  }

/*/////////////////////////////
        PID Data Send
/////////////////////////////*/

  else if (strcmp(commandFromLCD, "PRCV") == 0 && integerFromLCD == 1){
    Serial.println("Current PID values are:");
    Serial.print("P: ");
    Serial.print(Kp);
    Serial.print(" I ");
    Serial.print(Ki);
    Serial.print(" D ");
    Serial.println(Kd);
    Serial.println();
    Serial.println("Sending PID values over to LCD!");
    Serial.println();
    send_serial_command("P", 0, Kp, true);
    send_serial_command("I", 0, Ki, true);
    send_serial_command("D", 0, Kd, true);
    send_serial_command("PIDF", 0, NULL, true);
  }

/*/////////////////////////////
        PID Data Recv
/////////////////////////////*/

    else if (strcmp(commandFromLCD, "P") == 0 && integerFromLCD == 1){
    Kp = floatFromLCD;
    Serial.print("Proportional value is: ");
    Serial.println(Kp);
    Serial.print("PID values updated in main function!");
    Serial.println();
    dtcwPID.SetTunings(Kp, Ki, Kd);
  }
  else if (strcmp(commandFromLCD, "I") == 0 && integerFromLCD == 1){
    Ki = floatFromLCD;
    Serial.print("Integral value is: ");
    Serial.println(Ki);
    Serial.print("PID values updated in main function!");
    Serial.println();
    dtcwPID.SetTunings(Kp, Ki, Kd);
  }
  else if (strcmp(commandFromLCD, "D") == 0 && integerFromLCD == 1){
    Kd = floatFromLCD;
    Serial.print("Derivative value is: ");
    Serial.println(Kd);
    Serial.print("PID values updated in main function!");
    Serial.println();
    dtcwPID.SetTunings(Kp, Ki, Kd);
  }

/*/////////////////////////////
      Fan Manual Control
/////////////////////////////*/
  else if (strcmp(commandFromLCD, "F") == 0){
    fanControl(integerFromLCD);
  }

/*/////////////////////////////
          Heartbeat
/////////////////////////////*/

else if (strcmp(commandFromLCD, "H") == 0 && integerFromLCD == 1){
  send_serial_command("H", 2, NULL, true);
  avg_temp_send_en = false;
  three_temp_send_en = false;
  goto shutdown;
}


/*/////////////////////////////
         EEPROM Save
/////////////////////////////*/

  else if (strcmp(commandFromLCD, "S") == 0 && integerFromLCD == 1){
    tempcalWrite(Temp1Cal, Temp2Cal, Temp3Cal);
    pidWrite(Kp, Ki, Kd);
  }
}