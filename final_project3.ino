#include <Arduino.h>

#include <avr/io.h>
#include <util/delay.h>

#include "ultrasonicservo.h"
#include "motor.h"

#define TRIGGER_PIN PB4 // PIN 12 trigger on sensor
#define ECHO_PIN PB5     // PIN 13 echo on sensor
#define SERVO_PIN 10
#define F_CPU 16000000UL

// For motor control pins
#define MOTOR_A_DIR PD2
#define MOTOR_A_PWM PD5
#define MOTOR_B_DIR PD4
#define MOTOR_B_PWM PD6

void setup(){
  Serial.begin(9600);
  setupMotorPins();
  setupUltra();
  Serial.print("Starting...");
}

void loop(){
  double frontDistance = checkFrontDistance();
  Serial.print("FRONT: ");
  Serial.println(frontDistance);

  if (frontDistance <= 8) {
    Serial.println("Near Front Wall");
    turnleft(300);  // Full rotate
  }

  double rightDistance = checkRightDistance();
  Serial.print("RIGHT: ");
  Serial.println(rightDistance);
  if (rightDistance > 20) {
    cornerDetected();
  }

  followWall(0.3,8, MOTOR_RIGHT);
}

// void moveCloserToRightWall() {
//   Serial.println("Right: too far.");
//   turnRight();
//   moveForward();
// }

// void tooCloseToFront(int speed, int duration) {
//   Serial.println("Too close to front");
//   turnLeft(300);
//   stopMotor();
// }

// void tooCloseToRightWall(int speed, int duration) {
//   Serial.println("Too close to right wall. Adjusting position.");
//   turnLeft(speed, duration);
//   moveForward(300);  // Move forward slowly
// }

bool hasturnedRight = false;
void followWall(double powerFactor, double maintainDistance, side_Wheel side) {
  double distance = checkDistance();
  delay(500);
  Serial.print("Distance at Right: ");
  Serial.println(distance);
  if (distance > 20) {
    distance = maintainDistance;
  }
  if ((side == MOTOR_RIGHT) && (hasturnedRight == false)) {
    setupMotor(MOTOR_RIGHT, (1 / (powerFactor * (distance / maintainDistance))), DIRECTION_LOW);  //d4 low   percentPower * (distance / maintainDistance
    setupMotor(MOTOR_LEFT, (1 / (powerFactor * (maintainDistance / distance))), DIRECTION_HIGH);  //d2 high percentPower * (maintainDistance / distance)
  }
  hasturnedRight = true;
  delay(60);
  stopMotor();
}


void cornerDetected() {
  moveforward(100);
  turnright(150);
  moveforward(100);
  turnright(150);
  
  delay(250);
  stopMotor();
}

double checkRightDistance() {
  setServoPosition(350);  // Point sensor to the right side
  delay(60);
  return checkDistance();
}

double checkFrontDistance() {
  setServoPosition(1400); // Point sensor forward
  delay(60);
  return checkDistance();
}
