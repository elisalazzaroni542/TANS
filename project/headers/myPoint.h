#ifndef MYPOINT_H
#define MYPOINT_H

#include <TObject.h>
#include <math.h>

class myPoint : public TObject {
public:
    myPoint();
    myPoint(double X, double Y, double Z, int ID);  
    myPoint(double X, double Y, double Z, double Phi, int ID);  
    void Set(double X, double Y, double Z, double Phi, int ID);
    void Set(double X, double Y, double Z, int ID);


    double GetX() const { return x; }
    double GetY() const { return y; }
    double GetZ() const { return z; }
    double GetPhi() const { return phi; }
    int GetId() const { return id; }
    
    virtual ~myPoint();

    ClassDef(myPoint, 1);

private:
    double x;  
    double y;  
    double z; 
    double phi;
    int id; 
};

#endif 
