#ifndef TEAM_H
#define TEAM_H

#include<memory>
#include "Player.h"
#include "cfg/TeamConfigFile.h"
#include "Pitch.h"
#include "Frame.h"

class Match;

class TeamStrategy;

class Team{
    private:
        bool home_team;
        std::unique_ptr<Player[]> players;
        int player_count;

        TeamStrategy* teamstrat;

        TeamConfigFile tcf;


    public:
        Team();
        Team(TeamConfigFile, bool, Pitch&);
        
        Team(Team&);
        Team(Team&& team) { *this = std::move(team); }

        Team& operator=(Team&);
        Team& operator=(Team&&);

        Player& getPlayer(int);
        int getPlayerCount() const;

        bool isHomeTeam() const;

        void update(Frame);
        

 

};

#endif