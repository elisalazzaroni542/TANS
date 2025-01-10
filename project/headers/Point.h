#ifndef POINT_H
#define POINT_H

#include <TObject.h>
#include <math.h>

class Point : public TObject {
public:
    Point();
    Point(double X, double Y, double Z, int ID);  
    void Set(double X, double Y, double Z, int ID);

//    Point(double X, double Y, double Z);  
//    void Set(double X, double Y, double Z);


    double GetX() const { return x; }
    double GetY() const { return y; }
    double GetZ() const { return z; }
    unsigned int GetId() const { return id; }
    
    double GetPhi(double rCil);

    virtual ~Point();

    ClassDef(Point, 1);

private:
    double x;  
    double y;  
    double z; 
    int id; 
};

#endif 
