// Includes
#include <RedBot.h>
#include <RedBotSoftwareSerial.h>

// Instance of motors
RedBotMotors motors;

// Line sensors
RedBotSensor leftSensor = RedBotSensor(A0);
RedBotSensor centerSensor = RedBotSensor(A1);
RedBotSensor rightSensor = RedBotSensor(A6);

// Sensors
int sensorOneTrig = 9;
int sensorTwoTrig = 11;
int sensorOneEcho = 3;
int sensorTwoEcho = 10;

// Sonar dist
float sensorOneDist;
float sensorTwoDist;

// Line sensor
int valLeft;
int valCenter;
int valRight;  

// States
boolean followWall;
boolean findLine;
boolean followLine;

void setup ()
{
  // Setting up pins
  pinMode(sensorOneTrig, OUTPUT);
  pinMode(sensorTwoTrig, OUTPUT);
  pinMode(sensorOneEcho, INPUT);
  pinMode(sensorTwoEcho, INPUT);
  
  // Setting first state to be true
  followWall = true;
}

void loop ()
{
  
  // Starts followWall state
  if (followWall == true)
  {
    // Calculates distance to wall
    sensorOneDist = readSonar(sensorOneTrig, sensorOneEcho);
    sensorTwoDist = readSonar(sensorTwoTrig, sensorTwoEcho);    
   
   // Getting close to first obstactle in front
    if (sensorOneDist < 25)
    {     
      sensorOneDist = readSonar(sensorOneTrig, sensorOneEcho);
      // Stop if dist is under 10
      if (sensorOneDist < 10)
      {
        // followWall state finished
        followWall = false;\
        // Starting findLine state
        findLine = true;
      }
      // If not close yet, keep driving straight
      else
      {
        driveStraight(100, 100);
      }
    }
    // To far away, move closer
    else if (sensorTwoDist > 15)
    {
      driveCloser(100, 100);
    }
    // To close, move away
    else if (sensorTwoDist < 10)
    {
      driveAway(140, 100);
    }
    // if sensor one dist is between 15 and 10 move straight
    else
    {
      driveStraight(100, 100);
    }
    delay(50);
  }
  // Start of findLine state
  else if (findLine == true)
  {
    // Pivot right towards black line
    pivotRight90();
    // Keep driving straight until hits black line
    while((valLeft < 800) || (valCenter < 800) || (valRight < 800))
    {
      driveStraight(100, 50);
      valLeft = leftSensor.read();
      valCenter = centerSensor.read();
      valRight = rightSensor.read();
    }
    // Pivot left to follow line
    pivotLeft90();
    // findLine state finsihed
    findLine = false;
    // followLine state started 
    followLine = true;
  }
  // Start of followLine state
  else if (followLine = true)
  {
    // Reads in left and right line sensor. Don't use center line sensor.
    int valLeft = leftSensor.read();
    int valRight = rightSensor.read();
    
    delay(200);
    
    // if left line sensor on line, drive robot back to left
    if (valLeft > 800)
    {
      motors.rightDrive(200);
      motors.leftDrive(75);
      delay(100);
      motors.brake();
    }
     // if right line sensor on line, drive robot back to right
    else if (valRight > 800)
    {
      motors.leftDrive(150);
      motors.rightDrive(75);
      delay(100);
      motors.brake();
    }
    // drive straight
    else
    {
      driveStraight(150, 50);
    }    
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

// Drive close function
// Takes two parameters power and time.
// One engine has increased power of 20 to slightly turn and get closer
void driveCloser(int power, int time)
{
  motors.rightDrive(power);
  motors.leftDrive((power*0.72) + 20);
  delay(time);
  motors.brake();
}

// Drive away function
// Takes two parameters power and time.
// One engine has increased power of 20 to slightly turn and get further away
void driveAway(int power, int time)
{
  motors.rightDrive(power + 20);
  motors.leftDrive(power*0.72);
  delay(time);
  motors.brake();
}

// Pivot right 90 funciton
void pivotRight90()
{
  motors.leftDrive(150);
  motors.rightDrive((-150)*0.63);
  delay(280);
  motors.brake();
  delay(20);
}

// Pivot left 90 function
void pivotLeft90()
{
  motors.rightDrive(150);
  motors.leftDrive((-150)*0.63);
  delay(340);
  motors.brake();
  delay(20);
}

// read sonar function
// Takes two parameters trigPin and echoPin
// returns the distance as a float
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
  
  // Recieve signal
  // Calculate duration
  float duration = pulseIn(echoPin, HIGH);
  
  // Return distance
  return duration / 58;
}
