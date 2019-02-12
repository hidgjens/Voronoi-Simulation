#ifndef TEAMCONFIGFILE_H
#define TEAMCONFIGFILE_H

#include <string>
#include <unordered_map>

class TeamConfigFile{
    // object to store all the information provided by the config file

friend class Team;

private:
    // file info
    std::string configFileName;

protected:
    void parseConfigFile(const std::unordered_map<std::string, std::string> & );

public:
    ////// FILE CONTENTS

    int player_count;
    // AI
    std::string team_strat_raw;

    double min_team_dist;

    // exchange/metric
    double decay_coefficient;
    double attract_coefficient;
    double repel_coefficient;
    // spreading
    double edge_coefficient;
    double ally_coefficient;
    int players_to_consider;
    int maxi_samples;

    std::unordered_map<std::string, std::string> raw_parameters;


    TeamConfigFile();
    TeamConfigFile(std::string); // filename

    void loadFile(); // use the filename in class
    void loadFile(std::string); // overload the initial filename (if given)

    // accessors and mutators
    void setConfigFileName(std::string);
    std::string getConfigFileName() const;
};

#endif