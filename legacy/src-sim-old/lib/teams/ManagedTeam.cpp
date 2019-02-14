#include "ManagedTeam.h"
#include "RandomWalkTeam.h"
#include "ExchangeTeam.h"
#include "MetricTeam.h"

ManagedTeam::ManagedTeam() {
}
ManagedTeam::ManagedTeam(std::string nm) {
}
ManagedTeam::ManagedTeam(std::string nm, int pc) {
}
ManagedTeam::ManagedTeam(std::string nm, int pc, int sc) {
}

// accessors and mutators
void ManagedTeam::buildTeam(std::string nm, int pc, int sc) {
  setName(nm);
  setPlyrCnt(pc);
  setSmartCnt(sc);
  setTeam(Team(getName(), getPlyrCnt(), getNormAI(), getSmartCnt(), getSmartAI()));
}

// add new team types here
ManagedTeam* ManagedTeam::makeTeam(std::string team_type, std::string team_name, int plyr_cnt, int smrt_cnt, double d, double o, double s) {
  if (team_type == "RandomWalkers"){
    ManagedTeam* mngteam = new RandomWalkTeam;
    mngteam->buildTeam(team_name, plyr_cnt, smrt_cnt);
    return mngteam;
  } else
  if (team_type == "ExchangeTeam"){
    ManagedTeam* mngteam = new ExchangeTeam;
    mngteam->buildTeam(team_name, plyr_cnt, smrt_cnt);
    return mngteam;
  } else
  if (team_type == "MetricTeam"){
    ManagedTeam* mngteam = new MetricTeam(d, o, s);
    mngteam->buildTeam(team_name, plyr_cnt, smrt_cnt);
    return mngteam;
  } else {
    throw std::invalid_argument("Unrecognised team type: " + team_type + "\n");
  }
}

Team& ManagedTeam::getTeam() {
  return team;
}
void ManagedTeam::setTeam(Team tm) {
  team = tm;
}

void ManagedTeam::setNormAI(AI* ai){
  normAI = *ai;
}
AI* ManagedTeam::getNormAI(){
  return &normAI;
}
void ManagedTeam::setSmartAI(AI* ai){
  smartAI = *ai;
}
AI* ManagedTeam::getSmartAI(){
  return &smartAI;
}

void ManagedTeam::setPlyrCnt(int pl) {
  plyrCnt = pl;
}
int ManagedTeam::getPlyrCnt() const {
  return plyrCnt;
}
void ManagedTeam::setSmartCnt(int sm) {
  smartCnt = sm;
}
int ManagedTeam::getSmartCnt() const {
  return smartCnt;
}

void ManagedTeam::setName(std::string nm) {
  name = nm;
}
std::string ManagedTeam::getName() const {
  return name;
}
