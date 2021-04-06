// SWARM BOTS: 
// MULTI PLAYER RECEIVER CODE
// BUMP CODE 
// UPDATED TO SWICH/CASE FORMAT
// UPDATE TO .H_3.0 LIB
//////////////////////////////////////////////////////
//
// iRIN_botSTOP_R NOT SETTING LED
//
// New code received: 0. ISSUE
//
////////////////////////////////////////////////////
// 
//Pin numbers definition
const int motorEnableLeft = 9;
const int motorForwardLeft = 7;
const int motorBackLeft = 8;
const int motorEnableRight = 11;
const int motorForwardRight = 12;
const int motorBackRight = 10;
const int trigPinFront = A1;
const int echoPinFront = 2;
const int trigPinLeft = 3;
const int echoPinLeft = 4;
const int trigPinRight = 5;
const int echoPinRight = 6;
const int irPin = A0;
const int NEOIO = A4;

//RGB LED pins
const int LEDred = 13;
const int LEDgreen = A2;
const int LEDblue = A3;

enum Color : int {
  RED   = 1,
  GREEN = 2,
  BLUE  = 4
};

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

//Variables for IR Sensor
#define DECODE_NEC
#include <IRremote.h>

unsigned long current_code = 0;
boolean runFlag = false;

// TIMER
unsigned long activationTime = 0;
unsigned long timeout_ms = 10000;

//Control IR numbers
const long iRIN_ACTIVATION = 3158572800;    //16761405;
const long iRIN_botSTOP_R =  4077715200;    //16724175;
const long iRIN_botSTOP_G =  3877175040;    //16718055;
const long iRIN_botSTOP_B =  2707357440;    //16743045;
const long iRIN_botSTOP_RB = 4144561920;    //16716015;
const long iRIN_botSTOP_RG = 3810328320;    //16726215;
const long iRIN_botSTOP_BG = 2774204160;    //16734885;
const long iRIN_BUMP_LEFT =  3125149440;    //16753245;
const long iRIN_STALL =      3108437760;    //16736925;
const long iRIN_BUMP_RIGHT = 3091726080;    //16769565;
const long iRIN_NOPE_LEFT =  4161273600;    //16769055;
const long iRIN_NOPE_BACK  = 3927310080;    //16754775;
const long iRIN_NOPE_RIGHT = 4127850240;    //16748655;

void stop() {
  digitalWrite(motorForwardLeft, LOW);
  digitalWrite(motorBackLeft, LOW);
  digitalWrite(motorForwardRight, LOW);
  digitalWrite(motorBackRight, LOW);
  analogWrite(motorEnableLeft, 0);
  analogWrite(motorEnableRight, 0);
  runFlag = false;
}

// colorValue should be a bitwise combination of Color values, such as
// ( RED | BLUE ) for red and blue
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

/////////////////////////////////////////////////

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
  sensorRead();

  if ((distanceFront <= minFrontDistance) ||
      (distanceLeft <= minSideDistance) ||
      (distanceRight <= minSideDistance)) {
    if ((distanceLeft < stuckDistance) ||
        (distanceRight < stuckDistance) ||
        (distanceFront < stuckDistance)) {
      BOT_Back();
      delay(1.5*delayTime);
    } else if ((distanceFront <= minFrontDistance) &&
               (distanceLeft <= minSideDistance) &&
               (distanceRight <= minSideDistance)) {
      BOT_Back();
      delay(1.5*delayTime);
    } else if (distanceLeft > distanceRight ) {
      BOT_Left();
      delay(delayTime);
    } else if (distanceLeft <= distanceRight) {
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
  
  IrReceiver.begin(irPin, ENABLE_LED_FEEDBACK);     // NEW
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
  pinMode(NEOIO, OUTPUT);
  
  IrReceiver.enableIRIn();                // unedited ////////////////////////////////////
  Serial.begin(9600);
}

////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

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
      case iRIN_ACTIVATION:
        Serial.println("BOT ACTIVATION");
        runFlag = true;
        activationTime = millis();
        break;

      case iRIN_botSTOP_R:
        Serial.println("botSTOP_RED");
        stopAndSetLEDs(RED);
        break;
        
      case iRIN_botSTOP_G:
        Serial.println("botSTOP_GREEN");
        stopAndSetLEDs(GREEN);
        break;
        
      case iRIN_botSTOP_B:
        Serial.println("botSTOP_BLUE");
        stopAndSetLEDs(BLUE);
        break;
        
      case iRIN_botSTOP_RB:
        Serial.println("botSTOP_RED_BLUE");
        stopAndSetLEDs(RED | BLUE);
        break;
        
      case iRIN_botSTOP_RG:
        Serial.println("botSTOP_RED_GREEN");
        stopAndSetLEDs(RED | GREEN);
        break;
        
      case iRIN_botSTOP_BG:
        Serial.println("botSTOP_BLUE_GREEN");
        stopAndSetLEDs(BLUE | GREEN);
        break;
        
      case iRIN_BUMP_LEFT:
        if ( runFlag ) {
          Serial.println("BOT_BUMP_LEFT");
          BOT_Left();
          delay(500);
        }
        break;
        
      case iRIN_BUMP_RIGHT:
        if ( runFlag ) {
          Serial.println("BOT_BUMP_RIGHT");
          BOT_Right();
          delay(500);
        }
        break;

      case iRIN_STALL:
        if ( runFlag ) {
          Serial.println("BOT_STALL");
          BOT_Back();
          delay(500);
        }
        break;

      case iRIN_NOPE_LEFT:
        if ( runFlag ) {
          Serial.println("NOPE_LEFT");
          BOT_NOPE_LEFT();
          delay (350);
        }
        break;

      case iRIN_NOPE_BACK:
        if ( runFlag ) {
          Serial.println("NOOOOPE");
          BOT_NOPE_BACK();
          delay (800);
        }
        break;

      case iRIN_NOPE_RIGHT:
        if ( runFlag ) {
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
