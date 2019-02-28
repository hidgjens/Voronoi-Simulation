#ifndef MetricN_H
#define MetricN_H

#include "../TeamStrategy.h"
#include <vector>

class Player;
class TeamConfigFile;

class MetricN: public TeamStrategy{
    private:
        double decay_coefficient;
        double repel_coefficient;
        double attract_coefficient;
        int num_nearest_neighbours;

    public:
        MetricN();
        MetricN(TeamConfigFile, PitchModel*);
        
        void updateTeam(Team&, Frame);
        
        void MetricNMethod(Player*, Frame);

        Cart MetricNVector(Player*, Cart, double); // test player, source position, source control

};

#endif