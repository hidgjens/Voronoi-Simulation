#ifndef RANDOMWALK_H
#define RANDOMWALK_H

#include "AI.h"
#include "../Player.h"

class RandomWalk: public AI{
public:
  // constructors
  RandomWalk();

  // update
  void updateFrame(Player& , Match& );
  // {
  //
  //   // generate random magnitude and direction
  //   double radius = (((double) rand() / RAND_MAX) - 0.5) * plyr.getMaxStep();
  //   double phi = ((double) rand() / RAND_MAX) * 2 * M_PI ;
  //   // set player
  //   double dx = radius * cos(phi);
  //   double dy = radius * sin(phi);
  //   std::cout << "RWUPDATE " << dx << " " << dy << "\n";
  //   plyr.setdPos(dx, dy);
  // }
};

#endif
