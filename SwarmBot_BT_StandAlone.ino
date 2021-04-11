// SWARM BOTS SHIELD BLUETOOTH CONTROL
// this code will not work with the ir 
// or ultrasonic obsticle avoidance 
// ...yet
//
// See SwarmBots.online for more info

//Pin numbers definition
// Left motor pins
const int motorEnableLeft = 9;
const int motorForwardLeft = 7;
const int motorBackLeft = 8;
// Right motor pins
const int motorEnableRight = 11;
const int motorForwardRight = 12;
const int motorBackRight = 10;
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

//Variables for the Motors
const int leftMotorSpeed = 255;
const int rightMotorSpeed = 255;
const int delayTime = 150;

// Veriables for HM-10
#include <SoftwareSerial.h>
SoftwareSerial BTserial(2, 3); // RX | TX
char BtIN;

// RGB Led set function 
void setLEDs(int colorValue) {  
  digitalWrite(LEDblue, BLUE & colorValue);
  digitalWrite(LEDgreen, GREEN & colorValue);
  digitalWrite(LEDred, RED & colorValue);
}
void stopAndSetLEDs(int colorValue) {
  stop();
  setLEDs(colorValue);
}

// Movement Functions
void stop() {
  digitalWrite(motorForwardLeft, LOW);
  digitalWrite(motorBackLeft, LOW);
  digitalWrite(motorForwardRight, LOW);
  digitalWrite(motorBackRight, LOW);
  analogWrite(motorEnableLeft, 0);
  analogWrite(motorEnableRight, 0);
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

//////////////////////////////////////// TO BE ADDED AND TESTED LATER////////////
/////////////////////////////////////////////////////////////////////////////////
//void BOT_ObstacleAvoidance (){
//             BOT_ForwardFull();
//             sensorRead ();
//
//////////////////////////////////////////////////////////////////////
///////////////// LINE FOLLOWING SENSOR AI TREE /////////////////////
/////////////////////////////////////////////////////////////////////
//
////        (boundaryDetectionRIGHT_STATE > THRESHOLD && 
////          boundaryDetectionLEFT_STATE < THRESHOLD){
////          if the right sensor is high
////             and left sensor is low
////             bot will turn right 
////          function triggetrs on white 
////          delay 2 x 150
//////////////////////////////////////////////////////////////////
//
//          boundaryDetectionLEFT_STATE = analogRead(boundaryDetectionLEFT_pin);
//          boundaryDetectionRIGHT_STATE = analogRead(boundaryDetectionRIGHT_pin);
//
//      if(boundaryDetectionRIGHT_STATE > THRESHOLD && 
//          boundaryDetectionLEFT_STATE < THRESHOLD){
//        Serial.println("iR BOUNDARY DETECTED LEFT, TURN RIGHT");
//           BOT_Right();
//             delay(2*delayTime);
//  }  
//      if(boundaryDetectionRIGHT_STATE < THRESHOLD && 
//         boundaryDetectionLEFT_STATE > THRESHOLD){
//        Serial.println("iR BOUNDARY DETECTED RIGHT, TURN LEFT");
//           BOT_Left();
//            delay(2*delayTime);
//   }
//      if(boundaryDetectionRIGHT_STATE < THRESHOLD && 
//         boundaryDetectionLEFT_STATE < THRESHOLD) { 
//        Serial.println("iR BOUNDARY DETECTED FRONT, TURN AROUND");
//          BOT_NOPE_BACK();
//            delay(800);
//  }
//
//////////////////////////////////////////////////////////////////////
///////////////// LINE FOLLOWING SENSOR AI TREE /////////////////////
/////////////////////////////////////////////////////////////////////
//  
//  if ((distanceFront <= minFrontDistance) ||
//      (distanceLeft <= minSideDistance) ||
//      (distanceRight <= minSideDistance)) {
//    if ((distanceLeft < stuckDistance) ||
//        (distanceRight < stuckDistance) ||
//        (distanceFront < stuckDistance)) {
//         Serial.println("U/S ALL SENSORS, OBSTACLE DANGEROUSLY CLOSE! BACK UP!");
//         BOT_Back();
//         delay(1.5*delayTime);      
//   } 
//    else if ((distanceFront <= minFrontDistance) &&
//               (distanceLeft <= minSideDistance) &&
//               (distanceRight <= minSideDistance)) {
//                Serial.println("U/S ALL SENSORS OBSTACLE TOO CLOSE. BACK UP");
//                BOT_Back();
//                delay(1.5*delayTime);    
//   } 
//    else if (distanceLeft > distanceRight ) {
//               Serial.println("U/S OBSTACLE RIGHT, TURNING LEFT");
//               BOT_Left();
//               delay(delayTime);
//               
//    }
//    else if (distanceLeft <= distanceRight) {
//              Serial.println("U/S OBSTACLE LEFT, TURN RIGHT");
//              BOT_Right();
//              delay(delayTime);
//    }
//  }
//}
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

void setup() {
BTserial.begin(9600);

pinMode(motorEnableLeft, OUTPUT);
  pinMode(motorForwardLeft, OUTPUT);
  pinMode(motorBackLeft, OUTPUT);
  pinMode(motorEnableRight, OUTPUT);
  pinMode(motorForwardRight, OUTPUT);
  pinMode(motorBackRight, OUTPUT);
  pinMode(LEDred, OUTPUT);
  pinMode(LEDgreen, OUTPUT);
  pinMode(LEDblue, OUTPUT);
  
  Serial.begin(9600);
}
void loop() {

if (BTserial.available())
    {  
        BtIN = BTserial.read();
    }
if (BtIN == 'F'){
  BOT_ForwardFull();   
}
else if (BtIN == 'B'){
   BOT_NOPE_BACK();
}
else if (BtIN == 'L'){
  BOT_NOPE_LEFT();
}
else if (BtIN == 'R'){
  BOT_NOPE_RIGHT();
}
else if (BtIN == 'A'){
  stopAndSetLEDs(RED);
}
}
