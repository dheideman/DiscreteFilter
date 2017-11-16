# DiscreteFilter

Arduino library for implementing discrete-time filters.  This library allows for the implementation of time-invariant filters of arbitrary order with constant-coefficient discrete-time transfer functions.  Functions for quickly creating common filters(low-pass, high-pass filters) are included.

This library is still very much a work in progress.  Be aware that code-breaking updates may be unavoidable as features are added.

## Documentation

Coming Soon.

Note: The "createPIDController()" remains untested

## Examples

An examples folder is under development.

For the moment, here is an example of how the library can be used:
```
// Declare empty filter objects
DiscreteFilter lpfilter;  // Low-pass filter
DiscreteFilter hpfilter;  // High-pass filter

// Create complementary low- and high-pass filters
lpfilter.createFirstOrderLowPassFilter(FILTER_DT,FILTER_TAU);
hpfilter.createFirstOrderHighPassFilter(FILTER_DT,FILTER_TAU);

// Move the filters forward by one time step
// (should be performed once every FILTER_DT seconds)
float lowpassoutput = lpfilter.step(input);

// The most recent output can also be accessed with the "getLastOutput()" function
hpfilter.step(input);
float highpassoutput = hpfilter.getLastOutput();

// Custom filters/controllers can also be implemented
// (this example is identical to "createFirstOrderLowPassFilter()"
//  except that it includes filter saturation)
int   order = 1;                // 1st order filter
float saturation = 100.0;       // Don't output anything beyond +/- 100

float filterconst   = FILTER_DT/FILTER_TAU;
float numerator[]   = {filterconst, 0};
float denominator[] = {1, filterconst-1};
DiscreteFilter customlowpass(order, numerator, denominator, saturation);

// Sasturation can be added to any filter:
lpfilter.setSaturation(100.0);

// Now lpfilter and customlowpass are functionally identical

// Saturation can be disabled by setting it to 0 or a negative value
hpfilter.setSaturation(0);

// The filter inputs/outputs can be cleared:
lpfilter.clear();
hpfilter.clear();

// Various self-explanatory setters and getters exist:
hpfilter.setOrder(1);
hpfilter.setGain(1.0);
hpfilter.setNumerator(numerator);
hpfilter.setDenominator(denominator);

float input0  = hpfilter.getInput(0);
float output0 = hpfilter.getOutput(0);
float hpgain  = hpfilter.getGain();
int   hporder = hpfilter.getOrder();

```

## Installation

To install, download this library as a .zip or .tar.gz from github and place the extracted folder in your sketchbook's "libraries" folder
