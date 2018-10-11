#ifndef EXCHANGE_H
#define EXCHANGE_H

#include "../Match.h"
#include "Scenario.h"
#include "../ai/ExchangeAI.h"
#include "../ai/RandomWalk.h"

class Exchange: public Scenario {
private:
  RandomWalk hNormAI;
  ExchangeAI hSmartAI;
  RandomWalk aAI;

public:
  // default constructor
  Exchange();
  Exchange(std::string);
  Exchange(int, std::string);

  void buildScenario(int, int, int, int, double, double, std::string);
};

#endif
