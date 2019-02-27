#ifndef TEAM_H
#define TEAM_H

#include<memory>
#include "Player.h"
#include "cfg/TeamConfigFile.h"
#include "Pitch.h"
#include "Frame.h"
#include<iostream>

class Match;

class TeamStrategy;

class PitchModel;

class Team{
    private:
        bool home_team;
        std::unique_ptr<Player[]> players;
        int player_count;

        TeamStrategy* teamstrat;

        TeamConfigFile tcf;

        Pitch pitch_data;


    public:
        Team();
        Team(TeamConfigFile, bool, PitchModel*, Pitch);
        
        Team(Team&);
        Team(Team&& team) { *this = std::move(team); for (int i{0}; i < player_count; i++) players[i].setTeamPtr(this); }

        Team& operator=(Team&);
        Team& operator=(Team&&);

        Player& getPlayer(int);
        Player* getPlayerPtr(int);
        int getPlayerCount() const;

        bool isHomeTeam() const;

        void update(Frame);

        void addControl(int, double);
        void setZeroControl(int);
        double getControl(int);
        void normControl(double);

        Pitch getPitchData();
        double xlim();
        double ylim();

        Pitch* pitchptr() {
            return &pitch_data;
        }
        

 

};

#endif