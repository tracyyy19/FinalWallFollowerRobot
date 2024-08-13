#ifndef MOTOR_H
#define MOTOR_H

#include <Arduino.h>
#include <avr/io.h>
#include <util/delay.h>

// For motor control pins
#define MOTOR_A_DIR PD2
#define MOTOR_A_PWM PD5
#define MOTOR_B_DIR PD4
#define MOTOR_B_PWM PD6

#define MAX_PWM 255  // full range of 8-bit PWM

// Enum definitions for selecting motor side and direction
typedef enum {
  MOTOR_LEFT,
  MOTOR_RIGHT,
  MOTOR_MID  // Renamed for consistency
} side_Wheel;

typedef enum {
  DIRECTION_HIGH,
  DIRECTION_LOW  // Renamed for clarity
} direction_Value;

void setupMotorPins();
void setupMotor(side_Wheel side, double powerFactor, direction_Value dir);

void turnleft(int value);
void turnright(int value);
void moveforward(int value);
void stopMotor();

#endif