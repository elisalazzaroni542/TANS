#ifndef POINT_H
#define POINT_H

#include <TObject.h>
#include <math.h>

class Point : public TObject {
public:
    Point();
    Point(double X, double Y, double Z, int ID);  
    Point(double X, double Y, double Z, double Phi, int ID);  
    void Set(double X, double Y, double Z, double Phi, int ID);
    void Set(double X, double Y, double Z, int ID);


    double GetX() const { return x; }
    double GetY() const { return y; }
    double GetZ() const { return z; }
    double GetPhi() const { return phi; }
    int GetId() const { return id; }
    
    virtual ~Point();

    ClassDef(Point, 1);

private:
    double x;  
    double y;  
    double z; 
    double phi;
    int id; 
};

#endif 
