/*
 * FilteredSquareWave.ino
 * A program to test the DiscreteFilter library's low- and high-pass filters
 * Created by Daniel A. Heideman, February 2017
 * 
 * Best viewed using serial plotter tool
 */
#include <DiscreteFilter.h>

// Filter time constants (s)
#define FILTER_TAU    1.0
#define FILTER_DT     0.02

// Serial baud rate
#define BAUD_RATE     115200

// Input square wave frequency (hz)
#define INPUT_FREQ    0.20

// Declare variables
unsigned long cycletime = 0;
float input = 0;
float lowpassoutput  = 0;
float highpassoutput = 0;

// Declare filter objects
DiscreteFilter lpfilter;  // Low-pass filter
DiscreteFilter hpfilter;  // High-pass filter

void setup()
{
  // Create complementary low- and high-pass filters
  lpfilter.createFirstOrderLowPassFilter(FILTER_DT,FILTER_TAU);
  hpfilter.createFirstOrderHighPassFilter(FILTER_DT,FILTER_TAU);

  // Initialize serial connection
  Serial.begin(BAUD_RATE);
}

void loop()
{
  // Check if it's time to change the input
  if( millis() > cycletime )
  {
    // set time at which to change input again
    cycletime = millis() + 1000.0/INPUT_FREQ;

    // toggle input
    if( input == 0 )
      input = 1.0;
    else
      input = 0.0;
  }

  // Step filters forward once
  lowpassoutput  = lpfilter.step(input);
  highpassoutput = hpfilter.step(input);

  // Print input and filter outputs to screen in a nice format
  Serial.print(input);
  Serial.print(",");
  Serial.print(lowpassoutput);
  Serial.print(",");
  Serial.println(highpassoutput);

  delay(1000*FILTER_DT);
}
