#include "ExchangeAI.h"
#include "../Match.h"

ExchangeAI::ExchangeAI() {
  setDesc("Find nearest opponent with larger area and approach them in attempt to swap positions");
}

void ExchangeAI::updateFrame(Player& plyr, Match& match){
  // get player position
  auto plyrpos = plyr.getPos();
  // check the players team
  auto plyrteam = plyr.getTeam();
  // get plyrs control
  auto plyrctrl = plyr.getCtrl(match);
  // find opponent team
  Team eteam;
  Cart targetPos;
  Cart move;
  double dist2trgt = -1.0;

  if (plyrteam == "Home"){
    eteam = match.getAwayTeam();
  } else
  if (plyrteam == "Away"){
    eteam = match.getHomeTeam();
  } else {
    std::cout << "Unrecognised team " << plyrteam << "(this will crash now)\n";
  }

  for (int i{0}; i < eteam.getPlayerCount(); i++) {
    // looping through players
    if (eteam.getPlyrCtrl(i + 1, match) > plyrctrl) {
      // current enemy player has larger control than player
      if (eteam.getPos(i + 1).dist2(plyrpos) < dist2trgt || dist2trgt < 0) {
        // current enemy player is closer than previously stored enemy player
        targetPos = eteam.getPos(i + 1);
        dist2trgt = eteam.getPos(i + 1).dist2(plyrpos);
      }
    }
  }

  // now make move based on result
  if (dist2trgt < 0) {
    // no better area was found
    // do nothing
    move = Cart(0.0 , 0.0);
  } else {
    // better target was found, move towards it
    if (sqrt(dist2trgt) - 1.0 < plyr.getMaxStep()) {
      // player can reach the target, and will move 1 metre passed him
      //std::cout << sqrt(dist2trgt) + 1.0 << "\n";
      move = plyrpos.unitVect2(targetPos) * (sqrt(dist2trgt) + 1.0);
    } else {
      // move as much as possible in the direction of the target
      move = plyrpos.unitVect2(targetPos) * (plyr.getMaxStep());
      // if (move.mod() != 5){
      //   std::cout << move.mod() << "\n";
      // }
    }
  }
  // make move
  plyr.setdPos(move);
  plyr.checkSmartLegalPosition(match);



}
