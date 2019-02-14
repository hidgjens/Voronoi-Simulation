#include "Point.h"

// constructors
Point::Point(){}
Point::Point(Cart pPos) : pitchPos(pPos) {}
Point::Point(Cart pPos, double area) : pitchPos(pPos) , A(area) {}

Point::Point(Cart pPos, double homeDist, double awayDist) 
: pitchPos(pPos) , A(1.0) { calcAreas(homeDist, awayDist); }
Point::Point(Cart pPos, double homeDist, double awayDist, double area) 
: pitchPos(pPos) , A(area) { calcAreas(homeDist, awayDist); }

Point::Point(Cart pPos, Cart homePos, Cart awayPos) 
: pitchPos(pPos) , A(1.0) { calcAreas(homePos, awayPos); }
Point::Point(Cart pPos, Cart homePos, Cart awayPos, double area) 
: pitchPos(pPos) , A(area) { calcAreas(homePos, awayPos); }

// accessors
Cart Point::getPos() const { return pitchPos; }
double Point::getHomeArea() const { return A_h; }
double Point::getAwayArea() const { return A_a; }
double Point::getArea() const { return A; }

// mutators
void Point::setPos(Cart newPos) {
    pitchPos = newPos;
}
void Point::setPos(double xComp, double yComp) {
    pitchPos = Cart(xComp, yComp);
}
void Point::setHomeArea(double hmA) {
    A_h = hmA;
}
void Point::setAwayArea(double awA) {
    A_a = awA;
}
void Point::setAreas(double hmA, double awA) {
    setHomeArea(hmA);
    setAwayArea(awA);
}
void Point::calcAreas(double x_h, double x_a) {
    double a_h, a_a;
    if (x_h == 0.0 && x_a == 0.0){
        // need to catch divide by zero error, if both distances are zero, just divide by 2
        a_h = a_a = 0.5 * A;
    } else {
        a_h = A * x_a / (x_h + x_a);
        a_a = A * x_h / (x_h + x_a);
    }
    setAreas(a_h, a_a);
}
void Point::calcAreas(Cart posH, Cart posA) {
    double x_h = posH.dist(pitchPos);
    double x_a = posA.dist(pitchPos);
    calcAreas(x_h, x_a);
}
