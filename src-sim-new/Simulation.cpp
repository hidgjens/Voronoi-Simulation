#include "lib/cfg/MatchConfigFile.h"
#include "lib/cfg/TeamConfigFile.h"
#include "lib/SimManager.h"

int main(int argc, char* argv[]){
    srand (time(NULL));
    // check if enough args are given
    int args_required = 5;
    // match cfg, home cfg, away cfg, filename, samples, [starting number = 0]
    if (argc == 1 || (argc != args_required + 1 && argc != args_required + 2)) {
        std::cout << "Not enough arguments [" << (argc - 1) << "/" << args_required << "]\n"
            << "\t[1] - Match Config Filename\n"
            << "\t[2] - HomeTeam Config Filename\n"
            << "\t[3] - AwayTeam Config Filename\n"
            << "\t[4] - Filename\n"
            << "\t[5] - Samples\n"
            << "\t[6] - Starting file number [default: 0]\n"
        ;
        return 1;
    }

    // parse arguments
    std::string MCF_name, HM_TCF_name, AW_TCF_name, FILENAME;
    int SAMPLES, START_NUM;

    MCF_name = argv[1];
    HM_TCF_name = argv[2];
    AW_TCF_name = argv[3];
    FILENAME = argv[4];
    SAMPLES = std::stoi(argv[5]);
    if (argc == args_required + 2)
        START_NUM = std::stoi(argv[6]);
    else 
        START_NUM = 0;

    // load configs
    std::cout << "Loading configs...\n";
    std::cout << "\t-Match cfg\n";

    MatchConfigFile mcf(MCF_name);
    std::cout << "\t-Home cfg\n";

    TeamConfigFile htcf(HM_TCF_name);
    std::cout << "\t-Away cfg\n";
    TeamConfigFile atcf(AW_TCF_name);

    std::cout << htcf.getConfigFileName() << "\n";

    // build sim
    SimManager sim(mcf, htcf, atcf);
    sim.setNumberOfSamples(SAMPLES);
    sim.setStartNum(START_NUM);
    sim.start(FILENAME);

    return 0;
}