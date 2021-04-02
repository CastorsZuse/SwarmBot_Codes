// SWARM BOTS: 
// NEOPIXEL REMOTE ACTIVATION
// MULTI PLAYER RECEIVER CODE
// BUMP CODE 
// UPDATED TO SWICH/CASE FORMAT
// TIMER CODE
//
// SPECIAL THANKS TO:
//      Andy Tracy
//      Justus Stahler
//      Christian Restifo
//      CW Kreimer
//
// See SwarmBots.online for more info // 

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

void stop() {
  digitalWrite(motorForwardLeft, LOW);
  digitalWrite(motorBackLeft, LOW);
  digitalWrite(motorForwardRight, LOW);
  digitalWrite(motorBackRight, LOW);
  analogWrite(motorEnableLeft, 0);
  analogWrite(motorEnableRight, 0);
  runFlag = false;
}

void setLEDs(bool blueValue, bool greenValue, bool redValue, bool neoValue) {
  digitalWrite(LEDblue, blueValue);
  digitalWrite(LEDgreen, greenValue);
  digitalWrite(LEDred, redValue);
  digitalWrite(NEOIO, neoValue);
}


void stopAndSetLEDs(bool blueValue, bool greenValue, bool redValue) {
  stop();
  setLEDs(blueValue, greenValue, redValue, false);
}

/////////////////////////////////////////////////

void BOT_ForwardFull () {
  digitalWrite(motorForwardLeft, HIGH);
  digitalWrite(motorBackLeft, LOW);
  digitalWrite(motorForwardRight, HIGH);
  digitalWrite(motorBackRight, LOW);
  analogWrite(motorEnableLeft, leftMotorSpeed);
  analogWrite(motorEnableRight, rightMotorSpeed);
  setLEDs(true, true, true, true);
}

void BOT_Left () {
  digitalWrite(motorForwardLeft, LOW);
  digitalWrite(motorBackLeft, LOW);
  digitalWrite(motorForwardRight, HIGH);
  digitalWrite(motorBackRight, LOW);
  analogWrite(motorEnableLeft, 0);
  analogWrite(motorEnableRight, rightMotorSpeed);
  setLEDs(false, true, false, true);
}

void BOT_Right () {
  digitalWrite(motorForwardLeft, HIGH);
  digitalWrite(motorBackLeft, LOW);
  digitalWrite(motorForwardRight, LOW);
  digitalWrite(motorBackRight, LOW);
  analogWrite(motorEnableLeft, leftMotorSpeed);
  analogWrite(motorEnableRight, 0);
  setLEDs(false, true, false, true);
}

void BOT_Back () {
  digitalWrite(motorForwardLeft, LOW);
  digitalWrite(motorBackLeft, HIGH);
  digitalWrite(motorForwardRight, LOW);
  digitalWrite(motorBackRight, HIGH);
  analogWrite(motorEnableLeft, leftMotorSpeed);
  analogWrite(motorEnableRight, rightMotorSpeed);
  setLEDs(true, false, false, true);
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

  Serial.print("Left Sensor: ");
  Serial.println(distanceLeft);
  Serial.print("Right Sensor: ");
  Serial.println(distanceRight);
  Serial.print("Front Sensor: ");
  Serial.println(distanceFront);
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
  pinMode(NEOIO, OUTPUT);
  
  irrecv.enableIRIn();
  Serial.begin(9600);
}

////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

void loop() {
  if (irrecv.decode(&results)) {
    current_code = results.value;
    Serial.print("New code received: ");
    Serial.println(current_code);
    irrecv.resume();

    switch (current_code) { 
      case iRIN_ACTIVATION:
        Serial.println("BOT ACTIVATION");
        runFlag = true;
        break;
      
      // LED argument order: blue, green, red
      case iRIN_botSTOP_R:
        Serial.println("botSTOP_RED");
        stopAndSetLEDs(false, false, true);
        break;
        
      case iRIN_botSTOP_G:
        Serial.println("botSTOP_GREEN");
        stopAndSetLEDs(false, true, false);
        break;
        
      case iRIN_botSTOP_B:
        Serial.println("botSTOP_BLUE");
        stopAndSetLEDs(true, false, false);
        break;
        
      case iRIN_botSTOP_RB:
        Serial.println("botSTOP_RED_BLUE");
        stopAndSetLEDs(true, false, true);
        break;
        
      case iRIN_botSTOP_RG:
        Serial.println("botSTOP_RED_GREEN");
        stopAndSetLEDs(false, true, true);
        break;
        
      case iRIN_botSTOP_BG:
        Serial.println("botSTOP_BLUE_GREEN");
        stopAndSetLEDs(true, true, false);
        break;
        
      case iRIN_BUMP_LEFT:
        Serial.println("BOT_BUMP_LEFT");
        BOT_Left();
        delay(500);
        break;
        
      case iRIN_BUMP_RIGHT:
        Serial.println("BOT_BUMP_RIGHT");
        BOT_Right();
        delay(500);
        break;

      case iRIN_STALL:
        Serial.println("BOT_STALL");
        BOT_Back();
        delay(500);
        break;

      default:
        break;
    }
  }

  if ( runFlag ) {
    BOT_ObstacleAvoidance();
  }
}
