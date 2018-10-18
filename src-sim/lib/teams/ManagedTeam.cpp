#include "ManagedTeam.h"

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
