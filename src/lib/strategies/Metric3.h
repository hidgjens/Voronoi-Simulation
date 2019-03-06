#ifndef Metric3_H
#define Metric3_H

#include "../TeamStrategy.h"
#include <vector>

class Player;
class TeamConfigFile;

class Metric3: public TeamStrategy{
    private:
        double decay_coefficient;
        double repel_coefficient;
        double attract_coefficient;
        int num_nearest_neighbours;

    public:
        Metric3();
        Metric3(TeamConfigFile, PitchModel*);
        
        void updateTeam(Team&, Frame);
        
        void Metric3Method(Player*, Frame);

};

#endif