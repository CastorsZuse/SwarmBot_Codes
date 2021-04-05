//THIS CODE IS A BASE CODE BUILT FOR THE SWARM BOTS SHIELD
//THIS CODE WILL BE INTEGRATED INTO THE SWARM BOTS CODE
//CHECK OUT SwarmBots.online for more info
//OR  github.com/CastorsZuse FOR MORE CODES
/////////////////////////////////////////////////////////
////////////////////////////////////////////////////////

//Pin numbers definition
  const int motorForwardLeft   = 7;  
  const int motorBackLeft      = 8; 
  const int motorEnableLeft    = 9;
  const int motorForwardRight  = 12; 
  const int motorBackRight     = 10; 
  const int motorEnableRight   = 11;

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
  
//Line Sensor Connection
  const int LEFT_LINE_SENSOR_PIN =A4;
  const int RIGHT_LINE_SENSOR_PIN =A5;

  int LEFT_LINE_SENSOR_STATE;
  int RIGHT_LINE_SENSOR_STATE;

// (RED | GREEN | BLUE)
void setLEDs(int colorValue) {
  digitalWrite(LEDblue, BLUE & colorValue);
  digitalWrite(LEDgreen, GREEN & colorValue);
  digitalWrite(LEDred, RED & colorValue);

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

void BOT_NOPE_BACK () {
  digitalWrite(motorForwardLeft, LOW);
  digitalWrite(motorBackLeft, HIGH);
  digitalWrite(motorForwardRight, HIGH);
  digitalWrite(motorBackRight, LOW);
  analogWrite(motorEnableLeft, leftMotorSpeed);
  analogWrite(motorEnableRight, rightMotorSpeed);
  setLEDs(RED | BLUE);
}

void setup() {
  pinMode(motorForwardLeft, OUTPUT);
  pinMode(motorBackLeft, OUTPUT);
  pinMode(motorForwardRight, OUTPUT);
  pinMode(motorBackRight, OUTPUT);
  pinMode(LEDred, OUTPUT);
  pinMode(LEDgreen, OUTPUT);
  pinMode(LEDblue, OUTPUT);
  
  Serial.begin(9600);
  delay(3000);  
}

void loop() {
  LEFT_LINE_SENSOR_STATE = analogRead(LEFT_LINE_SENSOR_PIN);
  RIGHT_LINE_SENSOR_STATE = analogRead(RIGHT_LINE_SENSOR_PIN);

      if(RIGHT_LINE_SENSOR_STATE > 500 && LEFT_LINE_SENSOR_STATE < 500)
     {
  Serial.println("turning right");
      BOT_Right();
      delay(delayTime);
  }
  
      if(RIGHT_LINE_SENSOR_STATE < 500 && LEFT_LINE_SENSOR_STATE > 500)
      {
  Serial.println("turning left");
      BOT_Left();
      delay(delayTime);
   }

      if(RIGHT_LINE_SENSOR_STATE > 500 && LEFT_LINE_SENSOR_STATE > 500)
      {
  Serial.println("GO!");
      BOT_ForwardFull();
//      delay(delayTime);
  }

      if(RIGHT_LINE_SENSOR_STATE < 500 && LEFT_LINE_SENSOR_STATE < 500)
      { 
  Serial.println("NOOOPE!!!");
         BOT_NOPE_BACK();
         delay(800);
  }

 
}










 
