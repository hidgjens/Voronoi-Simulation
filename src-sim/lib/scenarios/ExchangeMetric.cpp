#include "ExchangeMetric.h"

ExchangeMetric::ExchangeMetric(){
  setLegacy(false);
  buildScenario(11, 1, 11, 200, 105.0, 68.0, "1HExchange");
}

ExchangeMetric::ExchangeMetric(std::string fn){
  setLegacy(false);
  buildScenario(11, 1, 11, 200, 105.0, 68.0, fn);
}

ExchangeMetric::ExchangeMetric(int frms, std::string fn){
  setLegacy(false);
  buildScenario(11, 1, 11, frms, 105.0, 68.0, fn);
}

ExchangeMetric::ExchangeMetric(int frms, std::string fn, bool leg){
  setLegacy(leg);
  buildScenario(11, 1, 11, frms, 105.0, 68.0, fn);
}

void ExchangeMetric::buildScenario(int hm, int hSmrtCnt, int aw, int frms, double pitchX, double pitchY, std::string fn) {
  // set member variables
  setFilename(fn);
  setHomeCnt(hm);
  setAwayCnt(aw);
  setFrames(frms);
  setPitchDims(pitchX, pitchY);
  setHomeTeam(Team("Home", hm, hNormAI, hSmrtCnt, hSmartAI));
  setAwayTeam(Team("Away", aw, aAI));
}
