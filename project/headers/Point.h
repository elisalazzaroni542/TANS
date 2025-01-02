#ifndef POINT_H
#define POINT_H

#include <TObject.h>

class Point : public TObject {
public:
    Point();
    Point(double x, double y, double z);  
    void Set(double x, double y, double z);

    double GetX() const { return Px; }
    double GetY() const { return Py; }
    double GetZ() const { return Pz; }

    virtual ~Point();  // Destructor

    ClassDef(Point, 1);

private:
    double Px;  
    double Py;  
    double Pz;  
};

#endif 
