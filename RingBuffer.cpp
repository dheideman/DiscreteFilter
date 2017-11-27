/*
  RingBuffer.cpp - Dynamically sizable ring buffers for DiscreteFilters library
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
 * Default Constructor
 * RingBuffer()
 * 
 * Default to length 1
 ******************************************************************************/
DiscreteFilter::RingBuffer::RingBuffer()
{
  _length = 1;
  _data = new float [_length];
  this->clear();
}

/*******************************************************************************
 * Useful Constructor
 * RingBuffer(int n)
 * 
 * Create proper ring buffer.
 ******************************************************************************/
DiscreteFilter::RingBuffer::RingBuffer(int n)
{
  _length = n;
  _data = new float [_length];
  this->clear();
}

/*******************************************************************************
 * Destructor
 * ~RingBuffer()
 * 
 * Destroy RingBuffer (hopefully)
 ******************************************************************************/
DiscreteFilter::RingBuffer::~RingBuffer()
{
  delete [] _data;
}

///////////////
// Functions //
///////////////

/*******************************************************************************
 * float getValue(int index)
 * 
 * Returns the value stored at index "index" (most recent = 0)
 ******************************************************************************/
float DiscreteFilter::RingBuffer::getValue(int index)
{
  return _data[this->convertIndex(index)];
}

/*******************************************************************************
 * void addValue(float newvalue)
 * 
 * Adds a new value to the front of the RingBuffer
 ******************************************************************************/
void DiscreteFilter::RingBuffer::addValue(float newvalue)
{
  _pos = this->convertIndex(-1);
  _data[_pos] = newvalue;
}

/*******************************************************************************
 * int getLength()
 * 
 * Returns the length of the RingBuffer
 ******************************************************************************/
int DiscreteFilter::RingBuffer::getLength()
{
  return _length;
}

/*******************************************************************************
 * void setLength(int n)
 * 
 * Resets the RingBuffer at a new length, n
 ******************************************************************************/
void DiscreteFilter::RingBuffer::setLength(int n)
{
  _length = n;
  _data = new float [n];
  this->clear();
}

/*******************************************************************************
 * void clear()
 * 
 * Clears the data stored in RingBuffer, resets all to 0
 ******************************************************************************/
void DiscreteFilter::RingBuffer::clear()
{
  for (int i=0; i<_length; i++)
  {
    _data[i] = 0;
  }
  _pos = 0;
}

///////////////////////
// Private Functions //
///////////////////////

/*******************************************************************************
 * int convertIndex(int index)
 * private
 * 
 * Converts the given index to an array index to retrieve values from data array
 * index 0 = most recent
 * index -1 = index (_length) = oldest
 ******************************************************************************/
int DiscreteFilter::RingBuffer::convertIndex(int index)
{
  int n = _pos - index;
  if (n<0) n += _length;
  return n % _length;
}