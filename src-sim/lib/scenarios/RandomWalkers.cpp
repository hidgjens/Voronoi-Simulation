#include "RandomWalkers.h"

RandomWalkers::RandomWalkers(){
  setLegacy(false);
  buildScenario(11, 11, 200, 105.0, 68.0, "1HExchange");
}

RandomWalkers::RandomWalkers(std::string fn){
  setLegacy(false);
  buildScenario(11, 11, 200, 105.0, 68.0, fn);
}

RandomWalkers::RandomWalkers(int frms, std::string fn){
  setLegacy(false);
  buildScenario(11, 11, frms, 105.0, 68.0, fn);
}

RandomWalkers::RandomWalkers(int frms, std::string fn, bool leg){
  setLegacy(leg);
  buildScenario(11, 11, frms, 105.0, 68.0, fn);
}

void RandomWalkers::buildScenario(int hm, int aw, int frms, double pitchX, double pitchY, std::string fn) {
  // set member variables
  std::cout << "building\n";
  setFilename(fn);
  setHomeCnt(hm);
  setAwayCnt(aw);
  setFrames(frms);
  setPitchDims(pitchX, pitchY);
  setHomeTeam(Team("Home", hm, hAI));
  setAwayTeam(Team("Away", aw, aAI));
}
