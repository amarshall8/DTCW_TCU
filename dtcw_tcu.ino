// Example 5 - Receive with start- and end-markers combined with parsing
#include <SoftwareSerial.h>
#include <OneWire.h>
#include <DallasTemperature.h>
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

// Half bridge wire definitions
#define POS_PWM 5
#define NEG_PWM 6
#define POS_EN 11
#define NEG_EN 12

// Fan wire definition
#define FAN_PIN 10

//Temp calibration EEPROM addresses
#define tempCalState 0
#define T1CalAddr 1
#define T2CalAddr 5
#define T3CalAddr 9

//PID parameter EEPROM addresses
#define KpAddr 13
#define KiAddr 21
#define KdAddr 29

// Set value for temperature sensor bit resolution
int TEMP_PRECISION = 12;

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);

float tempAvg; //average cold side temperature for PID loop algorithm
bool avg_temp_send_en = false;

//sensor value variables
float Temp1, Temp2, Temp3;
bool three_temp_send_en = false;

//sensor timing variables
int oldTime = 0;

//default temperature calibration values (fixed at code upload)
float Temp1Cal = 0;
float Temp2Cal = 0;
float Temp3Cal = 0;

bool Temp1Cal_recieved = false;
bool Temp2Cal_recieved = false;
bool Temp3Cal_recieved = false;

//PID Setup
//Define Variables for PID loop
double Setpoint, Input, Output;
//Specify links and initial tuning params
double Kp = 2;
double Ki = 5;
double Kd = 1;

//Setup PID
PID dtcwPID(&Input, &Output, &Setpoint,Kp,Ki,Kd,P_ON_M, DIRECT); //P_ON_M specifies that Proportional on Measurement be used

//Temperature sensor device addresses
DeviceAddress Temp1Addr = { 0x28, 0xA2, 0x57, 0x13, 0x0D, 0x00, 0x00, 0x48 }; //Temp1 device address
DeviceAddress Temp2Addr = { 0x28, 0xFD, 0xF9, 0x11, 0x0D, 0x00, 0x00, 0xD4 }; //Temp2 device address
DeviceAddress Temp3Addr = { 0x28, 0xC7, 0x70, 0x13, 0x0D, 0x00, 0x00, 0xC9 }; //Temp3 device address

bool pid_enable = false;
bool temp_ctrl_fans_on = false;

//============

void(* resetFunc) (void) = 0;//declare reset function at address 0


void setup() {
    hbInit();
    fanInit();
    Serial.begin(57600);
    Serial.println("Recieving Arduino Online");
    Serial.println();
    delay(750);
    sensors.begin();
    //set the resolution
    sensors.setResolution(Temp1Addr, TEMP_PRECISION);
    sensors.setResolution(Temp2Addr, TEMP_PRECISION);
    sensors.setResolution(Temp3Addr, TEMP_PRECISION);
    sensors.setWaitForConversion(false);

    dtcwPID.SetOutputLimits(-255, 255);

    tempcalInit();
    pidRead();

    tempBoard.begin(4800);
}

//============

void loop() {
    recvWithStartEndMarkers();
    readTemps();
    if (pid_enable == true){
      dtcwPID.SetControllerDirection(REVERSE);
      dtcwPID.Compute();
      hbControl(Output);
      Serial.println(Output);
    }
}