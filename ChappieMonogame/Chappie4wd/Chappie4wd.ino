#include <ESP8266WiFi.h>
#include <aREST.h>
#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"

// Create the motor shield object with the default I2C address
Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 

//LF = Left Front, LR = Left Rear, RF = Right Front, RR = Right Rear
Adafruit_DCMotor *LF_MOTOR = AFMS.getMotor(1);
Adafruit_DCMotor *LR_MOTOR = AFMS.getMotor(2);
Adafruit_DCMotor *RF_MOTOR = AFMS.getMotor(4);
Adafruit_DCMotor *RR_MOTOR = AFMS.getMotor(3);


// Create aREST instance
aREST rest = aREST();

//////////////////////
// WiFi Definitions //
//////////////////////
const char WiFiAPPSK[] = "ChuckleNuts1!";

/////////////////////
// Pin Definitions //
/////////////////////
const int LED_PIN = 0; // Thing's onboard, green LED
const int ANALOG_PIN = A0; // The only analog pin on the Thing
const int DIGITAL_PIN = 12; // Digital pin to be read

WiFiServer server(80);

// Function
int stop(String message);
int forward(String message);
int right(String message);
int left(String message);
int curve(String message);
int backward(String message);
int setSpeed(String newSpeed);

// Testing New Stuff
int speed;
String lastFunction;
String lastParam;
String direction;

// ******************************* End - Header *******************************************

void setup()
{
  Serial.begin(115200);
  pinMode(DIGITAL_PIN, INPUT_PULLUP);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  speed = 100;
  direction = "";
  lastFunction = "";
  lastParam = "";
  
  
  // Functions          
  rest.function("stop", stop);
  rest.function("forward", forward);
  rest.function("left", left);  
  rest.function("right", right);
  rest.function("backward", backward);
  rest.function("speed", setSpeed);
  rest.function("curve", curve);  
      
  // Give name and ID to device
  rest.set_id("1");
  rest.set_name("robot");

  AFMS.begin();  // create with the default frequency 1.6KHz

  WiFi.mode(WIFI_AP);
  String AP_NameString = "Chappie";

  char AP_NameChar[AP_NameString.length() + 1];
  memset(AP_NameChar, 0, AP_NameString.length() + 1);

  for (int i = 0; i < AP_NameString.length(); i++)
    AP_NameChar[i] = AP_NameString.charAt(i);

  WiFi.softAP(AP_NameChar, WiFiAPPSK);
    
  //setupWiFi();
  server.begin();
  //initMotor();
  
}

void loop()
{
  // Handle REST calls
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
  while(!client.available()){
    delay(1);
  }
  rest.handle(client);
}

// ******************************* Start - Navigation Functions  *******************************
int setSpeed(String newSpeed) 
{  
  Serial.println(newSpeed);
  int temp = newSpeed.toInt();

  if (temp < 0) {
    temp = 0;
  } else if (temp > 255){
    temp = 255;
  }

  if (temp != NULL){
    speed = temp;
  } else {
    speed = 0;
  }

  Serial.print("Speed: ");
  Serial.println(speed);

  if (lastFunction == "stop") {
    stop("");
  } else if (lastFunction == "forward") {
    forward("");
  } else if (lastFunction == "backward") {
    backward("");
  } else if (lastFunction == "left") {
    left("");
  } else if (lastFunction == "right") {
    right("");
  } else if (lastFunction == "curve") {
    Serial.print("Speed calling curve: ");
    Serial.println(lastParam);
    curve(lastParam);
  }  
}

int stop(String command) 
{  
  // Stop
  LF_MOTOR->setSpeed(0);
  LF_MOTOR->run( RELEASE );
  LR_MOTOR->setSpeed(0);
  LR_MOTOR->run( RELEASE );
 
  RF_MOTOR->setSpeed(0);
  RF_MOTOR->run( RELEASE );
  RR_MOTOR->setSpeed(0);
  RR_MOTOR->run( RELEASE );  

  lastFunction = "stop";
  lastParam = "";
}

int forward(String command) 
{
  Serial.println(speed);
  LF_MOTOR->setSpeed(speed);
  LF_MOTOR->run( FORWARD );
  LR_MOTOR->setSpeed(speed);
  LR_MOTOR->run( BACKWARD );
 
  RF_MOTOR->setSpeed(speed);
  RF_MOTOR->run( FORWARD );
  RR_MOTOR->setSpeed(speed );
  RR_MOTOR->run( BACKWARD );  
  
  lastFunction = "forward";
  lastParam = "";
  direction = "forward";
}

int left(String command) 
{  
  // Stop
  LF_MOTOR->setSpeed(speed);
  LF_MOTOR->run( BACKWARD );
  LR_MOTOR->setSpeed(speed);
  LR_MOTOR->run( FORWARD );
 
  RF_MOTOR->setSpeed(speed);
  RF_MOTOR->run( FORWARD );  
  RR_MOTOR->setSpeed(speed);
  RR_MOTOR->run( BACKWARD );  

  lastFunction = "left";
  lastParam = "";
}

int curve(String command)
{
  int leftSpeed = 0;
  int rightSpeed = 0;
  int temp = command.toInt();

 
  if (temp > 0) {    
    Serial.print("1");
    rightSpeed = speed + temp * 5;
    leftSpeed = speed - temp * 5;     
  } else if (temp < 0) {
    Serial.print("2");
    rightSpeed = speed + temp * 5;
    leftSpeed = speed - temp * 5;         
  } else {
    leftSpeed = speed;
    rightSpeed = speed;
  }

  if (leftSpeed > 255) {
    leftSpeed = 255;
  }

  if (leftSpeed < 0) {
    leftSpeed = 0;
  }

  if (rightSpeed > 255) {
    rightSpeed = 255;
  }

  if (rightSpeed < 0) {
    rightSpeed = 0;
  }

  if (speed == 0){
    leftSpeed = 0;
    rightSpeed = 0;
  }
  
  Serial.print("curve: ");
  Serial.print(command);  
  Serial.print(" leftSpeed: ");
  Serial.print(leftSpeed);  
  Serial.print(" rightSpeed: ");
  Serial.println(rightSpeed);  

  Serial.print("direction: ");
  Serial.println(direction);
  
  if (direction == "backward") {
    LF_MOTOR->setSpeed(leftSpeed);
    LF_MOTOR->run( BACKWARD );
    LR_MOTOR->setSpeed(leftSpeed);
    LR_MOTOR->run( FORWARD );
   
    RF_MOTOR->setSpeed(rightSpeed);
    RF_MOTOR->run( BACKWARD );  
    RR_MOTOR->setSpeed(rightSpeed);
    RR_MOTOR->run( FORWARD );  
  } else {
    LF_MOTOR->setSpeed(leftSpeed);
    LF_MOTOR->run( FORWARD );
    LR_MOTOR->setSpeed(leftSpeed);
    LR_MOTOR->run( BACKWARD );
   
    RF_MOTOR->setSpeed(rightSpeed);
    RF_MOTOR->run( FORWARD );  
    RR_MOTOR->setSpeed(rightSpeed);
    RR_MOTOR->run( BACKWARD );          
  }  

  lastFunction = "curve";
  lastParam = command;
}

int leftCurve(String command) 
{  
  Serial.print("leftCurve: ");
  Serial.println(command);
  int leftSpeed = 0;
  int rightSpeed = 0;
  int temp = command.toInt();

  switch (temp) {
    case 1:
      leftSpeed = speed - 10;
      rightSpeed = speed + 10;
      break;
    case 2:
      leftSpeed = speed - 5;
      rightSpeed = speed + 25;
      break;
    case 3:
      leftSpeed = speed - 15;
      rightSpeed = speed + 30;
      break;      
    default: 
      leftSpeed = speed;
      rightSpeed = speed;
      // if nothing else matches, do the default
      // default is optional
    break;
  }
 
  // Stop
  LF_MOTOR->setSpeed(leftSpeed);
  LF_MOTOR->run( FORWARD );
  LR_MOTOR->setSpeed(leftSpeed);
  LR_MOTOR->run( BACKWARD );
 
  RF_MOTOR->setSpeed(rightSpeed);
  RF_MOTOR->run( FORWARD );  
  RR_MOTOR->setSpeed(rightSpeed);
  RR_MOTOR->run( BACKWARD );  

  lastFunction = "leftCurve";
}

int right(String command) 
{  
  LF_MOTOR->setSpeed(speed);
  LF_MOTOR->run( FORWARD );
  LR_MOTOR->setSpeed(speed);
  LR_MOTOR->run( BACKWARD );
 
  RF_MOTOR->setSpeed(speed);
  RF_MOTOR->run( BACKWARD );  
  RR_MOTOR->setSpeed(speed);
  RR_MOTOR->run( FORWARD );  

  lastFunction = "right";
  lastParam = "";
}

int backward(String command) 
{  
  // Stop
  LF_MOTOR->setSpeed(speed);
  LF_MOTOR->run( BACKWARD );
  LR_MOTOR->setSpeed(speed);
  LR_MOTOR->run( FORWARD );
 
  RF_MOTOR->setSpeed(speed);
  RF_MOTOR->run( BACKWARD ); 
  RR_MOTOR->setSpeed(speed);
  RR_MOTOR->run( FORWARD ); 

  lastFunction = "backward";
  lastParam = "";
  direction = "backward";
}

// ******************************* End - Navigation Functions  *******************************
