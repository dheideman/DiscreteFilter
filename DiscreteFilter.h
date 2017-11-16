/*
  DiscreteFilter.h - Library for discrete-time siso filter implementation
  Created by Daniel A. Heideman, February 2017.
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
