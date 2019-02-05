#ifndef TESTINGAI_H
#define TESTINGAI_H

#include<string>
#include<iostream>
#include "AI.h"

// forward declarations
class Player;
class Match;

class TestingAI: public AI{
private:
  std::string desc;
public:
  // constructors
  TestingAI(); // defaul constructor
  TestingAI(std::string); // desc

  void updateFrame(Player&, Match&);
};

#endif
