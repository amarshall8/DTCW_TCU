// Simple function to set all Half-Bridge pins as outputs
void hbInit()
{
  pinMode(POS_PWM, OUTPUT);
  pinMode(NEG_PWM, OUTPUT);
  pinMode(POS_EN, OUTPUT);
  pinMode(NEG_EN, OUTPUT);
}


// Half-Bridge control function
// POL = false for positive output (hot), true for negative output (cold)
void hbControl(double PWM)
{
  if (PWM == 0)
  {
    digitalWrite(NEG_EN, LOW);
    digitalWrite(POS_EN, LOW);
    digitalWrite(NEG_PWM, LOW);
    digitalWrite(POS_PWM, LOW);
  }
  else if(PWM > 0)
  {
    // Disable negative side of H-Bridge
    digitalWrite(NEG_EN, HIGH);
    digitalWrite(POS_EN, HIGH);
    digitalWrite(NEG_PWM, LOW);
    // Enable and set value for positive side of H-Bridge
    analogWrite(POS_PWM, PWM);
  }
  else
  {
    // Disable positive side of H-Bridge
    digitalWrite(POS_EN, HIGH);
    digitalWrite(NEG_EN, HIGH);
    digitalWrite(POS_PWM, LOW);
    // Enable and set value for negative side of H-Bridge
    analogWrite(NEG_PWM, abs(PWM));
  }
}

// Fan pin setup function
void fanInit()
{
  pinMode(FAN_PIN, OUTPUT);
}

// Fan control function
void fanControl(int fanSpeed){
  if (fanSpeed == 0){
    digitalWrite(FAN_PIN, LOW);
  }
  else if (fanSpeed <= 255 && fanSpeed > 0){
    analogWrite(FAN_PIN, fanSpeed);
  }
}