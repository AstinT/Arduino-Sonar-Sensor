#include <RedBot.h>
#include <RedBotSoftwareSerial.h>

RedBotMotors motors;

int sensorOneTrig = 9;
int sensorTwoTrig = 11;
int sensorOneEcho = 3;
int sensorTwoEcho = 10;

float sensorOneDist;
float sensorTwoDist;

void setup ()
{
  Serial.begin (9600);
  pinMode(sensorOneTrig, OUTPUT);
  pinMode(sensorTwoTrig, OUTPUT);
  pinMode(sensorOneEcho, INPUT);
  pinMode(sensorTwoEcho, INPUT);
}

void loop ()
{
  sensorOneDist = readSonar(sensorOneTrig, sensorOneEcho);
  sensorTwoDist = readSonar(sensorTwoTrig, sensorTwoEcho);
  
  delay(100);
  
  if (sensorTwoDist > 10)
  {
    driveCloser(100, 100);
  }
  else if (sensorTwoDist < 5)
  {
    driveAway(100, 100);
  }
  else
  {
    driveStraight(100, 100);
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
  motors.leftDrive(power*0.90);
  delay(time);
  motors.brake();  
}

void driveAway(int power, int time)
{
  motors.rightDrive(power + 40);
  motors.leftDrive(power*0.72);
  delay(time);
  motors.brake();
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
