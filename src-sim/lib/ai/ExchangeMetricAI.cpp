#include "ExchangeAI.h"
#include "../Match.h"

ExchangeAI::ExchangeAI() {
  setDesc("Find nearest opponent with larger area and approach them in attempt to swap positions");
}

void ExchangeAI::updateFrame(Player& plyr, Match& match){
  // get player position
  auto plyrpos = plyr.getPos();

  // get teams
  Team hteam = match.getHomeTeam();
  Team eteam = match.getAwayTeam();

  // F vector
  Cart F(0.0, 0.0);

  // iterate through home team
  for (int i{0}; i < hteam.getPlayerCount(); i++) {
    // looping through players
    auto curplyr = hteam.getPlayer(i + 1);
    // check if curplyr is the player
    if (curplyr.getShirtNum() == plyr.getShirtNum() && curplyr.getTeam() == plyr.getTeam()) {
      // must be the same player
      continue;
    }
    // not same player, calculate their metric value

    // scalars
    auto area = curplyr.getCtrl(match);
    auto curplyrpos = curplyr.getPos();
    auto distance = plyrpos.dist(curplyrpos);
    double gamma;

    if (plyr.getTeam() == curplyr.getTeam()){
      gamma = -1.0;
    } else {
      gamma = 1.0;
    }

    auto unitVect = plyrpos.unitVect2(curplyrpos);

    F += unitVect * (gamma * area / (distance / sqrt(match.getPitchX() * match.getPitchY())));

  }

  for (int i{0}; i < eteam.getPlayerCount(); i++) {
    // looping through players
    auto curplyr = eteam.getPlayer(i + 1);
    // check if curplyr is the player
    if (curplyr.getShirtNum() == plyr.getShirtNum() && curplyr.getTeam() == plyr.getTeam()) {
      // must be the same player
      continue;
    }
    // not same player, calculate their metric value

    // scalars
    auto area = curplyr.getCtrl(match);
    auto curplyrpos = curplyr.getPos();
    auto distance = plyrpos.dist(curplyrpos);
    double gamma;

    if (plyr.getTeam() == curplyr.getTeam()){
      gamma = -1.0;
    } else {
      gamma = 1.0;
    }

    auto unitVect = plyrpos.unitVect2(curplyrpos);

    F += unitVect * (gamma * area / sqrt(match.getPitchX() * match.getPitchY()));

  }

  // make move
  plyr.setdPos(F);
  plyr.checkSmartLegalPosition(match);

}
