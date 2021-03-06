#include<iostream>
#include<string>
#include<chrono>
// probably going to remove these
#include "lib/scenarios/Scenario.h"
#include "lib/scenarios/UnitPolygon.h"
#include "lib/scenarios/Exchange.h"
#include "lib/scenarios/ExchangeMetric.h"
#include "lib/scenarios/RandomWalkers.h"
#include "lib/scenarios/TestingScenario.h"
// actually useful:
#include "lib/scenarios/ScenarioFromTeams.h"
#include "lib/teams/ManagedTeam.h"
#include "lib/teams/MetricTeam.h"
#include "lib/teams/ExchangeTeam.h"
#include "lib/teams/RandomWalkTeam.h"

using namespace std;

int main(int argc, char *argv[]){
  int args_required = 6;

  if (argc == 1 || ((argc != args_required + 1 && argv[4] != "UnitPolygon") && (argc != args_required + 2 && argv[4] == "UnitPolygon"))){
    cout << "Not enough arguments [" << (argc - 1) << "/" << args_required << "]\n"
      << "\t[1] - Filename\n"
      << "\t[2] - Samples\n"
      << "\t[3] - Frames\n"
      << "\t[4] - Starting number (for filename [use 0 if not threading])\n"
      << "\t[5] - Legacy CSVs? [yes/no]\n"
      << "\t[6] - Mode (RandomWalk/1HRandomWalk/1HExchange/1HMetExch/UnitPolygon/test)\n"
      << "\t[7] - Number of players/sides (for UnitPolygon)\n";
    return 0;
  }
  // parse arugments
  string filename{argv[1]};
  int samples = stoi(argv[2]);
  int frames = stoi(argv[3]);
  int start_num = stoi(argv[4]);
  string leg = argv[5];
  bool legacy;
  // check whether yes or no was passwed for legacy voronois
  if (leg == "yes") {
    legacy = true;
  } else
  if (leg == "no") {
    legacy = false;
  } else {
    cout << "Please give yes or no answer, not '" << leg << "'\n";
    return 1;
  }

  string mode = argv[6];

  // random seed for generating player positions
  srand (time(NULL));
  cout << mode << "\n";
  if (mode == "1HRandomWalk"){
    // randomwalk
    cout << "1H has random walk, everyone else is stationary\n\n";
    Scenario scen(frames, filename);
    scen.start(samples, start_num, legacy);
  } else
  if (mode == "RandomWalk"){
    cout << "All random walkers";
    RandomWalkers rndm(frames, filename);
    rndm.start(samples, start_num, legacy);
  } else
  if (mode == "1HExchange"){
    // 1 home player with random walk
    cout << "1H uses exchange AI all other players have a random walk\n\n";
    Exchange exch(frames, filename);
    exch.start(samples, start_num, legacy);
  } else
  if (mode == "UnitPolygon"){
    // 1H player surround by players in a unit polygon
    UnitPolygon unit(stoi(argv[7]), frames, filename);
    unit.start(samples, start_num, legacy);
  } else
  if (mode == "1HMetExch"){
    ExchangeMetric exch(frames, filename);
    exch.start(samples, start_num, legacy);
  } else
  if (mode == "HmMetExch"){
    // make teams
    ManagedTeam* hmTm = ManagedTeam::makeTeam("MetricTeam", "Home", 11, 11);
    ManagedTeam* awTm = ManagedTeam::makeTeam("MetricTeam", "Away", 11, 0);

    // make matches
    ScenarioFromTeams scen(*hmTm, *awTm, frames, filename);
    scen.start(samples, start_num, legacy);
  } else
  if (mode == "AllMetExch"){
    MetricTeam homeTm;
    homeTm.buildTeam("Home", 11, 11);
    MetricTeam awayTm;
    awayTm.buildTeam("Away", 11, 11);
    // make matches
    ScenarioFromTeams scen(homeTm, awayTm, frames, filename);
    scen.start(samples, start_num, legacy);
  } else
  if (mode == "HmRandom") {
    RandomWalkTeam homeTm;
    homeTm.buildTeam("Home", 11, 11);
    RandomWalkTeam awayTm;
    awayTm.buildTeam("Away", 11, 0);
    // make matches
    ScenarioFromTeams scen(homeTm, awayTm, frames, filename);
    scen.start(samples, start_num, legacy);
  } else
  if (mode == "test"){

    TestingScenario testscen(frames, filename);
    testscen.start(samples, start_num, legacy);

  } else {
    cout << "Unrecognised mode '" << mode << "'\n";
    return 1;
  }

  return 0;
}
