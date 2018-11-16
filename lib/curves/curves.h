#ifndef SHAPES_H
#define PI 3.14159

#include <mbed.h>
#include "USBMouseKeyboard.h"

typedef struct coords {
  double x;
  double y;
} XYCoords;

class Curve {
  public:
    Curve();
    void move_mouse(USBMouseKeyboard *keyboard);
  protected:
    XYCoords prevCoords;
    XYCoords currentCoords;
    XYCoords dCoords;
    void deltaStep();
    virtual void recalcCurrent() = 0;
    unsigned long long stepCount;
};

class Circle: public Curve {
  public:
    Circle(double radius, double freq);
  private:
    void recalcCurrent();
    double radius;
    double freq;
};

class Fig8: public Curve {
  public:
    Fig8(double a, double freq);
  private:
    unsigned int thresh;
    int sign;
    void recalcCurrent();
    double a;
    double freq;
};

#endif
