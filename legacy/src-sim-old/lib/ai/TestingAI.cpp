#include "TestingAI.h"
#include "../Player.h"
#include "../Match.h"

TestingAI::TestingAI() : desc("unspecified") {}

TestingAI::TestingAI(std::string des) : desc(des) {}

void TestingAI::updateFrame(Player& plyr, Match& match) {
  std::cout << "update\n";
  
  // check if move is allowed
  plyr.checkLegalPosition(match);
  match.checkCollisions(plyr);
}
