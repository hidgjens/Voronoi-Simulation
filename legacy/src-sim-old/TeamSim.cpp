#include<string>
#include<iostream>
#include "lib/teams/ManagedTeam.h"
#include "lib/scenarios/ScenarioFromTeams.h"

using namespace std;

int main(int argc, char* argv[]){

    // check if enough arguments were given
    int args_required = 8; 
    // team1 type, smart plyrs, team2 type, smart plyrs, filename, legacy, samples, frames, starting num = 0
    if (argc == 1 || (argc != args_required + 1 && argc != args_required + 2)){
        cout << "Not enough arguments [" << (argc - 1) << "/" << args_required << "]\n"
            << "\t[1] - Home Team type\n"
            << "\t[2] - Home Smart Count\n"
            << "\t[3] - Away Team Type\n"
            << "\t[4] - Away Smart Count\n"
            << "\t[5] - Filename\n"
            << "\t[6] - Legacy CSV? [yes/no]\n"
            << "\t[7] - Samples\n"
            << "\t[8] - Frames\n"
            << "\t[9] - Starting Number [default: 0]\n";

        return 0;
    }

    // parse arguments
    string hmType{argv[1]};
    int hmSC = stoi(argv[2]);
    string awType{argv[3]};
    int awSC = stoi(argv[4]);
    string filename{argv[5]};
    string leg{argv[6]};

    bool legacy;
    // check whether yes or no was passed for legacy voronois
    if (leg == "yes") {
        legacy = true;
    } else
    if (leg == "no") {
        legacy = false;
    } else {
        cout << "Please give yes or no answer, not '" << leg << "'\n";
        return 1;
    }

    int samples = stoi(argv[7]);
    int frames = stoi(argv[8]);
    int start_num = 0;
    if (argc == args_required + 2){
        start_num = stoi(argv[args_required + 1]);
    }

    // random seed
    srand (time(NULL));

    // make teams
    ManagedTeam* hmTm = ManagedTeam::makeTeam(hmType, "Home", 11, hmSC);
    ManagedTeam* awTm = ManagedTeam::makeTeam(awType, "Away", 11, awSC);

    // make matches
    ScenarioFromTeams scen(*hmTm, *awTm, frames, filename);
    scen.start(samples, start_num, legacy);
    return 0;
}