#include "Point.h"


ClassImp(Point)
    // Default constructor
    Point::Point() : x(0), y(0), z(0) {}

    // Standard constructor
    Point::Point(double X, double Y, double Z, int ID) : x(X), y(Y), z(Z), id(ID) {}
    //Point::Point(double X, double Y, double Z) : x(X), y(Y), z(Z) {}
    // Destructor
    Point::~Point() {}


    void Point::Set(double X, double Y, double Z, int ID) {
        x = X;
        y = Y;
        z = Z;
        id = ID;
    }

    double Point::GetPhi(double rCil){
        return asin(y/rCil);
    }
