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
    ~DiscreteFilter();
    float step(float input);
    void  createFirstOrderLowPassFilter(float dt, float tau);
    void  createFirstOrderHighPassFilter(float dt, float tau);
    void  setOrder(int order);
    void  setGain(float gain);
    void  setNumerator(float num[]);
    void  setDenominator(float den[]);
    void  clear();
    float getInput(int index);
    float getOutput(int index);
    float getLastOutput();
    float getGain();
    int   getOrder();

  private:
    int     _order;
    float   _gain;
    float*  _num;
    float*  _den;
    float*  _inputs;
    float*  _outputs;
    int     _pos;
    float   getBufferValue(float* buffer, int index);
    void    addValueToBuffer(float* buffer, float value);
    int     convertIndex(int index);
};

#endif