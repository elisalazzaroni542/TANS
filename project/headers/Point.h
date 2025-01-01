#ifndef POINT_H
#define POINT_H

#include <TObject.h>

class Point : public TObject {
public:

    Point();
    Point(double x, double y, double z);  
    void Set(double x, double y, double z);  

    virtual ~Point();  // Destructor

    double Px;  
    double Py;  
    double Pz;  



    ClassDef(Point, 1);
};

#endif 