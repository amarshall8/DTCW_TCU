// Function for initializing temperature calibration values from eeprom stored values
void tempcalInit()
{
  //default temp cal values:
  if (EEPROM.read(tempCalState) != 1) { //Check if there is a eeprom cal value set
    EEPROM.updateFloat(T1CalAddr, Temp1Cal); //Set Cal1 Value
    EEPROM.updateFloat(T2CalAddr, Temp2Cal); //Set Cal2 Value
    EEPROM.updateFloat(T3CalAddr, Temp3Cal); //Set Cal3 Value
    EEPROM.update(tempCalState, 1); //if not, note that eeprom cal has been set (1 at position 0)
    Serial.println("Wrote default calibration values!");
    Serial.println();
  }
  else { //Read stored values if there exists previous cal
    Temp1Cal = EEPROM.readFloat(T1CalAddr);
    Temp2Cal = EEPROM.readFloat(T2CalAddr);
    Temp3Cal = EEPROM.readFloat(T3CalAddr);
    Serial.println("Calibration values loaded!");
    Serial.println();
  }
}

void tempcalWrite(float T1, float T2, float T3)
{
  EEPROM.updateFloat(T1CalAddr, T1);
  EEPROM.updateFloat(T2CalAddr, T2);
  EEPROM.updateFloat(T3CalAddr, T3);
  Serial.println("Updated Calibration Values!");
  Serial.println();
}

 //Function to set values for PID from eeprom
void pidWrite(double Kp, double Ki, double Kd)
{
  EEPROM.updateDouble(KpAddr, Kp);
  EEPROM.updateDouble(KiAddr, Ki);
  EEPROM.updateDouble(KdAddr, Kd);
  Serial.println("Updated PID Values!");
  Serial.println();
}


//Function to read PID values from EEPROM and update algorithm
void pidRead()
{
  Kp = EEPROM.readDouble(KpAddr);
  Ki = EEPROM.readDouble(KiAddr);
  Kd = EEPROM.readDouble(KdAddr);
  Serial.println("PID values loaded!");
  Serial.println();
}
