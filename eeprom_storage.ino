// Function for initializing temperature calibration values from eeprom stored values
void tempcalInit()
{
  //default temp cal values:
  if (EEPROM.read(tempCalState) != 1) { //Check if there is a eeprom cal value set
    EEPROM.updateFloat(TN_bottomCalAddr, TempN_bottomCal); //Set Cal1 Value
    EEPROM.updateFloat(TN_topCalAddr, TempN_topCal); //Set Cal2 Value
    EEPROM.update(tempCalState, 1); //if not, note that eeprom cal has been set (1 at position 0)
    Serial.println("Wrote default calibration values!");
    Serial.println();
  }
  else { //Read stored values if there exists previous cal
    TempN_bottomCal = EEPROM.readFloat(TN_bottomCalAddr);
    TempN_topCal = EEPROM.readFloat(TN_topCalAddr);
    Serial.println("Calibration values loaded!");
    Serial.println();
  }
}

void tempcalWrite(float TNB, float TNT)
{
  EEPROM.updateFloat(TN_bottomCalAddr, TNB);
  EEPROM.updateFloat(TN_topCalAddr, TNT);
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
