#ifndef SCENARIOFROMTEAMS_H
#define SCENARIOFROMTEAMS_H

#include "../Match.h"
#include "../teams/ManagedTeam.h"
#include<chrono>

class ScenarioFromTeams{
private:
  std::string filename;
  double pitchX;
  double pitchY;
  int frames;
  ManagedTeam* hmMTeam;
  ManagedTeam* awMTeam;
public:
  // default constructor
  ScenarioFromTeams();
  ScenarioFromTeams(ManagedTeam&, ManagedTeam&);
  ScenarioFromTeams(ManagedTeam&, ManagedTeam&, std::string);
  ScenarioFromTeams(ManagedTeam&, ManagedTeam&, int, std::string);

  Match buildMatch();
  void start(int, bool);
  void start(int, int, bool);
};

#endif
