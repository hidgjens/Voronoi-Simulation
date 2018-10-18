#include "TestingScenario.h"

TestingScenario::TestingScenario(){
  setLegacy(false);
  buildScenario(11, 1, 11, 200, 105.0, 68.0, "TestingScenario");
}

TestingScenario::TestingScenario(std::string fn){
  setLegacy(false);
  buildScenario(11, 1, 11, 200, 105.0, 68.0, fn);
}

TestingScenario::TestingScenario(int frms, std::string fn){
  setLegacy(false);
  buildScenario(11, 1, 11, frms, 105.0, 68.0, fn);
}

TestingScenario::TestingScenario(int frms, std::string fn, bool leg){
  setLegacy(leg);
  buildScenario(11, 1, 11, frms, 105.0, 68.0, fn);
}

void TestingScenario::buildScenario(int hm, int hSmrtCnt, int aw, int frms, double pitchX, double pitchY, std::string fn) {
  // set member variables
  setFilename(fn);
  setHomeCnt(hm);
  setAwayCnt(aw);
  setFrames(frms);
  setPitchDims(pitchX, pitchY);
  setHomeTeam(Team("Home", hm, hNormAI, hSmrtCnt, hSmartAI));
  setAwayTeam(Team("Away", aw, aAI));
}
