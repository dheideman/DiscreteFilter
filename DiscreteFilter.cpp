/*
  DiscreteFilter.cpp - Library for discrete-time siso filter implementation
  Generic SISO filter.
  Created by Daniel A. Heideman, February 2017.
-------------------------------
The MIT License (MIT)

Copyright (c) 2017 Daniel Heideman

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

#include "Arduino.h"
#include "DiscreteFilter.h"

//////////////////
// Constructors //
//////////////////

/*******************************************************************************
 * Bare Bones Constructor
 * DiscreteFilter()
 *
 * Default order = 1;
 *
 * Makes a completely empty filter.
 * Mostly used to make common filters.
 ******************************************************************************/
DiscreteFilter::DiscreteFilter()
{
  _order   = 1;
  _gain    = 1.0;
  _sat     = 0.0;
  _num     = new float[_order+1];
  _den     = new float[_order+1];
  _inputs  = new RingBuffer(_order+1);
  _outputs = new RingBuffer(_order+1);
  float zeros[] = {0,0};

  this->setNumerator(zeros);
  this->setDenominator(zeros);
  this->clear();
}

/*******************************************************************************
 * Constructor
 * DiscreteFilter(int order, float num[], float den[])
 *
 * Makes a filter based on given parameters.
 * Should have empty input/output buffers.
 ******************************************************************************/
DiscreteFilter::DiscreteFilter(int order, float num[], float den[])
{
  _order   = order;
  _gain    = 1.0;
  _sat     = 0.0;
  _num     = new float[_order+1];
  _den     = new float[_order+1];
  _inputs  = new RingBuffer(_order+1);
  _outputs = new RingBuffer(_order+1);

  this->setNumerator(num);
  this->setDenominator(den);
  this->clear();
}

/*******************************************************************************
 * Constructor
 * DiscreteFilter(int order, float num[], float den[], float sat)
 *
 * Makes a filter based on given parameters.
 * Should have empty input/output buffers.
 * Includes saturation
 ******************************************************************************/
DiscreteFilter::DiscreteFilter(int order, float num[], float den[], float sat)
{
  _order   = order;
  _gain    = 1.0;
  _sat     = sat;
  _num     = new float[_order+1];
  _den     = new float[_order+1];
  _inputs  = new RingBuffer(_order+1);
  _outputs = new RingBuffer(_order+1);

  this->setNumerator(num);
  this->setDenominator(den);
  this->clear();
}

/*******************************************************************************
 * Destructor (I hope...)
 *
 * This should free up the memory used by _num and _den.
 ******************************************************************************/
DiscreteFilter::~DiscreteFilter()
{
  delete [] _num;
  delete [] _den;
  delete _inputs;
  delete _outputs;
}

//////////////////////////
// Main Filter Function //
//////////////////////////

/*******************************************************************************
 * float step()
 *
 * The one we're all here for: execute one step of the filter.
 * Returns the result and also saves output to _outputs buffer.
 ******************************************************************************/
float DiscreteFilter::step(float input)
{
  // Input new input into _input ring buffer
  _inputs->addValue(input);

  // Calculate new output;
  float newoutput = 0.0;

  // Deal with numerator first
  for(int i=0; i<_order+1; i++)
  {
    newoutput += _gain*_num[i]*_inputs->getValue(i);
  }

  // Deal with denominator next
  for(int i=1; i<_order+1; i++)
  {
    newoutput -= _den[i]*_outputs->getValue(i-1);
  }

  // Divide by coefficient of first term in the denominator, in case it isn't 1
  newoutput = newoutput/_den[0];

  // Check if saturation has been enabled
  if(_sat > 0)
  {
    // Check for positive saturation
    if(newoutput > _sat)
      newoutput = _sat;
    // Check for negative saturation
    if(newoutput < -1*_sat)
      newoutput = -1*_sat;
  }

  _outputs->addValue(newoutput);          // Save new output to _outputs
  return newoutput;                       // Return the new output as well
}

//////////////////////////////
// Useful Filter Generators //
//////////////////////////////

/*******************************************************************************
 * void createFirstOrderLowPassFilter(float dt, float tau)
 *
 * Create a 1st order low-pass filter
 *   tau  - Time constant (s)
 *   dt   - Time between executions (1/frequency) (s)
 ******************************************************************************/
void  DiscreteFilter::createFirstOrderLowPassFilter(float dt, float tau)
{
  float filterconst = dt/tau;
  float newnum[] = {filterconst, 0};
  float newden[] = {1, filterconst-1};

  // Start filling in the filter
  this->setOrder(1);
  this->setGain(1.0);
  this->setNumerator(newnum);
  this->setDenominator(newden);
}

/*******************************************************************************
 * void createFirstOrderHighPassFilter(float dt, float tau)
 *
 * Create a 1st order high-pass filter
 *   tau  - Time constant (s)
 *   dt   - Time between executions (1/frequency) (s)
 ******************************************************************************/
void  DiscreteFilter::createFirstOrderHighPassFilter(float dt, float tau)
{
  float filterconst = dt/tau;
  float newnum[] = {1-filterconst, filterconst-1};
  float newden[] = {1, filterconst-1};

  // Start filling in the filter
  this->setOrder(1);
  this->setGain(1.0);
  this->setNumerator(newnum);
  this->setDenominator(newden);
}

/*******************************************************************************
 * void createLeadLagCompensator(float dt, float taun, float taup)
 *
 * Create a simple lead-lag compensator
 *   taun - Time constant (lead) (s)
 *   taup - Time constant (lag)  (s)
 *   dt   - Time between executions (1/frequency) (s)
 *
 * Based on:
 * http://www.informit.com/articles/article.aspx?p=32090&seqNum=8
 ******************************************************************************/
void  DiscreteFilter::createLeadLagCompensator(float dt, float taun, float taup)
{
  float kn = 2.0f*taun/dt;
  float kp = 2.0f*taup/dt;

  float newnum[] = {kn, 1.0f, 1.0f - kn};
  float newden[] = {kp, 1.0f, 1.0f - kp};

  this->setOrder(2);
  this->setGain(1.0f);
  this->setNumerator(newnum);
  this->setDenominator(newden);
}

///////////////////
// Set Functions //
///////////////////

/*******************************************************************************
 * void setOrder(int order)
 *
 * Set the order of an existing filter, reset arrays and buffers to new size
 ******************************************************************************/
void  DiscreteFilter::setOrder(int order)
{
  _order   = order;
  _num     = new float[_order+1];
  _den     = new float[_order+1];
  _inputs  = new RingBuffer(_order+1);
  _outputs = new RingBuffer(_order+1);

  for(int i=0; i<_order+1; i++)
  {
    _num[i] = 0;
    _den[i] = 0;
  }
}

/*******************************************************************************
 * void setGain(float gain)
 *
 * Set the gain of an existing filter
 ******************************************************************************/
void DiscreteFilter::setGain(float gain)
{
  _gain = gain;
}

/*******************************************************************************
 * void setNumerator(float num[])
 *
 * Set the numerator of an existing filter
 ******************************************************************************/
void  DiscreteFilter::setNumerator(float num[])
{
  for(int i=0; i<_order+1; i++)
  {
    _num[i] = num[i];
  }
}

/*******************************************************************************
 * void setDenominator(float den[])
 *
 * Set the denominator of an existing filter
 ******************************************************************************/
void  DiscreteFilter::setDenominator(float den[])
{
  for(int i=0; i<_order+1; i++)
  {
    _den[i] = den[i];
  }
}

/*******************************************************************************
 * void setSaturation(float sat)
 *
 * Set the saturation of an existing filter
 ******************************************************************************/
void DiscreteFilter::setSaturation(float sat)
{
  _sat = sat;
}

/*******************************************************************************
 * void clear()
 *
 * Clear the input and output buffers.
 ******************************************************************************/
void  DiscreteFilter::clear()
{
  _inputs->clear();
  _outputs->clear();
}

///////////////////
// Get Functions //
///////////////////

/*******************************************************************************
 * float getInput()
 *
 * Returns the requested input.  0 is the most recent, -1 is the oldest
 ******************************************************************************/
float DiscreteFilter::getInput(int index)
{
  return _inputs->getValue(index);
}

/*******************************************************************************
 * float getOutput()
 *
 * Returns the requested output. 0 is the most recent, -1 is the oldest
 ******************************************************************************/
float DiscreteFilter::getOutput(int index)
{
  return _outputs->getValue(index);
}

/*******************************************************************************
 * float getLastOutput()
 *
 * Easily get the last output from the filter
 ******************************************************************************/
float DiscreteFilter::getLastOutput()
{
  return _outputs->getValue(0);
}

/*******************************************************************************
 * int getOrder()
 *
 * Returns the order of the filter
 ******************************************************************************/
int   DiscreteFilter::getOrder()
{
  return _order;
}

/*******************************************************************************
 * float getGain()
 *
 * Returns the gain of the filter
 ******************************************************************************/
float DiscreteFilter::getGain()
{
  return _gain;
}
