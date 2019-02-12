#ifndef UNITPOLYGON_H
#define UNITPOLYGON_H

#include "../Match.h"
#include "Scenario.h"

class UnitPolygon: public Scenario {
private:
  RandomWalk hmAI;
  AI awAI;

public:
  // default constructor
  UnitPolygon();
  // parameterised constructors
  UnitPolygon(int); // sides (number of away players)
  UnitPolygon(int, int, std::string); // away players, frames, filename
  UnitPolygon(int, int, double, double); // away players, frames, pitchX, pitchY
  UnitPolygon(int, int, double, double, std::string); // away players, frames, pitchX, pitchY, filename
  UnitPolygon(int, int, double, double, std::string, bool); // away players, frames, pitchX, pitchY, filename
  // 
  // void start(int);
  // void start(int, int);

  void buildScenario(int, int, double, double, std::string);
  Match buildMatch();

};

#endif
