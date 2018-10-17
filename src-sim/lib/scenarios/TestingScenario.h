#ifndef TESTINGSCENARIO_H
#define TESTINGSCENARIO_H

#include "../Match.h"
#include "Scenario.h"
#include "../ai/TestingAI.h"
#include "../ai/RandomWalk.h"

class TestingScenario: public Scenario {
private:
  RandomWalk hNormAI;
  TestingAI hSmartAI;
  RandomWalk aAI;

public:
  // default constructor
  TestingScenario();
  TestingScenario(std::string);
  TestingScenario(int, std::string);
  TestingScenario(int, std::string, bool);

  void buildScenario(int, int, int, int, double, double, std::string);
};

#endif
