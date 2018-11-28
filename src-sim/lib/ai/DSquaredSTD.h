#ifndef DSQUAREDSTD_H
#define DSQUAREDSTD_H

#include "AI.h"
#include "../Player.h"
#include <vector>

class PitchBorder;
class Cart;


class DSquaredSTD: public AI {
private:
  int players_to_consider; // number of players to sum over
  int density_samples; // number of steps to check 0 -> 360

public:
  // constructors
  DSquaredSTD();
  DSquaredSTD(int);
  DSquaredSTD(int, int);

  // accessors and mutators
  void setPlayers_to_consider(int);
  int getPlayers_to_consider() const;

  void setDensity_samples(int);
  int getDensity_samples() const;

  void updateFrame(Player&, Match&);

  bool checkLegalMove(Cart, Match&);

  double calcD(std::vector<Player*>, PitchBorder*, Cart, int);

};

#endif
