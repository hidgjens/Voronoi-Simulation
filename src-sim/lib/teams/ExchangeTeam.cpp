#include "ExchangeTeam.h"
#include "ManagedTeam.h"

ExchangeTeam::ExchangeTeam() {

}
// MetricTeam::MetricTeam(std::string nm) {
//   buildTeam(nm, 11, 11);
// }
// MetricTeam::MetricTeam(std::string nm, int pc) {
//   buildTeam(nm, pc, pc);
// }
// MetricTeam::MetricTeam(std::string nm, int pc, int sc) {
//   buildTeam(nm, pc, sc);
// }

void ExchangeTeam::setNormAI(RandomWalk* ai){
  normMetAI = *ai;
}
AI* ExchangeTeam::getNormAI(){
  return &normMetAI;
}
void ExchangeTeam::setSmartAI(ExchangeAI* ai){
  smartMetAI = *ai;
}
AI* ExchangeTeam::getSmartAI(){
  return &smartMetAI;
}
