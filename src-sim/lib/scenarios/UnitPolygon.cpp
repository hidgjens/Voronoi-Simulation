#include "UnitPolygon.h"

UnitPolygon::UnitPolygon() : hmAI(), awAI() {
  buildScenario(4, 2, 105.0, 68.0, "UnitPolygon-unspecified");
}
UnitPolygon::UnitPolygon(int aw) : hmAI(), awAI() {
  buildScenario(aw, 2, 105.0, 68.0, "UnitPolygon-" + std::to_string(aw));
}
UnitPolygon::UnitPolygon(int aw, int frms, std::string fn) : hmAI(), awAI() {
  buildScenario(aw, frms, 105.0, 68.0, fn);
}
UnitPolygon::UnitPolygon(int aw, int frms, double pitchX, double pitchY) : hmAI(), awAI() {
  buildScenario(aw, frms, pitchX, pitchY, "UnitPolygon-" + std::to_string(aw));
}
UnitPolygon::UnitPolygon(int aw, int frms, double pitchX, double pitchY, std::string fn) : hmAI(), awAI() {
  buildScenario(aw, frms, pitchX, pitchY, fn);
}

void UnitPolygon::buildScenario(int aw, int frms, double pitchX, double pitchY, std::string fn) {
  // set member variables
  setFilename(fn);
  setHomeCnt(1);
  setAwayCnt(aw);
  setFrames(frms);
  setPitchDims(pitchX, pitchY);
  setHomeTeam(Team("Home", 1, hmAI));
  setAwayTeam(Team("Away", aw, awAI));
}

void UnitPolygon::start(int samples){
  start(samples, 0);
}
void UnitPolygon::start(int samples, int init_num){
  // get starting time
  auto t_start = std::chrono::high_resolution_clock::now();
  // run samples
  for (int i{init_num}; i < samples + init_num; i++){
    std::cout << "Sample " << i << "\n\n";
    //
    // start match
    Match match = buildMatch();
    match.printPlayers();
    match.startSimulation(true);
    match.saveMatchToFile(getFilename() + "_" + std::to_string(i));
  }
  // finish time
  auto t_fin = std::chrono::high_resolution_clock::now();
  std::cout << "\nTime elapsed:\n"
    << std::chrono::duration_cast<std::chrono::seconds>(t_fin - t_start).count()
    << " seconds for " << samples << " samples\n";
}

Match UnitPolygon::buildMatch() {
  Match match = Match(getFrames(), getHomeTeam(), getAwayTeam(), getPitchX(), getPitchY());
  match.initUnitPolygon();
  return match;
}
