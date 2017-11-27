/*
  DiscreteFilter.h - Library for discrete-time siso filter implementation
  Created by Daniel A. Heideman, February 2017.
  Inspired by Strawson Design's Robotics Cape Installer:
  https://github.com/StrawsonDesign/Robotics_Cape_Installer
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

#ifndef DiscreteFilter_h
#define DiscreteFilter_h

#include "Arduino.h"

class DiscreteFilter
{

  public:
    DiscreteFilter();
    DiscreteFilter(int order, float num[], float den[]);
    DiscreteFilter(int order, float num[], float den[], float sat);
    ~DiscreteFilter();
    float step(float input);
    void  createFirstOrderLowPassFilter(float dt, float tau);
    void  createFirstOrderHighPassFilter(float dt, float tau);
    void  createLeadLagCompensator(float dt, float taun, float taup);
    void  setOrder(int order);
    void  setGain(float gain);
    void  setNumerator(float num[]);
    void  setDenominator(float den[]);
    void  setSaturation(float sat);
    void  clear();
    float getInput(int index);
    float getOutput(int index);
    float getLastOutput();
    float getGain();
    int   getOrder();

  private:
    class RingBuffer;

    int     _order;
    float   _gain;
    float   _sat;
    float*  _num;
    float*  _den;

    RingBuffer*  _inputs;
    RingBuffer*  _outputs;
};


class DiscreteFilter::RingBuffer
{
  public:
  RingBuffer();
  RingBuffer(int n);
  ~RingBuffer();
  float getValue(int index);
  void  addValue(float newvalue);
  int   getLength();
  void  setLength(int n);
  void  clear();
  private:
  int    _length;
  int    _pos;
  float* _data;
  int    convertIndex(int index);
};

#endif
