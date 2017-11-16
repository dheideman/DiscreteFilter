/*
  RingBuffer.cpp - Dynamically sizable ring buffers for DiscreteFilters library
  Created by Daniel A. Heideman, February 2017.
*/

#include "Arduino.h"
#include "DiscreteFilter.h"


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