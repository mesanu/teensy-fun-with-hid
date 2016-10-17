#include "curves.h"
#include <math.h>

Curve::Curve(){
  this->prevCoords.x=0;
  this->prevCoords.y=0;
  this->currentCoords.x=0;
  this->currentCoords.y=0;
  this->dCoords.x=0;
  this->dCoords.y=0;
  this->stepCount=0;
}

XYCoords Curve::deltaStep(){
  this->stepCount++;
  recalcCurrent();
  this->dCoords.x=this->currentCoords.x-prevCoords.x;
  this->dCoords.y=this->currentCoords.y-prevCoords.y;
  if(this->dCoords.x != 0 || this->dCoords.y != 0){
    this->prevCoords=this->currentCoords;
  }

  return dCoords;
}

void Curve::recalcCurrent(){
}

Circle::Circle(double radius,double freq){
  this->radius=radius;
  this->freq=freq;
}

void Circle::recalcCurrent(){
  this->stepCount++;
  this->currentCoords.x=radius*cos(freq*2*PI*(double)(stepCount)/1000);
  this->currentCoords.y=radius*sin(freq*2*PI*(double)(stepCount)/1000);
}

Fig8::Fig8(double a, double freq){
  this->a=a;
  this->freq=freq;
  this->thresh=(unsigned int)(1000/freq);
  this->sign=1;
}

void Fig8::recalcCurrent(){
  this->stepCount++;
  if((this->stepCount%this->thresh)==0){
    this->sign*=(-1);
  }
  this->currentCoords.x+=2*this->sign;
  this->currentCoords.y=a*sin(freq*2*PI*(double)(this->stepCount)/1000);
}
