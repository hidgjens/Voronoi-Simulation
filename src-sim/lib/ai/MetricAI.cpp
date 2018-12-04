#include "MetricAI.h"
#include "../Match.h"
#include<fstream>
#include<cmath>
#include "influences/PitchEdge.h"

MetricAI::MetricAI() : decay_const(1.0), op_coeff(1.0), sm_coeff(1.0) {
  setDesc("Find nearest opponent with larger area and approach them in attempt to swap positions");
}

MetricAI::MetricAI(double d, double o, double s) : decay_const(d), op_coeff(o), sm_coeff(s) {
  setDesc("Find nearest opponent with larger area and approach them in attempt to swap positions");
}

void MetricAI::updateFrame(Player& plyr, Match& match){

  // get player position
  auto plyrpos = plyr.getPos();

  // get teams
  auto hteam = match.getHomeTeam();
  auto ateam = match.getAwayTeam();

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

  // F vector
  Cart F(0.0, 0.0);
  Player curplyr;

  // edge effects
  PitchBorder* pb = new PitchBorder(match.getPitchX(), match.getPitchY());
  double d0 = sqrt(match.getPitchX() * match.getPitchY());

  if (pb->homeEdge.perpendicularDistance(plyrpos) <= dscf.edge_limit)
    F += Cart(1.0, 0.0) * dscf.edge_scale * (1.0 / match.getPlayerCnt()) * exp(- 1.0 * pb->homeEdge.perpendicularDistance(plyrpos) / (ecf.decay_coeff * d0)); // 1/22 is roughly the avg. player control
  
  if (pb->awayEdge.perpendicularDistance(plyrpos) <= dscf.edge_limit)
    F += Cart(-1.0, 0.0) * dscf.edge_scale * (1.0 / match.getPlayerCnt()) * exp(- 1.0 * pb->awayEdge.perpendicularDistance(plyrpos) / (ecf.decay_coeff * d0)); // 1/22 is roughly the avg. player control
  
  if (pb->leftEdge.perpendicularDistance(plyrpos) <= dscf.edge_limit)
    F += Cart(0.0, -1.0) * dscf.edge_scale * (1.0 / match.getPlayerCnt()) * exp(- 1.0 * pb->leftEdge.perpendicularDistance(plyrpos) / (ecf.decay_coeff * d0)); // 1/22 is roughly the avg. player control
  
  if (pb->rightEdge.perpendicularDistance(plyrpos) <= dscf.edge_limit)
    F += Cart(0.0, 1.0) * dscf.edge_scale * (1.0 / match.getPlayerCnt()) * exp(- 1.0 * pb->rightEdge.perpendicularDistance(plyrpos) / (ecf.decay_coeff * d0)); // 1/22 is roughly the avg. player control

  // iterate through home team
  for (int i{0}; i < (hteam.getPlayerCount() + ateam.getPlayerCount()); i++) {
    // looping through players
    if (i < hteam.getPlayerCount()){
      //std::cout << "if\n";
      // i is home player
      curplyr = hteam.getPlayer(i + 1);
      //std::cout << curplyr.getPosX() << std::endl;
    } else {
      //std::cout << "else\n";
      int j = i - hteam.getPlayerCount();
      curplyr = ateam.getPlayer(j + 1);
      //std::cout << curplyr.getPos().xComp() << std::endl;
    }

    F += metricV(plyr, curplyr, match);
    // check if curplyr is the player
    // if (curplyr.getShirtNum() == plyr.getShirtNum() && curplyr.getTeam() == plyr.getTeam()) {
    //   // must be the same player
    //   continue;
    // }
    // not same player, calculate their metric value
    //
    // // scalars
    // auto area = curplyr.getCtrl(match);
    // auto curplyrpos = curplyr.getPos();
    // auto distance = plyrpos.dist(curplyrpos);
    // if (distance < 1.0) {
    //   distance = 1.0;
    // }
    // double gamma;
    //
    // if (plyr.getTeam() == curplyr.getTeam()){
    //   gamma = -0.1;
    // } else {
    //   gamma = 1.0;
    // }
    //
    // auto unitVect = plyrpos.unitVect2(curplyrpos);

    // F += unitVect * (gamma * area / (distance / sqrt(match.getPitchX() * match.getPitchY())));
    // Ffile << (gamma * area / (distance / sqrt(match.getPitchX() * match.getPitchY()))) << "\n";

  }

  // for (int i{0}; i < eteam.getPlayerCount(); i++) {
  //   // looping through players
  //   auto curplyr = eteam.getPlayer(i + 1);
  //   // check if curplyr is the player
  //   if (curplyr.getShirtNum() == plyr.getShirtNum() && curplyr.getTeam() == plyr.getTeam()) {
  //     // must be the same player
  //     continue;
  //   }
  //   // not same player, calculate their metric value
  //
  //   // scalars
  //   auto area = curplyr.getCtrl(match);
  //   auto curplyrpos = curplyr.getPos();
  //   auto distance = plyrpos.dist(curplyrpos);
  //   if (distance < 1.0) {
  //     distance = 1.0;
  //   }
  //   double gamma;
  //
  //   if (plyr.getTeam() == curplyr.getTeam()){
  //     gamma = -0.1;
  //   } else {
  //     gamma = 1.0;
  //   }
  //
  //   auto unitVect = plyrpos.unitVect2(curplyrpos);
  //
  //   F += unitVect * (gamma * area / (distance / sqrt(match.getPitchX() * match.getPitchY())));
  //   Ffile << (gamma * area / (distance / sqrt(match.getPitchX() * match.getPitchY()))) << "\n";
  //
  // }
  // Ffile.close();
  // make move
  //std::cout << F.mod() << std::endl;
  plyr.setIdPos(F.unitVect() * plyr.getMaxStep());
  plyr.checkLegalPosition(match);
  match.checkCollisions(plyr);

}

Cart MetricAI::metricV(Player& test_plyr, Player& far_plyr, Match& match) {
  //std::cout << test_plyr.getShirtNum() << "\t" << far_plyr.getShirtNum() << std::endl;
  auto A_j = far_plyr.getCtrl(match);
  auto A_i = test_plyr.getCtrl(match);
  if (A_i >= A_j && test_plyr.getTeam() != far_plyr.getTeam()) {return Cart(0.0, 0.0);} // ignore smaller areas only if opposing team
  //std::cout << A_j << std::endl;
  auto testplyr_pos = test_plyr.getPos();
  auto farplyr_pos = far_plyr.getPos();
  auto d_ij = testplyr_pos.dist(farplyr_pos);
  auto d_0 = sqrt(match.getPitchX() * match.getPitchY()) * ecf.decay_coeff;
  double gamma;

    // need to check if the player is on the active side of the pitch
  /*
    Home Team possession => x >= 0
    Away Team possession => x <= 0
  */
  // first find out if current player is in possession, then if the far player is in the right half
  if (match.getHomePossession()){
    // home team possession
    if (far_plyr.getPosX() < 0)
      return Cart(0.0, 0.0); // wrong side

  } else {
    // away team possession
    if (far_plyr.getPosX() > 0)
      return Cart(0.0, 0.0);

  }

  // check gamma

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
    gamma = 1.0 * ecf.opp_coeff;
  }
  auto scalar = A_j * exp(-1.0 * d_ij / d_0) * gamma;
  auto unitVector = testplyr_pos.unitVect2(farplyr_pos);
  //std::cout << A_j << "\t" << d_ij << "\t" << d_0 << "\n";
  //std::cout << scalar << "\n";

  if (gamma == 0.0){
    // catch the case where univector will fail
    return Cart(0.0, 0.0);
  } else
  {
    return unitVector * scalar;
    
  }

}

double MetricAI::metricD(Player& test_plyr, Player& far_plyr, Match& match) {
  return metricV(test_plyr, far_plyr, match).mod();
}
