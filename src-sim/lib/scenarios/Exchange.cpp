#include "Exchange.h"

Exchange::Exchange(){
  buildScenario(11, 1, 11, 200, 105.0, 68.0, "1HExchange");
}

Exchange::Exchange(std::string fn){
  buildScenario(11, 1, 11, 200, 105.0, 68.0, fn);
}

Exchange::Exchange(int frms, std::string fn){
  buildScenario(11, 1, 11, frms, 105.0, 68.0, fn);
}

void Exchange::buildScenario(int hm, int hSmrtCnt, int aw, int frms, double pitchX, double pitchY, std::string fn) {
  // set member variables
  std::cout << "building\n";
  setFilename(fn);
  setHomeCnt(hm);
  setAwayCnt(aw);
  setFrames(frms);
  setPitchDims(pitchX, pitchY);
  setHomeTeam(Team("Home", hm, hNormAI, hSmrtCnt, hSmartAI));
  setAwayTeam(Team("Away", aw, aAI));
}
