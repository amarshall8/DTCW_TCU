void readTemps(){
  //NTC thermistor top variables:
  uint8_t i;
  float average_NTC_val;

  //NTC thermistor bottom variables:
  uint8_t i_hot;
  float average_hot_NTC_val;

  int currentTime = millis() / 750;
  int deltaTime = currentTime - oldTime;
  oldTime = currentTime;
  if (deltaTime >= 1){
    if (avg_temp_send_en == true){
      send_serial_command("T", 2, average_NTC_temp, true);
      Serial.print("Current Average Temperature: ");
      Serial.println(average_NTC_temp);
      Serial.print("Current PID PWM Value: ");
      Serial.println(Output);
    }
    else if (two_temp_send_en == true){
      send_serial_command("T", 1, average_hot_NTC_temp, true);
      send_serial_command("T", 2, average_NTC_temp, true);
      Serial.print("Current Diagnostic Temperatures:");
      Serial.print(" NTC_bottom: ");
      Serial.println(average_hot_NTC_temp);
      Serial.print(" NTC_top: ");
      Serial.println(average_NTC_val);
    }
    deltaTime = 0;
  }

  // Top NTC Code

  for (i=0; i< NUMSAMPLES; i++) {
   samples[i] = analogRead(THERMISTORPIN);
   delay(10);
  }
 
  // average all the samples out
  average_NTC_val = 0;
  for (i=0; i< NUMSAMPLES; i++) {
     average_NTC_val += samples[i];
  }
  average_NTC_val /= NUMSAMPLES;

  // convert the value to resistance
  average_NTC_val = 1023 / average_NTC_val - 1;
  average_NTC_val = SERIESRESISTOR / average_NTC_val;

  // Steinhart conversion
  average_NTC_temp = average_NTC_val / THERMISTORNOMINAL;     // (R/Ro)
  average_NTC_temp = log(average_NTC_temp);                  // ln(R/Ro)
  average_NTC_temp /= BCOEFFICIENT;                   // 1/B * ln(R/Ro)
  average_NTC_temp += 1.0 / (TEMPERATURENOMINAL + 273.15); // + (1/To)
  average_NTC_temp = 1.0 / average_NTC_temp;                 // Invert
  average_NTC_temp -= 273.15;                         // convert absolute temp to C
  average_NTC_temp += TempN_topCal;
  
  Input = average_NTC_temp;

  // Bottom NTC Code

  for (i_hot=0; i_hot< NUMSAMPLES; i_hot++) {
   samples_hot[i_hot] = analogRead(THERMISTORPIN_hot);
   delay(10);
  }
 
  // average all the samples out
  average_hot_NTC_val = 0;
  for (i_hot=0; i_hot< NUMSAMPLES; i_hot++) {
     average_hot_NTC_val += samples_hot[i_hot];
  }
  average_hot_NTC_val /= NUMSAMPLES;

  // convert the value to resistance
  average_hot_NTC_val = 1023 / average_hot_NTC_val - 1;
  average_hot_NTC_val = SERIESRESISTOR_hot / average_hot_NTC_val;

  // Steinhart conversion
  average_hot_NTC_temp = average_hot_NTC_val / THERMISTORNOMINAL_hot;     // (R/Ro)
  average_hot_NTC_temp = log(average_hot_NTC_temp);                  // ln(R/Ro)
  average_hot_NTC_temp /= BCOEFFICIENT;                   // 1/B * ln(R/Ro)
  average_hot_NTC_temp += 1.0 / (TEMPERATURENOMINAL + 273.15); // + (1/To)
  average_hot_NTC_temp = 1.0 / average_hot_NTC_temp;                 // Invert
  average_hot_NTC_temp -= 273.15;                         // convert absolute temp to C
  average_hot_NTC_temp += TempN_bottomCal;
  
}
