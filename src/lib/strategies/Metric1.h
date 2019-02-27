#ifndef Metric1_H
#define Metric1_H

#include "../TeamStrategy.h"

class Player;
class TeamConfigFile;

class Metric1: public TeamStrategy{
    private:
        double decay_coefficient;
        double repel_coefficient;
        double attract_coefficient;

    public:
        Metric1();
        Metric1(TeamConfigFile, Pitch*);

        void updateTeam(Team&, Frame);
        
        void Metric1Method(Player*, Frame);

        Cart Metric1Vector(Player*, Cart, double); // test player, source position, source control

};

#endif