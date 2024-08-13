#include "motor.h"

void setupMotorPins() {
  // Set motor direction pins as output
  DDRD |= (1 << MOTOR_A_DIR) | (1 << MOTOR_B_DIR);
  // Set PWM pins as output
  DDRD |= (1 << MOTOR_B_PWM) | (1 << MOTOR_B_PWM);

  TCCR0A |= (1 << WGM01) | (1 << WGM00);
  TCCR0A |= (1 << COM0A1) | (1 << COM0B1);  // Non-inverting mode
  TCCR0B |= (1 << CS00);  // No prescaler
}

void setupMotor(side_Wheel side, double powerFactor, direction_Value dir) {
    // Calculate PWM value based on the power factor
    uint8_t pwmValue = (uint8_t)(MAX_PWM / powerFactor);

    switch (side) {
        case MOTOR_LEFT:
            OCR0A = pwmValue;  // Set PWM for left motor
            PORTD = dir == DIRECTION_HIGH ? (PORTD | (1 << MOTOR_A_DIR)) : (PORTD & ~(1 << MOTOR_A_DIR));
            break;
        case MOTOR_RIGHT:
            OCR0B = pwmValue;  // Set PWM for right motor
            PORTD = dir == DIRECTION_HIGH ? (PORTD | (1 << MOTOR_B_DIR)) : (PORTD & ~(1 << MOTOR_B_DIR));
            break;
        case MOTOR_MID:
            // Assuming MOTOR_MID is an alias for one or both motors behaving identically
            OCR0B = pwmValue;  // This example just reuses right motor's settings
            PORTD = dir == DIRECTION_HIGH ? (PORTD | (1 << MOTOR_B_DIR)) : (PORTD & ~(1 << MOTOR_B_DIR));
            break;
        default:
            // Handle unexpected values
            break;
    }
}

void turnleft(int value) {
  Serial.println("Rotate Left > 45");  // d2:d4 == low;low

  setupMotor(MOTOR_LEFT, 2.5, DIRECTION_HIGH);   //d2 high
  setupMotor(MOTOR_RIGHT, 2.5, DIRECTION_HIGH);  //d4 low
  delay(value);
  setupMotor(MOTOR_LEFT, MAX_PWM + 1, DIRECTION_LOW);   //d2 high
  setupMotor(MOTOR_RIGHT, MAX_PWM + 1, DIRECTION_LOW);  //d4 high

  stopMotor();
}

void turnright(int value) {
  setupMotor(MOTOR_LEFT, 2.5, DIRECTION_LOW);   //d2 high
  setupMotor(MOTOR_RIGHT, 2.5, DIRECTION_LOW);  //d4 high
  delay(value);
  setupMotor(MOTOR_LEFT, MAX_PWM + 1, DIRECTION_HIGH);   //d2 low
  setupMotor(MOTOR_RIGHT, MAX_PWM + 1, DIRECTION_HIGH);  //d4 low

  stopMotor();
}

void moveforward(int value) {
  setupMotor(MOTOR_LEFT, 3.5, DIRECTION_LOW);  //d2 high
  setupMotor(MOTOR_RIGHT, 3.5, DIRECTION_HIGH);  //d4 low
  delay(value);
  stopMotor();
}

void stopMotor() {
  OCR0A = 0;
  OCR0B = 0;
}

