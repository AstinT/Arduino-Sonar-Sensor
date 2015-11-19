#include <RedBot.h>
#include <RedBotSoftwareSerial.h>

RedBotMotors motors;
RedBotSensor leftSensor = RedBotSensor(A1);
RedBotSensor centerSensor = RedBotSensor(A4);
RedBotSensor rightSensor = RedBotSensor(A5);

//Sensors
int sensorOneTrig = 9;
int sensorTwoTrig = 11;
int sensorOneEcho = 3;
int sensorTwoEcho = 10;

//Sonar dist
float sensorOneDist;
float sensorTwoDist;

//Line sensor
int valLeft;
int valCenter;
int valRight;  

//States
boolean followWall;
boolean findLine;

void setup ()
{
  pinMode(sensorOneTrig, OUTPUT);
  pinMode(sensorTwoTrig, OUTPUT);
  pinMode(sensorOneEcho, INPUT);
  pinMode(sensorTwoEcho, INPUT);
  
  followWall = true;
}

void loop ()
{
  if (followWall == true)
  {
    sensorOneDist = readSonar(sensorOneTrig, sensorOneEcho);
    sensorTwoDist = readSonar(sensorTwoTrig, sensorTwoEcho);    
   
    if (sensorOneDist < 25)
    {     
      sensorOneDist = readSonar(sensorOneTrig, sensorOneEcho);
      if (sensorOneDist < 10)
      {
        followWall = false;
        findLine = true;
      }
      else
      {
        driveStraight(100, 100);
      }
    }
    else if (sensorTwoDist > 15)
    {
      driveCloser(100, 100);
    }
    else if (sensorTwoDist < 10)
    {
      driveAway(100, 100);
    }    
    else
    {
      driveStraight(100, 100);
    }
    delay(50);
  }
  else if (findLine == true)
  {
    pivotRight90();
    while((valLeft < 800) || (valCenter < 800) || (valRight < 800))
    {
      driveStraight(100, 50);
      valLeft = leftSensor.read();
      valCenter = centerSensor.read();
      valRight = rightSensor.read();
    }
    
    findLine = false;
  }
}


// Drive straight function.
// Takes two parameters power and time.
void driveStraight(int power, int time)
{
  motors.rightDrive(power);
  motors.leftDrive(power*0.72);
  delay(time);
  motors.brake();
}

void driveCloser(int power, int time)
{
  motors.rightDrive(power);
  motors.leftDrive((power*0.72) + 20);
  delay(time);
  motors.brake();
}

void driveAway(int power, int time)
{
  motors.rightDrive(power + 20);
  motors.leftDrive(power*0.72);
  delay(time);
  motors.brake();
}

void pivotRight90()
{
  motors.leftDrive(150);
  motors.rightDrive((-150)*0.63);
  delay(280);
  motors.brake();
  delay(20);
}

float readSonar (int trigPin, int echoPin)
{
  // Clear the trigger
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  
  // Send the sonar pulse
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  
  // Clear the trigger
  digitalWrite(trigPin, LOW);
  
  // Calculate duration
  float duration = pulseIn(echoPin, HIGH);
  
  // Return distance
  return duration / 58;
}
