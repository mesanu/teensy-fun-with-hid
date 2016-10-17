#ifndef SHAPES_H
#define PI 3.14159
typedef struct coords {
  double x;
  double y;
} XYCoords;

class Curve {
  public:
    Curve();
    XYCoords deltaStep();
  protected:
    XYCoords prevCoords;
    XYCoords currentCoords;
    XYCoords dCoords;
    virtual void recalcCurrent();
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
