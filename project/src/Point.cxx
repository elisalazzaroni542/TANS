#include "Point.h"


ClassImp(Point)
    // Default constructor
    Point::Point() : x(0), y(0), z(0) {}

    // Standard constructor
    Point::Point(double X, double Y, double Z) : x(X), y(Y), z(Z) {}

    void Point::Set(double X, double Y, double Z) {
        x = X;
        y = Y;
        z = Z;
    }

    // Destructor
    Point::~Point() {}
