#include "ScenarioFromTeams.h"

ScenarioFromTeams::ScenarioFromTeams() : filename("ScenarioFromTeams"), pitchX(105.0), pitchY(68.0), frames(100) {

}
ScenarioFromTeams::ScenarioFromTeams(ManagedTeam& hm, ManagedTeam& aw) : filename("ScenarioFromTeams"), pitchX(105.0), pitchY(68.0), frames(100) {
  hmMTeam = &hm;
  awMTeam = &aw;
}
ScenarioFromTeams::ScenarioFromTeams(ManagedTeam& hm, ManagedTeam& aw, std::string fn) : filename(fn), pitchX(105.0), pitchY(68.0), frames(100) {
  hmMTeam = &hm;
  awMTeam = &aw;
}
ScenarioFromTeams::ScenarioFromTeams(ManagedTeam& hm, ManagedTeam& aw, int fms, std::string fn) : filename(fn), pitchX(105.0), pitchY(68.0), frames(fms) {
  hmMTeam = &hm;
  awMTeam = &aw;
}

Match ScenarioFromTeams::buildMatch() {
  Match match = Match(frames, hmMTeam->getTeam(), awMTeam->getTeam(), pitchX, pitchY);
  match.initRandObjPos();
  return match;
}

void ScenarioFromTeams::start(int samples, bool leg) {
  start(samples, 0, leg);
}

void ScenarioFromTeams::start(int samples, int init_num, bool leg) {
  // get starting time
  auto t_start = std::chrono::high_resolution_clock::now();
  // run samples
  for (int i{init_num}; i < samples + init_num; i++){
    std::cout << "Sample " << i << "\n\n";
    // start match
    Match match = buildMatch();
    match.printPlayers();
    match.startSimulation(!leg);
    match.saveMatchToFile(filename + "_" + std::to_string(i), leg);
  }
  // finish time
  auto t_fin = std::chrono::high_resolution_clock::now();
  std::cout << "\nTime elapsed:\n"
    << std::chrono::duration_cast<std::chrono::seconds>(t_fin - t_start).count()
    << " seconds for " << samples << " samples\n";

}
