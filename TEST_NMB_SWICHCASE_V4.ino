// SWARM BOTS: 
// NEOPIXEL REMOTE ACTIVATION
// MULTI PLAYER RECEIVER CODE
// BUMP CODE 
// UPDATED TO SWICH/CASE FORMAT
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
unsigned long key_value = 0;
boolean stopFlag = false;
////////////////////////////////////////////
///////////////////////////////////////////

void stopCar_RED () {
  digitalWrite(motorForwardLeft, LOW);
  digitalWrite(motorBackLeft, LOW);
  digitalWrite(motorForwardRight, LOW);
  digitalWrite(motorBackRight, LOW);
  analogWrite(motorEnableLeft, 0);
  analogWrite(motorEnableRight, 0);
  digitalWrite(LEDblue, LOW);
  digitalWrite(LEDgreen, LOW);
  digitalWrite(LEDred, HIGH);
  digitalWrite(NEOIO, LOW);
}

void stopCar_GREEN () {
  digitalWrite(motorForwardLeft, LOW);
  digitalWrite(motorBackLeft, LOW);
  digitalWrite(motorForwardRight, LOW);
  digitalWrite(motorBackRight, LOW);
  analogWrite(motorEnableLeft, 0);
  analogWrite(motorEnableRight, 0);
  digitalWrite(LEDblue, LOW);
  digitalWrite(LEDgreen, HIGH);
  digitalWrite(LEDred, LOW);
  digitalWrite(NEOIO, LOW);
}

void stopCar_BLUE () {
  digitalWrite(motorForwardLeft, LOW);
  digitalWrite(motorBackLeft, LOW);
  digitalWrite(motorForwardRight, LOW);
  digitalWrite(motorBackRight, LOW);
  analogWrite(motorEnableLeft, 0);
  analogWrite(motorEnableRight, 0);
  digitalWrite(LEDblue, HIGH);
  digitalWrite(LEDgreen, LOW);
  digitalWrite(LEDred, LOW);
  digitalWrite(NEOIO, LOW);
}

void stopCar_RED_BLUE () {
  digitalWrite(motorForwardLeft, LOW);
  digitalWrite(motorBackLeft, LOW);
  digitalWrite(motorForwardRight, LOW);
  digitalWrite(motorBackRight, LOW);
  analogWrite(motorEnableLeft, 0);
  analogWrite(motorEnableRight, 0);
  digitalWrite(LEDblue, HIGH);
  digitalWrite(LEDgreen, LOW);
  digitalWrite(LEDred, HIGH);
  digitalWrite(NEOIO, LOW);
}
void stopCar_RED_GREEN () {
  digitalWrite(motorForwardLeft, LOW);
  digitalWrite(motorBackLeft, LOW);
  digitalWrite(motorForwardRight, LOW);
  digitalWrite(motorBackRight, LOW);
  analogWrite(motorEnableLeft, 0);
  analogWrite(motorEnableRight, 0);
  digitalWrite(LEDblue, LOW);
  digitalWrite(LEDgreen, HIGH);
  digitalWrite(LEDred, HIGH);
  digitalWrite(NEOIO, LOW);
}
void stopCar_BLUE_GREEN () {
  digitalWrite(motorForwardLeft, LOW);
  digitalWrite(motorBackLeft, LOW);
  digitalWrite(motorForwardRight, LOW);
  digitalWrite(motorBackRight, LOW);
  analogWrite(motorEnableLeft, 0);
  analogWrite(motorEnableRight, 0);
  digitalWrite(LEDblue, HIGH);
  digitalWrite(LEDgreen, HIGH);
  digitalWrite(LEDred, LOW);
  digitalWrite(NEOIO, LOW);
}



/////////////////////////////////////////////////

void goForwardFull () {
  digitalWrite(motorForwardLeft, HIGH);
  digitalWrite(motorBackLeft, LOW);
  digitalWrite(motorForwardRight, HIGH);
  digitalWrite(motorBackRight, LOW);
  analogWrite(motorEnableLeft, leftMotorSpeed);
  analogWrite(motorEnableRight, rightMotorSpeed);
  digitalWrite(LEDblue, HIGH); 
  digitalWrite(LEDgreen, HIGH);
  digitalWrite(LEDred, HIGH);
  digitalWrite(NEOIO, HIGH);
}

void goLeft () {
  digitalWrite(motorForwardLeft, LOW);
  digitalWrite(motorBackLeft, LOW);
  digitalWrite(motorForwardRight, HIGH);
  digitalWrite(motorBackRight, LOW);
  analogWrite(motorEnableLeft, 0);
  analogWrite(motorEnableRight, rightMotorSpeed);
  digitalWrite(LEDblue, LOW);
  digitalWrite(LEDgreen, HIGH);
  digitalWrite(LEDred, LOW);
  digitalWrite(NEOIO, HIGH);
}

void goRight () {
  digitalWrite(motorForwardLeft, HIGH);
  digitalWrite(motorBackLeft, LOW);
  digitalWrite(motorForwardRight, LOW);
  digitalWrite(motorBackRight, LOW);
  analogWrite(motorEnableLeft, leftMotorSpeed);
  analogWrite(motorEnableRight, 0);
  digitalWrite(LEDblue, LOW);
  digitalWrite(LEDgreen, HIGH);
  digitalWrite(LEDred, LOW);
  digitalWrite(NEOIO, HIGH);
}

void goBack () {
  digitalWrite(motorForwardLeft, LOW);
  digitalWrite(motorBackLeft, HIGH);
  digitalWrite(motorForwardRight, LOW);
  digitalWrite(motorBackRight, HIGH);
  analogWrite(motorEnableLeft, leftMotorSpeed);
  analogWrite(motorEnableRight, rightMotorSpeed);
  digitalWrite(LEDblue, HIGH);
  digitalWrite(LEDgreen, LOW);
  digitalWrite(LEDred, LOW);
  digitalWrite(NEOIO, HIGH);
}

//////////////////////////////////////////////////
///////////////////////////////////////////////////

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


void loop() {
  sensorRead();
  Serial.println(results.value);
  if (irrecv.decode(&results)) { // decode the received signal and store it in results
        irrecv.resume();
    if (results.value == 0xFFFFFFFF) { // if the value is equal to 0xFFFFFFFF
      results.value = key_value; // set the value to the key value
      if (irrecv.decode(&results)) {
    unsigned long res;
    res = results.value, DEC;

}
    }
    switch (results.value) { 
  case 0xFFC23D:
        Serial.println("BOT ACTIVATION");
        if ((distanceFront <= minFrontDistance) || (distanceLeft <= minSideDistance) || (distanceRight <= minSideDistance)) {
         if ((distanceLeft < stuckDistance) || (distanceRight < stuckDistance) || (distanceFront < stuckDistance)) {
            goBack();
            delay(1.5*delayTime);
            }
            else if ((distanceFront <= minFrontDistance) && (distanceLeft <= minSideDistance) && (distanceRight <= minSideDistance)) {
             goBack();
              delay(1.5*delayTime);
            }
            else if (distanceLeft > distanceRight ) {
             goLeft();
             delay(delayTime);
           }
           else if (distanceLeft <= distanceRight) {
             goRight();
             delay(delayTime);
            }
           else
              goForwardFull();
           }
           else
             goForwardFull();
               break;
        
      case 0xFF30CF:
        Serial.println("stopCar_RED");
            stopCar_RED();
        break;
        
      case 0xFF18E7:
        Serial.println("stopCar_GREEN");
            stopCar_GREEN();
        break;
        
      case 0xFF7A85:
        Serial.println("stopCar_BLUE");
           stopCar_BLUE();
        break;
        
      case 0xFF10EF:
        Serial.println("stopCar_RED_BLUE");
           stopCar_RED_BLUE();
        break ;
        
      case 0xFF38C7:
        Serial.println("stopCar_RED_GREEN");
            stopCar_RED_GREEN();
        break ;
        
      case 0xFF5AA5:
        Serial.println("stopCar_BLUE_GREEN");
            stopCar_BLUE_GREEN();
        break ;
        
      case 0xFFA25D:
        Serial.println("BUMP_LEFT");
            goLeft();
             delay(500);
        break ;
        
      case 0xFFE21D:
        Serial.println("BUMP_RIGHT");
            goRight();
              delay(500);
        break ;

      case 0xFF629D:
        Serial.println("STALL");
            goBack();
              delay(500);
        break ;
          default:
        break;

    }
    key_value = results.value; // store the value as key_value
    irrecv.resume(); // reset the receiver for the next code
  }
}
