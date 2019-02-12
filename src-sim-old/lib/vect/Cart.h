#ifndef CART_H
#define CART_H

#include<iostream>
#include<string>
#include<cmath>

class Cart{
private:
  double x;
  double y;
  std::string units; // units are purely for printing

public:
  Cart(); // default
  Cart(double, double); // x, y
  Cart(double, double, std::string); // x, y, units

  // addition
  Cart& operator+=(const Cart& rhs){
    x += rhs.x;
    y += rhs.y;
    return *this;
  }
  friend Cart operator+(Cart lhs, const Cart& rhs){
    lhs += rhs;
    return lhs;
  }
  // subtraction
  Cart& operator-=(const Cart& rhs){
    x -= rhs.x;
    y -= rhs.y;
    return *this;
  }
  friend Cart operator-(Cart lhs, const Cart& rhs){
    lhs -= rhs;
    return lhs;
  }
  // multiplication
  Cart& operator*=(const Cart& rhs){
    x *= rhs.x;
    y *= rhs.y;
    return *this;
  }
  friend Cart operator*(Cart lhs, const Cart& rhs){
    lhs *= rhs;
    return lhs;
  }
  Cart& operator*=(const double& rhs){
    x *= rhs;
    y *= rhs;
    return *this;
  }
  friend Cart operator*(Cart lhs, const double& rhs){
    lhs *= rhs;
    return lhs;
  }
  // division
  Cart& operator/=(const Cart& rhs){
    x /= rhs.x;
    y /= rhs.y;
    return *this;
  }
  friend Cart operator/(Cart lhs, const Cart& rhs){
    lhs /= rhs;
    return lhs;
  }
  Cart& operator/=(const double& rhs){
    x /= rhs;
    y /= rhs;
    return *this;
  }
  friend Cart operator/(Cart lhs, const double& rhs){
    lhs /= rhs;
    return lhs;
  }

  // accessors
  double xComp() const;
  double yComp() const;
  double mod2() const;
  double mod() const;
  void print() const;

  // mutators
  void setX(double);
  void setY(double);
  void set(double, double);
  void dX(double);
  void dY(double);
  void d(double, double);

  void setUnits(std::string);

  double dist2(Cart&) const;
  double dist(Cart&) const;

  Cart unitVect() const; // return this vector with mod = 1
  Cart unitVect2(Cart&) const; // unit vector in direction of argument

};

#endif
