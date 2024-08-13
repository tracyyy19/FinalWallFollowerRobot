#include "ultrasonicservo.h"

void setServoPosition(int pulseWidth)
{
  unsigned long lastPulseTime = micros();
  int count = 0;

  // Generate 50 pulses to ensure the servo has ample time to reach the position
  while (count < 50)
  {
    if (micros() - lastPulseTime >= SERVO_PERIOD)
    {                                // Check if it's time for the next pulse
      digitalWrite(SERVO_PIN, HIGH); // Start a new pulse
      delayMicroseconds(pulseWidth); // Length of the pulse sets the servo position
      digitalWrite(SERVO_PIN, LOW);  // End the pulse

      lastPulseTime = micros(); // Reset the timer at the end of the pulse
      count++;
    }
  }
}

double checkDistance()
{
  double measurements[5]; // Array to store multiple measurements
  for (int i = 0; i < 5; i++)
  {
    // Trigger the sensor
    PORTB |= (1 << TRIGGER_PIN);
    _delay_us(2);
    PORTB &= ~(1 << TRIGGER_PIN);
    _delay_us(10);
    // Wait for echo start
    while (!(PINB & (1 << ECHO_PIN)))
      ;
    TCNT1 = 0; // timer
    // Wait for echo end
    while (PINB & (1 << ECHO_PIN))
      ;
    measurements[i] = TCNT1 * 0.02700787;
    _delay_ms(10); // Small delay between measurements
  }
  // Compute the average or median
  return median(measurements, 5); // Implement this function or use an average
}

double median(double arr[], int n)
{
  // Sort the array (using a simple bubble sort)
  for (int i = 0; i < n - 1; i++)
  {
    for (int j = 0; j < n - i - 1; j++)
    {
      if (arr[j] > arr[j + 1])
      {
        // Swap elements
        double temp = arr[j];
        arr[j] = arr[j + 1];
        arr[j + 1] = temp;
      }
    }
  }

  // Find the median
  if (n % 2 != 0)
  { // Odd number of elements
    return arr[n / 2];
  }
  else
  { // Even number of elements
    return (arr[(n - 1) / 2] + arr[n / 2]) / 2.0;
  }
}

void setupUltra()
{
  // Set trigger pin as output
  DDRB |= (1 << TRIGGER_PIN);
  // Clear Trigger
  PORTB &= ~(1 << TRIGGER_PIN);
  // Set echo pin as input
  DDRB &= ~(1 << ECHO_PIN);

  TCCR1B = (1 << WGM13) | (1 << WGM12);

  TCCR1B = (1 << WGM12);               // Configure for CTC mode
  TCCR1B |= (1 << CS11) | (1 << CS10); // Set prescaler to 64
  OCR1A = 4999;                        // Set up for a specific frequency if necessary

  OCR1A = 4999; // Set frequency to 50 Hz
}