#include "Point.h"


ClassImp(Point)
    // Default constructor
    Point::Point() : Px(0), Py(0), Pz(0) {}

    // Standard constructor
    Point::Point(double x, double y, double z) : Px(x), Py(y), Pz(z) {}

    void Point::Set(double x, double y, double z) {
        Px = x;
        Py = y;
        Pz = z;
    }

    // Destructor
    Point::~Point() {}
