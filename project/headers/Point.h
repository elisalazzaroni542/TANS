#ifndef POINT_H
#define POINT_H

#include <TObject.h>

class Point : public TObject {
public:
    Point();
    Point(double X, double Y, double Z);  
    void Set(double X, double Y, double Z);

    double GetX() const { return x; }
    double GetY() const { return y; }
    double GetZ() const { return z; }

    virtual ~Point();  // Destructor

    ClassDef(Point, 1);

private:
    double x;  
    double y;  
    double z;  
};

#endif 
