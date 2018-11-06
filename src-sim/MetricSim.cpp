#include<string>
#include<iostream>
#include "lib/teams/ManagedTeam.h"
#include "lib/scenarios/ScenarioFromTeams.h"

using namespace std;

int main(int argc, char* argv[]){

    // check if enough arguments were given
    int args_required = 7; 
    // d, o, s, filename, legacy, samples, frames, starting num = 0
    if (argc == 1 || (argc != args_required + 1 && argc != args_required + 2)){
        cout << "Not enough arguments [" << (argc - 1) << "/" << args_required << "]\n"
            << "\t[1] - Decay const\n"
            << "\t[2] - Opposite Coeff\n"
            << "\t[3] - Same Coeff\n"
            << "\t[4] - Filename\n"
            << "\t[5] - Legacy CSV? [yes/no]\n"
            << "\t[6] - Samples\n"
            << "\t[7] - Frames\n"
            << "\t[8] - Starting Number [default: 0]\n";

        return 0;
    }

    // parse arguments
    double decay = stod(argv[1]);
    double opp = stod(argv[2]);
    double same = stod(argv[3]);
    string filename{argv[4]};
    string leg{argv[5]};

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

    int samples = stoi(argv[6]);
    int frames = stoi(argv[7]);
    int start_num = 0;
    if (argc == args_required + 2){
        start_num = stoi(argv[args_required + 1]);
    }

    // random seed
    srand (time(NULL));

    // make teams
    ManagedTeam* hmTm = ManagedTeam::makeTeam("MetricTeam", "Home", 11, 11, decay, opp, same);
    ManagedTeam* awTm = ManagedTeam::makeTeam("MetricTeam", "Away", 11, 0);

    // make matches
    ScenarioFromTeams scen(*hmTm, *awTm, frames, filename);
    scen.start(samples, start_num, legacy);
    return 0;
}