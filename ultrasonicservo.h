#ifndef ULTRASONICSERVO__H
#define ULTRASONICSERVO_H

#include <Arduino.h>
#include <avr/io.h>
#include <util/delay.h>

#define SERVO_PERIOD 20000  // Microseconds (20 milliseconds)

#define TRIGGER_PIN PB4 // PIN 12 trigger on sensor
#define ECHO_PIN PB5     // PIN 13 echo on sensor
#define SERVO_PIN 10

double checkDistance();
double median(double arr[], int n);
void setServoPosition(int pulseWidth);
void setupUltra();

#endif