#ifndef SPREADING_H
#define SPREADING_H

#include "AI.h"
#include "../Player.h"
#include "cfgs/DSquaredConfigFile.h"

class Spreading: public AI {
private:
  int players_to_consider; // number of players to sum over
  int density_samples; // number of steps to check 0 -> 360 for p(theta)
  DSquaredConfigFile dscf;


public:
  // constructors
  Spreading();
  Spreading(int);
  Spreading(int, int);

  // accessors and mutators
  void setPlayers_to_consider(int);
  int getPlayers_to_consider() const;

  void setDensity_samples(int);
  int getDensity_samples() const;

  // filter
  virtual double filter(double);

  void updateFrame(Player&, Match&);

};

#endif
