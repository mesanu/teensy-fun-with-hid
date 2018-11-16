#include <mbed.h>
#include "curves.h"
#include "USBMouseKeyboard.h"
#include <math.h>

Curve::Curve(){
  prevCoords.x=0;
  prevCoords.y=0;
  currentCoords.x=0;
  currentCoords.y=0;
  dCoords.x=0;
  dCoords.y=0;
  stepCount=0;
}

void Curve::move_mouse(USBMouseKeyboard *keyboard){
  deltaStep();
  keyboard->move(currentCoords.x, currentCoords.y);
}

void Curve::deltaStep(){
  stepCount++;
  recalcCurrent();
  dCoords.x=currentCoords.x-prevCoords.x;
  dCoords.y=currentCoords.y-prevCoords.y;
  if(dCoords.x != 0 || dCoords.y != 0){
    prevCoords=currentCoords;
  }
}

Circle::Circle(double radius,double freq){
  this->radius=radius;
  this->freq=freq;
}

void Circle::recalcCurrent(){
  stepCount++;
  currentCoords.x=radius*cos(freq*2*PI*(double)(stepCount)/1000);
  currentCoords.y=radius*sin(freq*2*PI*(double)(stepCount)/1000);
}

Fig8::Fig8(double a, double freq){
  this->a=a;
  this->freq=freq;
  this->thresh=(unsigned int)(1000/freq);
  this->sign=1;
}

void Fig8::recalcCurrent(){
  stepCount++;
  if((stepCount%thresh)==0){
    sign*=(-1);
  }
  currentCoords.x+=2*sign;
  currentCoords.y=a*sin(freq*2*PI*(double)(stepCount)/1000);
}
