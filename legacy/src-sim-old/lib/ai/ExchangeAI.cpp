#include "ExchangeAI.h"
#include "../Match.h"
#include<cmath>

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

    // before anything else, check if there's a player within 4 meters, if so move directly away from them
  Player* nearestplayer = plyr.getTeamPtr()->nearestPlayer(plyr);
  double distance2nearest = nearestplayer->getPos().dist(plyrpos);
  if (distance2nearest < 3.0 * plyr.getMaxStep()) {
    Cart unitvector_away = nearestplayer->getPos().unitVect2(plyrpos); // here i have deliberately done this way round, i've worked out the unit vector from ally to player so i can just move in that direction
    double theta = atan2(unitvector_away.xComp(), unitvector_away.yComp());
    plyr.scatter(plyr.getMaxStep(), theta);
    plyr.checkLegalPosition(match);
    match.checkCollisions(plyr);
    return;
  }

  // find opponent team
  Team eteam;
  Cart targetPos;
  double met = 0.0;

  

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
      auto new_met = metricD(plyr, eplyr, match);

      if (new_met > met) {
        // current enemy player is closer than previously stored enemy player
        met = new_met;
        targetPos = eteam.getPos(i + 1);
      }
    }
  }

  if (met > 0){
    // make move
    plyr.setIPos(targetPos);
    plyr.checkLegalPosition(match);
    match.checkCollisions(plyr);
  }


}

Cart ExchangeAI::metricV(Player& test_plyr, Player& far_plyr, Match& match) {
  auto A_j = far_plyr.getCtrl(match);
  auto A_i = test_plyr.getCtrl(match);
  if (A_i >= A_j && test_plyr.getTeam() != far_plyr.getTeam()) {return Cart(0.0, 0.0);} // ignore smaller areas
  auto testplyr_pos = test_plyr.getPos();
  auto farplyr_pos = far_plyr.getPos();
  auto d_ij = testplyr_pos.dist(farplyr_pos);
  auto d_0 = sqrt(match.getPitchX() * match.getPitchY());
  double gamma;

  // need to check if the player is on the active side of the pitch
  /*
    Home Team possession => x >= 0
    Away Team possession => x <= 0
  */
  // first find out if current player is in possession, then if the far player is in the right half
  // if (match.getHomePossession()){
  //   // home team possession
  //   if (far_plyr.getPosX() < 0)
  //     return Cart(0.0, 0.0); // wrong side

  // } else {
  //   // away team possession
  //   if (far_plyr.getPosX() > 0)
  //     return Cart(0.0, 0.0);

  // }

  // check value of gamma
  if (test_plyr.getTeam() == far_plyr.getTeam()){
    if (test_plyr.getShirtNum() == far_plyr.getShirtNum()){
      // players are the same person
      gamma = 0.0;
    } else {
      // both same team
      gamma = -1.0 * ecf.same_coeff;
    }

  } else {
    // opposite teams
    gamma = ecf.opp_coeff;
  }
  auto scalar = A_j * exp(-1.0 * d_ij / (ecf.decay_coeff * d_0)) * gamma;
  auto unitVector = testplyr_pos.unitVect2(farplyr_pos);
  //std::cout << A_j << "\t" << d_ij << "\t" << d_0 << "\n";
  //std::cout << scalar << "\n";

  if (gamma == 0.0){
    return Cart(0.0, 0.0);
  } else
  {
    return unitVector * scalar;
  }
}

double ExchangeAI::metricD(Player& test_plyr, Player& far_plyr, Match& match) {
  return metricV(test_plyr, far_plyr, match).mod();
}
