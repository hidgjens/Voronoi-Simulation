#ifndef METRIC_H
#define METRIC_H

#include "../TeamStrategy.h"

class Player;
class TeamConfigFile;

class Metric: public TeamStrategy{
    private:
        double decay_coefficient;
        double repel_coefficient;
        double attract_coefficient;

    public:
        Metric();
        Metric(TeamConfigFile, PitchModel*);

        void updateTeam(Team&, Frame);
        
        void metricMethod(Player*, Frame);

        Cart metricVector(Player*, Cart, double); // test player, source position, source control

};

#endif