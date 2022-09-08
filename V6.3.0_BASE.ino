// SWARM BOTS: 
// MULTI PLAYER RECEIVER CODE
// BUMP/NOPE CODE 
// UPDATED TO SWICH/CASE FORMAT 
// TIMER FUNCTION
// UPDATED TO 3.1 irRemote LIB
// Control IR numbers 3.0 UPDATE 
// zappedFlag UPDATE
// UPDATED DEBUG IN SERIAL MONITOR
// ledRED DEBUG OPTION UPDATE IN SETUP
// BLUETOOTH APP CONTROL
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
//
// CONCEPT AND DESIGN BY: Nolan Cash
// WRITEN BY: Nolan Cash
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

// RGB LED pins
const int LEDred = 13;
const int LEDgreen = A2;
const int LEDblue = A3;

// Neopixel Ring On Off pin
const int NEOIO = A4;

// Veritables for color set
enum Color : int {
  RED   = 1,
  GREEN = 2,
  BLUE  = 4
};

// Veriables for bluetooth
  const int BTState = 2;
  int state;

// Veritables for iR barrier detectors
const int THRESHOLD = 800; 
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
unsigned long timeout_ms = 90000;   // 1000 = 1 second

////Control IR numbers 3.0 UPDATE
//const long iRIN_botACTIVATION = 3409232445;
//const long iRIN_botSTOP_R =     3409195215;
//const long iRIN_botSTOP_G =     3409214085;
//const long iRIN_botSTOP_B =     3409189095;
//const long iRIN_botSTOP_RB =    3409187055;
//const long iRIN_botSTOP_RG =    3409197255;
//const long iRIN_botSTOP_BG =    3409205925;
//const long iRIN_botBumpLEFT =   3409224285;
//const long iRIN_botSTALL =      3409207965;
//const long iRIN_botBumpRIGHT =  3409240605;
//const long iRIN_botNOPEleft =   3409240095;
//const long iRIN_botNOPEback  =  3409225815;
//const long iRIN_botNOPEright =  3409219695;

////Control IR numbers TEST REMOTE SILVER
//const long iRIN_botACTIVATION = 3158572800;
//const long iRIN_botSTOP_R =     4077715200;
//const long iRIN_botSTOP_G =     3877175040;
//const long iRIN_botSTOP_B =     2707357440;
//const long iRIN_botSTOP_RB =    4144561920;
//const long iRIN_botSTOP_RG =    3810328320;
//const long iRIN_botSTOP_BG =    2774204160;
//const long iRIN_botBumpLEFT =   3125149440;
//const long iRIN_botSTALL =      3108437760;
//const long iRIN_botBumpRIGHT =  3091726080;
//const long iRIN_botNOPEleft =   4161273600;
//const long iRIN_botNOPEback  =  3927310080;
//const long iRIN_botNOPEright =  4127850240;

//Control IR numbers TEST REMOTE BLACK
const long iRIN_botACTIVATION = 3125149440; // POWER
const long iRIN_botSTOP_R =     3091726080; // STOP
const long iRIN_botSTOP_G =     4077715200; // ONE
const long iRIN_botSTOP_B =     3877175040; // TWO
const long iRIN_botSTOP_RB =    2707357440; // THREE
const long iRIN_botSTOP_RG =    4144561920; // FOUR
const long iRIN_botSTOP_BG =    3810328320; // FIVE
const long iRIN_botBumpLEFT =   3141861120; // |<<
const long iRIN_botSTALL =      3208707840; // >||
const long iRIN_botBumpRIGHT =  3158572800; // >>|
const long iRIN_botNOPEleft =   4161273600; // UP
const long iRIN_botNOPEback  =  3927310080; // VOL-
const long iRIN_botNOPEright =  4127850240; // DOWN


// Veritables for setting leds 
void setLEDs(int colorValue, bool neoValue) {
  digitalWrite(LEDblue, BLUE & colorValue);
  digitalWrite(LEDgreen, GREEN & colorValue);
  digitalWrite(LEDred, RED & colorValue);
  digitalWrite(NEOIO, neoValue);
}

void stopAndSetLEDs(int colorValue) {
  stop();
  setLEDs(colorValue, false);
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
  setLEDs(RED | GREEN | BLUE, true);
}

void BOT_Left () {
  digitalWrite(motorForwardLeft, LOW);
  digitalWrite(motorBackLeft, LOW);
  digitalWrite(motorForwardRight, HIGH);
  digitalWrite(motorBackRight, LOW);
  analogWrite(motorEnableLeft, 0);
  analogWrite(motorEnableRight, rightMotorSpeed);
  setLEDs(GREEN, true);
}

void BOT_Right () {
  digitalWrite(motorForwardLeft, HIGH);
  digitalWrite(motorBackLeft, LOW);
  digitalWrite(motorForwardRight, LOW);
  digitalWrite(motorBackRight, LOW);
  analogWrite(motorEnableLeft, leftMotorSpeed);
  analogWrite(motorEnableRight, 0);
  setLEDs(GREEN, true);
}

void BOT_Back () {
  digitalWrite(motorForwardLeft, LOW);
  digitalWrite(motorBackLeft, HIGH);
  digitalWrite(motorForwardRight, LOW);
  digitalWrite(motorBackRight, HIGH);
  analogWrite(motorEnableLeft, leftMotorSpeed);
  analogWrite(motorEnableRight, rightMotorSpeed);
  setLEDs(BLUE, true);
}
void BOT_Back_Right () {
  digitalWrite(motorForwardLeft, LOW);
  digitalWrite(motorBackLeft, HIGH);
  digitalWrite(motorForwardRight, LOW);
  digitalWrite(motorBackRight, LOW);
  analogWrite(motorEnableLeft, leftMotorSpeed);
  analogWrite(motorEnableRight, 0);
  setLEDs(BLUE | GREEN, true);
}

void BOT_Back_Left () {
  digitalWrite(motorForwardLeft, LOW);
  digitalWrite(motorBackLeft, LOW);
  digitalWrite(motorForwardRight, LOW);
  digitalWrite(motorBackRight, HIGH);
  analogWrite(motorEnableLeft, 0);
  analogWrite(motorEnableRight, rightMotorSpeed);
  setLEDs(BLUE | GREEN, true);
}

void BOT_NOPE_LEFT () {
  digitalWrite(motorForwardLeft, LOW);
  digitalWrite(motorBackLeft, HIGH);
  digitalWrite(motorForwardRight, HIGH);
  digitalWrite(motorBackRight, LOW);
  analogWrite(motorEnableLeft, leftMotorSpeed);
  analogWrite(motorEnableRight, rightMotorSpeed);
  setLEDs(RED | GREEN, true);
}

void BOT_NOPE_BACK () {
  digitalWrite(motorForwardLeft, LOW);
  digitalWrite(motorBackLeft, HIGH);
  digitalWrite(motorForwardRight, HIGH);
  digitalWrite(motorBackRight, LOW);
  analogWrite(motorEnableLeft, leftMotorSpeed);
  analogWrite(motorEnableRight, rightMotorSpeed);
  setLEDs(RED | BLUE, true);
}

void BOT_NOPE_RIGHT () {
  digitalWrite(motorForwardLeft, HIGH);
  digitalWrite(motorBackLeft, LOW);
  digitalWrite(motorForwardRight, LOW);
  digitalWrite(motorBackRight, HIGH);
  analogWrite(motorEnableLeft, leftMotorSpeed);
  analogWrite(motorEnableRight, rightMotorSpeed);
  setLEDs(RED | GREEN, true);
}

void BOT_ObstacleAvoidance (){
             BOT_ForwardFull();
             sensorRead ();
  
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
  pinMode(BTState, INPUT);
  pinMode(NEOIO, OUTPUT);
  IrReceiver.enableIRIn();
  Serial.begin(9600);
}

void loop() {
  /////////////////////////////////////// Bluetooth Functions
///////////////////////////////////////////////////////////
    if(Serial.available() > 0){     
      state = Serial.read();   
    }
      if (state == 'F') {
        BOT_ForwardFull();
    }
        else if (state == 'B') {
          BOT_Back();
    }
          else if (state == 'L') {
            BOT_NOPE_LEFT();
    }
            else if (state == 'R') {
              BOT_NOPE_RIGHT();
    }
              else if (state == 'G') {
                BOT_Left();
              }
                else if (state == 'I') {
                  BOT_Right();
                }
                  else if (state == 'H') {
                    BOT_Back_Left();
                  }
                    else if (state == 'J') {
                      BOT_Back_Right();
                    }
                  else if (state == 'S'){     
                    stop();
    }
//////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////

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
