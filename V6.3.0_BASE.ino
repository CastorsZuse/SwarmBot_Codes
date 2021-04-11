// SWARM BOTS: 
// MULTI PLAYER RECEIVER CODE
// BUMP/NOPE CODE 
// UPDATED TO SWICH/CASE FORMAT 
// TIMER FUNCTION
// UPDATED TO 3.1 irRemote LIB
// Control IR numbers 3.0 UPDATE 
// zappedFlag UPDATE
// BOUNDARY DETECTION UPGRADE 
// UPDATED DEBUG IN SERIAL MONITOR
// ledRED DEBUG OPTION UPDATE IN SETUP
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
//
// CONCEPT AND DESIGN BY: Nolan Cash
// WRITEN BY: Nolan Cash
// CoWRITEN BY: Andy Tracy
// SPECIAL THANKS TO:
//      Justus Stahler
//      Christian Restifo
//      CW Kreimer
//      Steve Owens 
// See SwarmBots.online for more info // 
//
////////////////////////////////////////// NOTES //////////
///////////////////////////////////////////////////////////
//
// This code is the curent base code with working list above
//  current issues:

//      1: needs blink without delay to happen on ledRED 
//         after timeout_ms, till iRIN_botACTIVATION is receved 
//            FIX: unknown 
//
//
//    4.5.21 STATUS: 
//        line following ir sensors are working with
//        a 10-15mm range.       
//        function triggers on white not black.
//
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
//
//Pin numbers definition
// Left motor pins
const int motorEnableLeft = 9;
const int motorForwardLeft = 7;
const int motorBackLeft = 8;
// Right motor pins
const int motorEnableRight = 11;
const int motorForwardRight = 12;
const int motorBackRight = 10;
// Ultrasonic sensor pins
const int trigPinFront = A1;
const int echoPinFront = 2;
const int trigPinLeft = 3;
const int echoPinLeft = 4;
const int trigPinRight = 5;
const int echoPinRight = 6;
// iR receiver pin
const int irPin = A0;
// iR input pins
const int boundaryDetectionLEFT_pin = A4;
const int boundaryDetectionRIGHT_pin = A5;
//RGB LED pins
const int LEDred = 13;
const int LEDgreen = A2;
const int LEDblue = A3;

// Veritables for color set
enum Color : int {
  RED   = 1,
  GREEN = 2,
  BLUE  = 4
};

// Veritables for iR barrier detectors
const int THRESHOLD = 900; 
int boundaryDetectionLEFT_STATE;
int boundaryDetectionRIGHT_STATE;

//Variables for the Motors
const int leftMotorSpeed = 255;
const int rightMotorSpeed = 255;
const int delayTime = 150;

//Variables for Ultrasonic Sensors
long durationFront;
int distanceFront;
long durationLeft;
int distanceLeft;
long durationRight;
int distanceRight;
const int minFrontDistance = 30;
const int minSideDistance = 20;
const int stuckDistance = 10;

//Variables for IR Receiver
#define DECODE_NEC
#include <IRremote.h>
unsigned long current_code = 0;

// Variables for flags
boolean runFlag = false;
boolean zappedFlag = true;
// true = 1 || false = 0
// compare using ( >=< ) 

// Veritables for the TIMER
unsigned long activationTime = 0;
unsigned long timeout_ms = 30000;   // 1000 = 1 second

//Control IR numbers 3.0 UPDATE
const long iRIN_botACTIVATION = 3409232445;
const long iRIN_botSTOP_R =     3409195215;
const long iRIN_botSTOP_G =     3409214085;
const long iRIN_botSTOP_B =     3409189095;
const long iRIN_botSTOP_RB =    3409187055;
const long iRIN_botSTOP_RG =    3409197255;
const long iRIN_botSTOP_BG =    3409205925;
const long iRIN_botBumpLEFT =   3409224285;
const long iRIN_botSTALL =      3409207965;
const long iRIN_botBumpRIGHT =  3409240605;
const long iRIN_botNOPEleft =   3409240095;
const long iRIN_botNOPEback  =  3409225815;
const long iRIN_botNOPEright =  3409219695;

// Veritables for setting leds 
void setLEDs(int colorValue) {  
  digitalWrite(LEDblue, BLUE & colorValue);
  digitalWrite(LEDgreen, GREEN & colorValue);
  digitalWrite(LEDred, RED & colorValue);
}

void stopAndSetLEDs(int colorValue) {
  stop();
  setLEDs(colorValue);
}

void stop() {
  digitalWrite(motorForwardLeft, LOW);
  digitalWrite(motorBackLeft, LOW);
  digitalWrite(motorForwardRight, LOW);
  digitalWrite(motorBackRight, LOW);
  analogWrite(motorEnableLeft, 0);
  analogWrite(motorEnableRight, 0);
  runFlag = false;
  zappedFlag = true;
}

void BOT_ForwardFull () {
  digitalWrite(motorForwardLeft, HIGH);
  digitalWrite(motorBackLeft, LOW);
  digitalWrite(motorForwardRight, HIGH);
  digitalWrite(motorBackRight, LOW);
  analogWrite(motorEnableLeft, leftMotorSpeed);
  analogWrite(motorEnableRight, rightMotorSpeed);
  setLEDs(RED | GREEN | BLUE);
}

void BOT_Left () {
  digitalWrite(motorForwardLeft, LOW);
  digitalWrite(motorBackLeft, LOW);
  digitalWrite(motorForwardRight, HIGH);
  digitalWrite(motorBackRight, LOW);
  analogWrite(motorEnableLeft, 0);
  analogWrite(motorEnableRight, rightMotorSpeed);
  setLEDs(GREEN);
}

void BOT_Right () {
  digitalWrite(motorForwardLeft, HIGH);
  digitalWrite(motorBackLeft, LOW);
  digitalWrite(motorForwardRight, LOW);
  digitalWrite(motorBackRight, LOW);
  analogWrite(motorEnableLeft, leftMotorSpeed);
  analogWrite(motorEnableRight, 0);
  setLEDs(GREEN);
}

void BOT_Back () {
  digitalWrite(motorForwardLeft, LOW);
  digitalWrite(motorBackLeft, HIGH);
  digitalWrite(motorForwardRight, LOW);
  digitalWrite(motorBackRight, HIGH);
  analogWrite(motorEnableLeft, leftMotorSpeed);
  analogWrite(motorEnableRight, rightMotorSpeed);
  setLEDs(BLUE);
}

void BOT_NOPE_LEFT () {
  digitalWrite(motorForwardLeft, LOW);
  digitalWrite(motorBackLeft, HIGH);
  digitalWrite(motorForwardRight, HIGH);
  digitalWrite(motorBackRight, LOW);
  analogWrite(motorEnableLeft, leftMotorSpeed);
  analogWrite(motorEnableRight, rightMotorSpeed);
  setLEDs(RED | GREEN);
}

void BOT_NOPE_BACK () {
  digitalWrite(motorForwardLeft, LOW);
  digitalWrite(motorBackLeft, HIGH);
  digitalWrite(motorForwardRight, HIGH);
  digitalWrite(motorBackRight, LOW);
  analogWrite(motorEnableLeft, leftMotorSpeed);
  analogWrite(motorEnableRight, rightMotorSpeed);
  setLEDs(RED | BLUE);
}

void BOT_NOPE_RIGHT () {
  digitalWrite(motorForwardLeft, HIGH);
  digitalWrite(motorBackLeft, LOW);
  digitalWrite(motorForwardRight, LOW);
  digitalWrite(motorBackRight, HIGH);
  analogWrite(motorEnableLeft, leftMotorSpeed);
  analogWrite(motorEnableRight, rightMotorSpeed);
  setLEDs(RED | GREEN);
}

void BOT_ObstacleAvoidance (){
             BOT_ForwardFull();
             sensorRead ();

////////////////////////////////////////////////////////////////////
/////////////// LINE FOLLOWING SENSOR AI TREE /////////////////////
///////////////////////////////////////////////////////////////////

//        (boundaryDetectionRIGHT_STATE > THRESHOLD && 
//          boundaryDetectionLEFT_STATE < THRESHOLD){
//          if the right sensor is high
//             and left sensor is low
//             bot will turn right 
//          function triggetrs on white 
//          delay 2 x 150
////////////////////////////////////////////////////////////////

          boundaryDetectionLEFT_STATE = analogRead(boundaryDetectionLEFT_pin);
          boundaryDetectionRIGHT_STATE = analogRead(boundaryDetectionRIGHT_pin);

      if(boundaryDetectionRIGHT_STATE > THRESHOLD && 
          boundaryDetectionLEFT_STATE < THRESHOLD){
        Serial.println("iR BOUNDARY DETECTED LEFT, TURN RIGHT");
           BOT_Right();
             delay(2*delayTime);
  }  
      if(boundaryDetectionRIGHT_STATE < THRESHOLD && 
         boundaryDetectionLEFT_STATE > THRESHOLD){
        Serial.println("iR BOUNDARY DETECTED RIGHT, TURN LEFT");
           BOT_Left();
            delay(2*delayTime);
   }
      if(boundaryDetectionRIGHT_STATE < THRESHOLD && 
         boundaryDetectionLEFT_STATE < THRESHOLD) { 
        Serial.println("iR BOUNDARY DETECTED FRONT, TURN AROUND");
          BOT_NOPE_BACK();
            delay(800);
  }

////////////////////////////////////////////////////////////////////
/////////////// LINE FOLLOWING SENSOR AI TREE /////////////////////
///////////////////////////////////////////////////////////////////
  
  if ((distanceFront <= minFrontDistance) ||
      (distanceLeft <= minSideDistance) ||
      (distanceRight <= minSideDistance)) {
    if ((distanceLeft < stuckDistance) ||
        (distanceRight < stuckDistance) ||
        (distanceFront < stuckDistance)) {
         Serial.println("U/S ALL SENSORS, OBSTACLE DANGEROUSLY CLOSE! BACK UP!");
         BOT_Back();
         delay(1.5*delayTime);      
   } 
    else if ((distanceFront <= minFrontDistance) &&
               (distanceLeft <= minSideDistance) &&
               (distanceRight <= minSideDistance)) {
                Serial.println("U/S ALL SENSORS OBSTACLE TOO CLOSE. BACK UP");
                BOT_Back();
                delay(1.5*delayTime);    
   } 
    else if (distanceLeft > distanceRight ) {
               Serial.println("U/S OBSTACLE RIGHT, TURNING LEFT");
               BOT_Left();
               delay(delayTime);
               
    }
    else if (distanceLeft <= distanceRight) {
              Serial.println("U/S OBSTACLE LEFT, TURN RIGHT");
              BOT_Right();
              delay(delayTime);
    }
  }
}

void sensorRead () {
  //Read front sensor value
  digitalWrite(trigPinFront, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPinFront, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPinFront, LOW);
  durationFront = pulseIn(echoPinFront, HIGH);
  distanceFront = durationFront * 0.034 / 2;
  
  //Read left sensor value
  digitalWrite(trigPinLeft, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPinLeft, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPinLeft, LOW);
  durationLeft = pulseIn(echoPinLeft, HIGH);
  distanceLeft = durationLeft * 0.034 / 2;
 
  //Read right sensor value
  digitalWrite(trigPinRight, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPinRight, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPinRight, LOW);
  durationRight = pulseIn(echoPinRight, HIGH);
  distanceRight = durationRight * 0.034 / 2;

//  Serial.print("Left Sensor: ");
//  Serial.println(distanceLeft);
//  Serial.print("Right Sensor: ");
//  Serial.println(distanceRight);
//  Serial.print("Front Sensor: ");
//  Serial.println(distanceFront);
}

void setup() {
////////////////////////////////////////////////////
/////////////////////////// DEBUG NOTE /////////////
//         for debugging led feeback, comment out...

  IrReceiver.begin(irPin, ENABLE_LED_FEEDBACK);
  IrReceiver.begin(irPin); // <-- THIS LINE

// WARNING! ledRED WILL NOT WORK IN DEBUG MODE // 
///////////////////////////////////////////////////  

  pinMode(motorEnableLeft, OUTPUT);
  pinMode(motorForwardLeft, OUTPUT);
  pinMode(motorBackLeft, OUTPUT);
  pinMode(motorEnableRight, OUTPUT);
  pinMode(motorForwardRight, OUTPUT);
  pinMode(motorBackRight, OUTPUT);
  pinMode(trigPinFront, OUTPUT);
  pinMode(echoPinFront, INPUT);
  pinMode(trigPinLeft, OUTPUT);
  pinMode(echoPinLeft, INPUT);
  pinMode(trigPinRight, OUTPUT);
  pinMode(echoPinRight, INPUT);
  pinMode(LEDred, OUTPUT);
  pinMode(LEDgreen, OUTPUT);
  pinMode(LEDblue, OUTPUT);
  
  IrReceiver.enableIRIn();
  Serial.begin(9600);
}

void loop() {
  if (runFlag && (millis() - activationTime) > timeout_ms) {
        stopAndSetLEDs(RED | GREEN | BLUE);
  }

  if (IrReceiver.decode()){
    current_code = IrReceiver.decodedIRData.decodedRawData;
    Serial.print("New code received: ");
    Serial.println(current_code);
    IrReceiver.resume();        


    switch (current_code) { 
      case iRIN_botACTIVATION:
        Serial.println("BOT ACTIVATION");
      zappedFlag = false;
        runFlag = true;
        activationTime = millis();
        break;

      case iRIN_botSTOP_R:
       if (runFlag > zappedFlag) {
        Serial.println("botSTOP_RED");
        stopAndSetLEDs(RED);
        delay(50);
       }
        break;
        
      case iRIN_botSTOP_G:
       if (runFlag > zappedFlag) {
        Serial.println("botSTOP_GREEN");
        stopAndSetLEDs(GREEN);
       }
        break;
        
      case iRIN_botSTOP_B:
       if (runFlag > zappedFlag) {
        Serial.println("botSTOP_BLUE");
        stopAndSetLEDs(BLUE);
       }
        break;
        
      case iRIN_botSTOP_RB:
       if (runFlag > zappedFlag) {
        Serial.println("botSTOP_RED_BLUE");
        stopAndSetLEDs(RED | BLUE);
       }
        break;
        
      case iRIN_botSTOP_RG:
       if (runFlag > zappedFlag) {
        Serial.println("botSTOP_RED_GREEN");
        stopAndSetLEDs(RED | GREEN);
       }
        break;
        
      case iRIN_botSTOP_BG:
       if (runFlag > zappedFlag) {
        Serial.println("botSTOP_BLUE_GREEN");
        stopAndSetLEDs(BLUE | GREEN);
       }
        break;

      case iRIN_botBumpLEFT:
        if (runFlag > zappedFlag) {  
          Serial.println("BOT_BUMP_LEFT");
          BOT_Left();
          delay(500);
        }
        break;
        
      case iRIN_botBumpRIGHT:
        if (runFlag > zappedFlag) {
          Serial.println("BOT_BUMP_RIGHT");
          BOT_Right();
          delay(500);
        }
        break;

      case iRIN_botSTALL:
        if (runFlag > zappedFlag) {
          Serial.println("BOT_STALL");
          BOT_Back();
          delay(500);
        }
        break;

      case iRIN_botNOPEleft:
        if (runFlag > zappedFlag) {
          Serial.println("NOPE_LEFT");
          BOT_NOPE_LEFT();
          delay (350);
        }
        break;

      case iRIN_botNOPEback:
        if (runFlag > zappedFlag) {
          Serial.println("NOOOOPE");
          BOT_NOPE_BACK();
          delay (800);
        }
        break;

      case iRIN_botNOPEright:
        if (runFlag > zappedFlag) {
          Serial.println("NOPE_RIGHT");
          BOT_NOPE_RIGHT();
          delay (350);
        }
        break;

      default:
        break;
    }
  }

  if ( runFlag ) {
    BOT_ObstacleAvoidance();
  }
  }
