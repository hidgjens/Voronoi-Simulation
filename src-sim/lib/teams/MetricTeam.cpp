#include "MetricTeam.h"
#include "ManagedTeam.h"

MetricTeam::MetricTeam() {

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

void MetricTeam::setNormAI(RandomWalk* ai){
  normMetAI = *ai;
}
AI* MetricTeam::getNormAI(){
  //std::cout << "metricNormAI\n";
  return &normMetAI;
}
void MetricTeam::setSmartAI(ExchangeMetricAI* ai){
  smartMetAI = *ai;
}
AI* MetricTeam::getSmartAI(){
  //std::cout << "metricSmartAI\n";
  return &smartMetAI;
}
