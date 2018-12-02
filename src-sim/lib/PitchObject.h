#ifndef PITCHOBJECT_H
#define PITCHOBJECT_H

#include<iostream>
#include<string>
#include<cmath>
#include<time.h>
#include<stdlib.h>
#include<stdio.h>
#include "vect/Cart.h"

class Match;

class PitchObject {
  // base class for object on pitch
protected:
  // object name
  std::string name;
  // position
  Cart pos;
  // velocity
  Cart vel;

  double hitbox_radius;

public:
  // Default constructor
  PitchObject();

  // accessors and mutators
  // name
  std::string getName() const;
  void setName(std::string);

  double getObjectRadius() const;
  void setObjectRadius(double);
  // position
  double getPosX() const;
  double getPosY() const;
  Cart getPos() const;
  void setPos(Cart);
  void setPos(double, double);
  void setPosX(double);
  void setPosY(double);

  void setdPos(Cart);
  void setdPos(double, double);
  void setdPosX(double);
  void setdPosY(double);


  void printPos() const;
  void randPos(Match&);

  void updateFrame(Match&);
  void smartUpdate(Match&);
  void checkLegalPosition(Match&);
  void checkSmartLegalPosition(Match&);

  void scatter(double, double);



};
#endif
