#include "RandomWalkTeam.h"
#include "ManagedTeam.h"

RandomWalkTeam::RandomWalkTeam() {

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

void RandomWalkTeam::setNormAI(AI* ai){
  normMetAI = *ai;
}
AI* RandomWalkTeam::getNormAI(){
  //std::cout << "metricNormAI\n";
  return &normMetAI;
}
void RandomWalkTeam::setSmartAI(RandomWalk* ai){
  smartMetAI = *ai;
}
AI* RandomWalkTeam::getSmartAI(){
  //std::cout << "metricSmartAI\n";
  return &smartMetAI;
}
