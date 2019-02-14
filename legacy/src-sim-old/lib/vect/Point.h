#ifndef POINT_H
#define POINT_H

#include "Cart.h"

class Point {
/* 
    A point represent an element of area on the pitch. A point is divided
    between players depending on how much 'influence' they have over the
    point. For each point, the closest two players from each team will be
    considered. The point will be divided amongst the players dependent on
    the inverse of their distance to the point. So if home player is distance
    X_h from point P with area A, and similarly X_a for an away player, then the areas 
    (A_h, A_a) is split proportional to (1/X_h) and (1/X_a) respectively 
    such that A_h + A_a = A     i.e. 
    
    A_h = A * (1/X_h) / (1/X_h + 1/X_a) = A * X_a / (X_h + X_a)
    A_a = A * X_h / (X_h + X_a) = A - A_h
*/
private:
    Cart pitchPos;
    //double X_h, X_a; // home distance, away distance -> possible not needed, saves space at least
    double A_h, A_a; // home area, away area
    double A; // total area, usually just equal to one, but just incase

public:
    // constructors
    Point(); // default
    Point(Cart); // pos
    Point(Cart, double); // pos, A
    Point(Cart, double, double); // pos, X_h, X_a, A = 1
    Point(Cart, double, double, double); // pos, X_h, X_a, A
    Point(Cart, Cart, Cart); // pos, pos_h, pos_a, A = 1
    Point(Cart, Cart, Cart, double); // pos, pos_h, pos_a, A

    // accessors
    Cart getPos() const;
    double getHomeArea() const;
    double getAwayArea() const;
    double getArea() const;

    // mutators
    void setPos(Cart);
    void setPos(double, double);
    void setHomeArea(double);
    void setAwayArea(double);
    void setAreas(double, double);
    void calcAreas(double, double); // X_h, X_a
    void calcAreas(Cart, Cart); // pos_h, pos_a

};

#endif