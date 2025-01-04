#include "Point.h"


ClassImp(Point)
    // Default constructor
    Point::Point() : x(0), y(0), z(0) {}

    // Standard constructor
    Point::Point(double X, double Y, double Z, unsigned int ID) : x(X), y(Y), z(Z), id(ID) {}

    // Destructor
    Point::~Point() {}


    void Point::Set(double X, double Y, double Z, unsigned int ID) {
        x = X;
        y = Y;
        z = Z;
        id = ID;
    }

    double Point::getPhi(double rCil){
        return asin(y/rCil);
    }
