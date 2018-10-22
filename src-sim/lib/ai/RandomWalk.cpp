#include "RandomWalk.h"
#include "../Player.h"
#include "../Match.h"

RandomWalk::RandomWalk() {
  setDesc("RandomWalk");
}

// update
void RandomWalk::updateFrame(Player& plyr, Match& match) {

  // generate random magnitude and direction
  double radius = (((double) rand() / RAND_MAX) - 0.5) * plyr.getMaxStep();
  double phi = ((double) rand() / RAND_MAX) * 2 * M_PI ;
  // set player
  double dx = radius * cos(phi);
  double dy = radius * sin(phi);
  //std::cout << "RWUPDATE " << dx << " " << dy << "\n";
  plyr.setdPos(dx, dy);
  plyr.checkLegalPosition(match);
  match.checkCollisions(plyr);
}
