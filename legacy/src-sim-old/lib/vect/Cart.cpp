#include "Cart.h"
// constructors
Cart::Cart() : x(-5000.0), y(+5000.0), units(""){}

Cart::Cart(double x_, double y_) : x(x_), y(y_), units(""){}

Cart::Cart(double x_, double y_, std::string units_) : x(x_), y(y_), units(units_){}

// accessors
double Cart::xComp() const{
  return x;
}
double Cart::yComp() const{
  return y;
}
double Cart::mod2() const{
  return x * x + y * y;
}
double Cart::mod() const{
  return sqrt(mod2());
}
void Cart::print() const{
  std::cout << "(" << x << ", " << y << ") " << units;
}

// mutators
void Cart::setX(double nx){
  x = nx;
}
void Cart::setY(double ny){
  y = ny;
}
void Cart::set(double nx, double ny){
  setX(nx); setY(ny);
}
void Cart::dX(double dx){
  x += dx;
}
void Cart::dY(double dy){
  y += dy;
}
void Cart::d(double dx, double dy){
  dX(dx); dY(dy);
}

void Cart::setUnits(std::string nunits){
  units = nunits;
}

double Cart::dist2(Cart& trgt) const{
  double dx = trgt.x - x;
  double dy = trgt.y - y;
  return dx * dx + dy * dy;
}
double Cart::dist(Cart& trgt) const{
  return sqrt(dist2(trgt));
}

Cart Cart::unitVect() const {
  auto md = mod();
  if (md == 0.0){
    // divide by zero error
    return Cart(0.0, 0.0);
  } else {
    // safe
    return *this / mod();
  }
}
Cart Cart::unitVect2(Cart& trgt) const {
  auto dst = dist(trgt);
  if (dst == 0.0){
    // divide by zero error
    return Cart(0.0, 0.0);
  } else {
    // safe
    return (trgt - *this) / dist(trgt);
  }
}
