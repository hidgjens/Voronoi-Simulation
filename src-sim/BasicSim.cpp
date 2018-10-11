#include<iostream>
#include<string>
#include<chrono>
#include "lib/scenarios/Scenario.h"
#include "lib/scenarios/UnitPolygon.h"
#include "lib/scenarios/Exchange.h"
#include "lib/scenarios/RandomWalkers.h"

using namespace std;

int main(int argc, char *argv[]){
  int args_required = 4;
  cout << argv[4] << " " << argc << endl;
  if ((argc != args_required + 1 && argv[4] != "UnitPolygon") && (argc != args_required + 2 && argv[4] == "UnitPolygon")){
    cout << "Not enough arguments [" << (argc - 1) << "/" << args_required << "]\n"
      << "\t[1] - Filename\n"
      << "\t[2] - Samples\n"
      << "\t[3] - Starting number (for filename [use 0 if not threading])\n"
      << "\t[4] - Mode (RandomWalk/1HRandomWalk/1HExchange/UnitPolygon)\n"
      << "\t[5] - Number of players/sides (for UnitPolygon)\n";
    return 1;
  }
  string filename{argv[1]};
  int samples = stoi(argv[2]);
  int start_num = stoi(argv[3]);
  //string mode{"1hrandomwalk"};
  string mode = argv[4];
  int frms = 200;

  // random seed for generating player positions
  srand (time(NULL));
  cout << mode << "\n";
  if (mode == "RandomWalk"){
    // randomwalk
    cout << "All players have a random walk\n\n";
    Scenario scen(frms, filename);
    scen.start(samples, start_num);
  } else
  if (mode == "1HRandomWalk"){
    cout << "1H player has a random walk and everyone else is stationary";
    RandomWalkers rndm(frms, filename);
    rndm.start(samples, start_num);
  } else
  if (mode == "1HExchange"){
    // 1 home player with random walk
    cout << "1H uses exchange AI all other players have a random walk\n\n";
    Exchange exch(frms, filename);
    exch.start(samples, start_num);
  } else
  if (mode == "UnitPolygon"){
    // 1H player surround by players in a unit polygon
    UnitPolygon unit(stoi(argv[5]), 2, filename);
    unit.start(samples, start_num);
  } else {
    cout << "Unrecognised mode '" << mode << "'\n";
    return 1;
  }

  return 0;
}
