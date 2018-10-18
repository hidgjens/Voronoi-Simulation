#ifndef EXCHANGEMETRIC_H
#define EXCHANGEMETRIC_H

#include "../Match.h"
#include "Scenario.h"
#include "../ai/ExchangeMetricAI.h"
#include "../ai/RandomWalk.h"

class ExchangeMetric: public Scenario {
private:
  RandomWalk hNormAI;
  ExchangeMetricAI hSmartAI;
  RandomWalk aAI;

public:
  // default constructor
  ExchangeMetric();
  ExchangeMetric(std::string);
  ExchangeMetric(int, std::string);
  ExchangeMetric(int, std::string, bool);

  void buildScenario(int, int, int, int, double, double, std::string);
};

#endif
