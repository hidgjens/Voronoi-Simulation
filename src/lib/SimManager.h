#ifndef SIMMANAGER_H
#define SIMMANAGER_H

#include "cfg/MatchConfigFile.h"
#include "cfg/TeamConfigFile.h"
#include "Match.h"
#include "Pitch.h"
#include "Team.h"
#include<iostream>


class SimManager{

private:
    MatchConfigFile mcf;
    TeamConfigFile home_tcf, away_tcf;
    int samples, start_num;

public:
    SimManager();
    SimManager(MatchConfigFile, TeamConfigFile, TeamConfigFile);

    // accessors and mutators
    void setMCF(MatchConfigFile);
    MatchConfigFile& getMCF();

    void setHomeTCF(TeamConfigFile);
    TeamConfigFile& getHomeTCF();
    void setAwayTCF(TeamConfigFile);
    TeamConfigFile& getAwayTCF();

    void setNumberOfSamples(int);
    int getNumberOfSamples() const;

    void setStartNum(int);
    int getStartNum() const;

    //Match buildMatch();
    void start(std::string);
    

};

#endif