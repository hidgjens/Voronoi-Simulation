#include "ExchangeMetricAI.h"
#include "../Match.h"
#include<fstream>
#include<cmath>

ExchangeMetricAI::ExchangeMetricAI() {
  setDesc("Find nearest opponent with larger area and approach them in attempt to swap positions");
}

void ExchangeMetricAI::updateFrame(Player& plyr, Match& match){

  // get player position
  auto plyrpos = plyr.getPos();

  // get teams
  auto hteam = match.getHomeTeam();
  auto ateam = match.getAwayTeam();

  // F vector
  Cart F(0.0, 0.0);
  Player curplyr;

  // // open F file
  // std::ofstream Ffile;
  // Ffile.open("F-results.txt");


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

Cart ExchangeMetricAI::metricV(Player& test_plyr, Player& far_plyr, Match& match) {
  //std::cout << test_plyr.getShirtNum() << "\t" << far_plyr.getShirtNum() << std::endl;
  auto A_j = far_plyr.getCtrl2(match);
  auto A_i = test_plyr.getCtrl2(match);
  if (A_i >= A_j && test_plyr.getTeam() != far_plyr.getTeam()) {return Cart(0.0, 0.0);} // ignore smaller areas only if opposing team
  //std::cout << A_j << std::endl;
  auto testplyr_pos = test_plyr.getPos();
  auto farplyr_pos = far_plyr.getPos();
  auto d_ij = testplyr_pos.dist(farplyr_pos);
  auto d_0 = sqrt(match.getPitchX() * match.getPitchY());
  double gamma;

  if (test_plyr.getTeam() == far_plyr.getTeam()){
    if (test_plyr.getShirtNum() == far_plyr.getShirtNum()){
      // players are the same person
      gamma = 0.0;
    } else {
      // both same team
      gamma = -1.0;
    }

  } else {
    // opposite teams
    gamma = 1.0;
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

double ExchangeMetricAI::metricD(Player& test_plyr, Player& far_plyr, Match& match) {
  return metricV(test_plyr, far_plyr, match).mod();
}
