// Example 5 - Receive with start- and end-markers combined with parsing
#include <SoftwareSerial.h>
#include <EEPROMex.h>
#include <PID_v1.h>

/*////////////////////////////
        Serial Setup
////////////////////////////*/

const byte numChars = 32;
char receivedChars[numChars];
char tempChars[numChars];        // temporary array for use when parsing

      // variables to hold the parsed data
char commandFromLCD[numChars] = {0};
int integerFromLCD = 0;
float floatFromLCD = 0.0;

SoftwareSerial tempBoard(7, 8); // RX, TX

/*///////////////////////////////////
    PID/Temperature Sensor Setup
///////////////////////////////////*/

// Data wire is plugged into port 2 on the Arduino
#define ONE_WIRE_BUS 2

// Thermistor sensor cold side definitions

// which analog pin to connect
#define THERMISTORPIN A7         
// resistance at 25 degrees C
#define THERMISTORNOMINAL 100000      
// temp. for nominal resistance (almost always 25 C)
#define TEMPERATURENOMINAL 25   
// how many samples to take and average, more takes longer
// but is more 'smooth'
#define NUMSAMPLES 6
// The beta coefficient of the thermistor (usually 3000-4000)
#define BCOEFFICIENT 3950
// the value of the 'other' resistor
#define SERIESRESISTOR 100000    

int samples[NUMSAMPLES];

// Thermistor sensor hot side definitions

// Average NTC Thermistor value
float average_NTC_temp;

// which analog pin to connect
#define THERMISTORPIN_hot A6         
// resistance at 25 degrees C
#define THERMISTORNOMINAL_hot 10000        
// the value of the 'other' resistor
#define SERIESRESISTOR_hot 10000    

int samples_hot[NUMSAMPLES];

// Average NTC Thermistor value
float average_hot_NTC_temp;

// Half bridge wire definitions
#define POS_PWM 5
#define NEG_PWM 6
#define POS_EN 11
#define NEG_EN 12

// Fan wire definition
#define FAN_PIN 10

//Temp calibration EEPROM addresses
#define tempCalState 0
#define TN_bottomCalAddr 1
#define TN_topCalAddr 5

//PID parameter EEPROM addresses
#define KpAddr 13
#define KiAddr 21
#define KdAddr 29


bool avg_temp_send_en = false;

int temp3_fan_speed = 0;

bool two_temp_send_en = false;

//sensor timing variables
int oldTime = 0;

//default temperature calibration values (fixed at code upload)
float TempN_bottomCal = 0;
float TempN_topCal = 0;

bool TempN_bottomCal_recieved = false;
bool TempN_topCal_recieved = false;


//PID Setup
//Define Variables for PID loop
double Setpoint, Input, Output;
//Specify links and initial tuning params
double Kp = 2;
double Ki = 5;
double Kd = 1;

//Setup PID
PID dtcwPID(&Input, &Output, &Setpoint,Kp,Ki,Kd, DIRECT); //P_ON_M specifies that Proportional on Measurement be used

bool pid_enable = false;
bool fan_manual_control = false;

//============

void(* resetFunc) (void) = 0;//declare reset function at address 0


void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  analogReference(EXTERNAL);
  hbInit();
  fanInit();
  fanControl(0);
  Serial.begin(57600);
  Serial.println("Recieving Arduino Online");
  Serial.println();
  delay(750);

  dtcwPID.SetOutputLimits(-255, 255);

  tempcalInit();
  pidRead();

  tempBoard.begin(4800);
}

//============

void loop() {
    recvWithStartEndMarkers();
    readTemps();
    //Serial.println(average_NTC_temp);
    if (pid_enable == true){
        dtcwPID.SetControllerDirection(DIRECT);
        dtcwPID.Compute();
        hbControl(Output);
        fanControl(abs(Output));
        digitalWrite(LED_BUILTIN, HIGH);
    }
    else if(fan_manual_control == false) {
      fanControl(average_hot_NTC_temp - 20 + 80);
      digitalWrite(LED_BUILTIN, LOW);
    }
    if (average_hot_NTC_temp > 70 || average_NTC_temp > 70){
      pid_enable == false;
      hbControl(0);
    }
}