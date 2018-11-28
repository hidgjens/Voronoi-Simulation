#ifndef DSQUARED_H
#define DSQUARED_H

#include "AI.h"
#include "../Player.h"

class DSquared: public AI {
private:
  int players_to_consider; // number of players to sum over
  int density_samples; // number of steps to check 0 -> 360

public:
  // constructors
  DSquared();
  DSquared(int);
  DSquared(int, int);

  // accessors and mutators
  void setPlayers_to_consider(int);
  int getPlayers_to_consider() const;

  void setDensity_samples(int);
  int getDensity_samples() const;

  void updateFrame(Player&, Match&);

};

#endif
