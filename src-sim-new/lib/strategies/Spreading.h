#ifndef SPREADING_H
#define SPREADING_H

#include "../TeamStrategy.h"
#include<vector>

class Player;
class TeamConfigFile;
struct PlayerInfo;

class Spreading: public TeamStrategy{

    private:
        double edge_coefficient;
        double ally_coefficient;
        int players_to_consider;
        int samples_to_run;

    public:
        Spreading();
        Spreading(TeamConfigFile);

        void updateTeam(Team&, Frame);

        void spreadingMethod(Player&, Frame);

        double distanceToXedge(Cart);
        double distanceToYedge(Cart);

        Cart XedgePosition(Cart);
        Cart YedgePosition(Cart);

        double computeD(std::vector<PlayerInfo>, Cart);

};

#endif