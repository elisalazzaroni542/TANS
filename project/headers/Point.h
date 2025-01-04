#ifndef POINT_H
#define POINT_H

#include <TObject.h>
#include <math.h>

class Point : public TObject {
public:
    Point();
    Point(double X, double Y, double Z, unsigned int ID);  
    void Set(double X, double Y, double Z, unsigned int ID);

    double GetX() const { return x; }
    double GetY() const { return y; }
    double GetZ() const { return z; }
    unsigned int GetId() const { return id; }
    
    double getPhi(double rCil);

    virtual ~Point();

    ClassDef(Point, 1);

private:
    double x;  
    double y;  
    double z; 
    unsigned int id; 
};

#endif 
