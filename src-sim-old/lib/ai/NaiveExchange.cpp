#include "NaiveExchange.h"
#include "../Match.h"
#include<cmath>

NaiveExchange::NaiveExchange() {
  setDesc("Find nearest opponent with larger area and approach them in attempt to swap positions");
}

void NaiveExchange::updateFrame(Player& plyr, Match& match){
  /*
    Find nearest player with more area.
  */
  // get player position
  auto plyrpos = plyr.getPos();
  // check the players team
  auto plyrteam = plyr.getTeam();
  // get plyrs control
  auto plyrctrl = plyr.getCtrl(match);


  // find opponent team
  Team eteam;
  Cart targetPos;
  double mindist = -1.0; // -ve so if nothing changes it's value it's easy to tell

  // temp variables
  double temp_dist;
  Cart temp_pos;

  

  if (plyrteam == "Home"){
    eteam = match.getAwayTeam();
  } else
  if (plyrteam == "Away"){
    eteam = match.getHomeTeam();
  } else {
    std::cout << "Unrecognised team " << plyrteam << " (this will crash now)\n";
  }

  for (int i{0}; i < eteam.getPlayerCount(); i++) {
    // looping through players
    if (eteam.getPlyrCtrl(i + 1, match) > plyrctrl) {
      // current enemy player has larger control than player
      auto eplyr = eteam.getPlayer(i + 1);
      // get position and distance
      temp_pos = eplyr.getPos();
      temp_dist = temp_pos.dist2(plyrpos);
      

      if (temp_dist < mindist || mindist < 0.0) {
        // current enemy player is closer than previously stored enemy player
        mindist = temp_dist;
        targetPos = temp_pos;
      }
    }
  }

  if (mindist > 0.0){
    // make move
    plyr.setIPos(targetPos);
    plyr.checkLegalPosition(match);
    match.checkCollisions(plyr);
  }


}
