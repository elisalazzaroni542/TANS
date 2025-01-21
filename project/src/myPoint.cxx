#include "myPoint.h"


ClassImp(myPoint)
    myPoint::myPoint() : x(0), y(0), z(0), phi(0), id(-1) {}

    myPoint::myPoint(double X, double Y, double Z, int ID) : x(X), y(Y), z(Z), id(ID) {}
    myPoint::myPoint(double X, double Y, double Z, double Phi, int ID) : x(X), y(Y), z(Z), phi(Phi), id(ID) {}
    myPoint::~myPoint() {}


    void myPoint::Set(double X, double Y, double Z, double Phi, int ID) {
        x = X;
        y = Y;
        z = Z;
        phi = Phi;
        id = ID;
    }


    void myPoint::Set(double X, double Y, double Z, int ID) {
        x = X;
        y = Y;
        z = Z;
        id = ID;
    }
