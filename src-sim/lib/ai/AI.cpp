#include "AI.h"
#include "../Player.h"
#include "../Match.h"

AI::AI() : desc("unspecified") {}

AI::AI(std::string des) : desc(des) {}

void AI::setDesc(std::string des) {
  desc = des;
}
std::string AI::getDesc() const {
  return desc;
}

void AI::updateFrame(Player& plyr, Match& match) {
  //std::cout<< "AI UPDATE\n";
}
