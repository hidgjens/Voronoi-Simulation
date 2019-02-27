#ifndef TEAMEXCHANGE_H
#define TEAMEXCHANGE_H

#include "../TeamStrategy.h"
#include<vector>
#include<algorithm>

class TeamExchange: public TeamStrategy{
    private:
        int targets_to_prioritise; // how many targets get 2 players assigned

    public:
        TeamExchange();
        TeamExchange(TeamConfigFile, PitchModel*);

        void updateTeam(Team&, Frame);

        

};


#endif