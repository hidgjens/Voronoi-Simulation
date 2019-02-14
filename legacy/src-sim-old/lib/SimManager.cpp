#include "SimManager.h"
#include <chrono>

SimManager::SimManager(){}
SimManager::SimManager(MatchConfigFile mcf_, TeamConfigFile hm_tcf_, TeamConfigFile aw_tcf_) 
: home_tcf(hm_tcf_)
, away_tcf(aw_tcf_)
, mcf(mcf_)
, legacy(false)
{
    home_tcf.setTeamName("Home");
    away_tcf.setTeamName("Away");
}

// accessors and mutators
void SimManager::setMCF(MatchConfigFile mcf_) { mcf = mcf_; }
MatchConfigFile& SimManager::getMCF() { return mcf; }

void SimManager::setHomeTCF(TeamConfigFile tcf_) { home_tcf = tcf_; }
TeamConfigFile& SimManager::getHomeTCF() { return home_tcf; }
void SimManager::setAwayTCF(TeamConfigFile tcf_) { away_tcf = tcf_; }
TeamConfigFile& SimManager::getAwayTCF() { return away_tcf; }

void SimManager::setNumberOfSamples(int nos_) { samples = nos_; }
int SimManager::getNumberOfSamples() const { return samples; }

void SimManager::setStartNum(int sn_) { start_num = sn_; }
int SimManager::getStartNum() const { return start_num; }

void SimManager::setLegacyFiles(bool leg) { legacy = leg; }
bool SimManager::getLegacyFiles() const { return legacy; }

Match SimManager::buildMatch() {
    Match match(mcf, home_tcf, away_tcf);
    match.initRandObjPos();
    return match;
}

void SimManager::start(std::string filename) {
      // get starting time
  auto t_start = std::chrono::high_resolution_clock::now();
  // run samples
  for (int i{start_num}; i < samples + start_num; i++){
    std::cout << "Sample " << i << "\n\n";
    // start match
    Match match = buildMatch();
    match.printPlayers();
    match.startSimulation(!legacy);
    match.saveSummaryToFile(filename, i);
    //match.saveMatchToFile(filename, i, legacy);
  }
  // finish time
  auto t_fin = std::chrono::high_resolution_clock::now();
  std::cout << "\nTime elapsed:\n"
    << std::chrono::duration_cast<std::chrono::seconds>(t_fin - t_start).count()
    << " seconds for " << samples << " samples\n";
}
