#ifndef TEAMSTRATEGY_H
#define TEAMSTRATEGY_H

# define M_PI           3.14159265358979323846  /* pi */

#include<string>
#include<cmath>
#include "Cart.h"
#include "Pitch.h"
#include "Frame.h"
#include "Player.h"
#include "Team.h"
#include "cfg/TeamConfigFile.h"
#include<iostream>


class TeamStrategy{
    
    protected:
        Pitch* pitch_data;
        std::string description;
        double min_team_distance; // in units of step_size

    public:
        TeamStrategy(){}
        TeamStrategy(Pitch* pitch) : pitch_data(pitch) {}

        static TeamStrategy* CreateStrat(Pitch*, TeamConfigFile);

        void setDescription(std::string s) { description = s; }
        std::string getDescription() const { return description; }

        void setPitch(Pitch* pitch) { pitch_data = pitch; }

        virtual void updateTeam(Team&, Frame) {}


};

#endif