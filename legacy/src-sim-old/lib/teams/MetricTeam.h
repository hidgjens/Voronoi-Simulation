#ifndef METRICTEAM_H
#define METRICTEAM_H

#include "../Team.h"
#include "../ai/ExchangeMetricAI.h"
#include "../ai/RandomWalk.h"
#include "ManagedTeam.h"
#include "../ai/AI.h"

class MetricTeam: public ManagedTeam{
  // probably not the best way to do this, but this an attempt at a more flexible way to build teams of different types. The smart and normal AI are stored in the managed team, the addresses of which are passed to the Team class stored within this managed team. Then there is one Managed Team object which contains everything a team needs, which can pass a reference to the team it is storing to a Match class
private:
  RandomWalk normMetAI;
  ExchangeMetricAI smartMetAI;
  double decay_const;
  double op_coeff;
  double sm_coeff;
public:
  // constructors
  MetricTeam();
  MetricTeam(double, double, double);
  // MetricTeam(std::string); // team name
  // MetricTeam(std::string, int); // team name, players
  // MetricTeam(std::string, int, int); // tema name, players, smart

  void setNormAI(RandomWalk*);
  void setSmartAI(ExchangeMetricAI*);
  AI* getNormAI();
  AI* getSmartAI();
};

#endif
