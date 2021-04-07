// SWARM BOTS: 
// NEOPIXEL REMOTE ACTIVATION
// MULTI PLAYER RECEIVER CODE
// BUMP CODE 
// UPDATED TO SWICH/CASE FORMAT
//////////////////////////////////////////////////////
////////////////////////////////////////////////////
//
// THIS IS THE LINE FOLLOWING INTIGRADE 
// FROM THE WORKING TEST CODE, FOR DEBUG
//
//    4.5.21 STATUS: 
//        line following ir sensors are working with
//        a 10-15mm range. 
//        turn functions are bing called and led is
//        changing, however the L/R motor does not 
//        stop. 
//        its as if another function is pushing thru...
//            and or delay between calls
//        
//        function triggers on white not black.
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

const int LEFT_LINE_SENSOR_PIN = A4;
const int RIGHT_LINE_SENSOR_PIN = A5;

//RGB LED pins
const int LEDred = 13;
const int LEDgreen = A2;
const int LEDblue = A3;

  const int THRESHOLD = 900; 
  // boolean ZAPPED_Flag = true
  int LEFT_LINE_SENSOR_STATE;
  int RIGHT_LINE_SENSOR_STATE

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
#include <IRremote.h>
IRrecv irrecv(irPin);
decode_results results;
unsigned long current_code = 0;
boolean runFlag = false;

// TIMER
unsigned long activationTime = 0;
unsigned long timeout_ms = 90000;   // 1000 = 1 second

//Control IR numbers
const long iRIN_ACTIVATION = 16761405;
const long iRIN_botSTOP_R =  16724175;
const long iRIN_botSTOP_G =  16718055;
const long iRIN_botSTOP_B =  16743045;
const long iRIN_botSTOP_RB = 16716015;
const long iRIN_botSTOP_RG = 16726215;
const long iRIN_botSTOP_BG = 16734885;
const long iRIN_BUMP_LEFT =  16753245;
const long iRIN_STALL =      16736925;
const long iRIN_BUMP_RIGHT = 16769565;
const long iRIN_NOPE_LEFT =  16769055;
const long iRIN_NOPE_BACK  = 16754775;
const long iRIN_NOPE_RIGHT = 16748655;

void stop() {
  digitalWrite(motorForwardLeft, LOW);
  digitalWrite(motorBackLeft, LOW);
  digitalWrite(motorForwardRight, LOW);
  digitalWrite(motorBackRight, LOW);
  analogWrite(motorEnableLeft, 0);
  analogWrite(motorEnableRight, 0);
  runFlag = false;
//ZAPPED_Flag = true;
}

// colorValue should be a bitwise combination of Color values, such as
// ( RED | BLUE ) for red and blue
void setLEDs(int colorValue) {  
  digitalWrite(LEDblue, BLUE & colorValue);
  digitalWrite(LEDgreen, GREEN & colorValue);
  digitalWrite(LEDred, RED & colorValue);
}

void stopAndSetLEDs(int colorValue) {
  stop();
  setLEDs(colorValue);
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

//        (RIGHT_LINE_SENSOR_STATE > THRESHOLD && 
//          LEFT_LINE_SENSOR_STATE < THRESHOLD){
//          if the right sensor is high
//             and left sensor is low
//             bot will turn right 
//          function triggetrs on white 
////////////////////////////////////////////////////////////////

          LEFT_LINE_SENSOR_STATE = analogRead(LEFT_LINE_SENSOR_PIN);
          RIGHT_LINE_SENSOR_STATE = analogRead(RIGHT_LINE_SENSOR_PIN);

      if(RIGHT_LINE_SENSOR_STATE > THRESHOLD && 
          LEFT_LINE_SENSOR_STATE < THRESHOLD){
        Serial.println("iR BOUNDARY DETECTED LEFT, TURN RIGHT");
           BOT_Right();
             delay(delayTime);
  }  
      if(RIGHT_LINE_SENSOR_STATE < THRESHOLD && 
         LEFT_LINE_SENSOR_STATE > THRESHOLD){
        Serial.println("iR BOUNDARY DETECTED RIGHT, TURN LEFT");
           BOT_Left();
            delay(delayTime);
   }
      if(RIGHT_LINE_SENSOR_STATE < THRESHOLD && 
         LEFT_LINE_SENSOR_STATE < THRESHOLD) { 
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
  
  irrecv.enableIRIn();
  Serial.begin(9600);
}

////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

void loop() {
  if (runFlag && (millis() - activationTime) > timeout_ms) {
        stop();
        setLEDs(RED | GREEN | BLUE);
  }  

  if (irrecv.decode(&results)) {
    current_code = results.value;
    Serial.print("New code received: ");
    Serial.println(current_code);
    irrecv.resume();

    switch (current_code) { 
      case iRIN_ACTIVATION:
        Serial.println("BOT ACTIVATION");
//      ZAPPED_Flag = false;
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

////////////////////////////////////////////////////
// if ( runFlag ) && ( ZAPPED_Flag ) {      
// if (( runFlag == true) && (ZAPPED_Flag == false){
//      Set true and false to 1 0
// if (runFlag > ZAPPED_Flag) {
// if (runFlag < ZAPPED_Flag) {
// if (runFlag = ZAPPED_Flag) {

      case iRIN_BUMP_LEFT:
        if ( runFlag ) {   // Replace all lines in relation 
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
