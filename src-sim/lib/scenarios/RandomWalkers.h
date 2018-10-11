#ifndef RANDOMWALKERS_H
#define RANDOMWALKERS_H

#include "../Match.h"
#include "Scenario.h"
#include "../ai/RandomWalk.h"

class RandomWalkers: public Scenario {
private:
  RandomWalk hAI;
  RandomWalk aAI;

public:
  // default constructor
  RandomWalkers();
  RandomWalkers(std::string);
  RandomWalkers(int, std::string);

  void buildScenario(int, int, int, double, double, std::string);
};

#endif
