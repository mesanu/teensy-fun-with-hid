#ifndef SHAPES_H
#define PI 3.14159
typedef struct coords {
  int x;
  int y;
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
    void recalcCurrent();
  private:
    double radius;
    double freq;
};

#endif
